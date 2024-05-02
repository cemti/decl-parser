#include "pch.h"
#include "TypeParser.h"
using namespace System;
using namespace Collections::Generic;
using namespace Linq;

namespace DeclParser
{
	IList<NamedDeclaration>^ TypeParser::Variables::get()
	{
		return DeclArray[0];
	}
	
	TypeParser::TypeParser(String^ input, BaseType::DataModel dataModel, bool permissive) : _lexer(input), _permissive(permissive), _dataModel(dataModel)
	{
		FundamentalType::TypeLength length{ };

		if (dataModel >= BaseType::DataModel::LLP64)
			length = FundamentalType::TypeLength::LongLong;
		
		DeclArray = gcnew array<List<NamedDeclaration>^> { gcnew List<NamedDeclaration>, gcnew List<NamedDeclaration> };
		DeclaredTypes = gcnew Dictionary<String^, NamedType^>;
		
		try
		{
			Parse();
		}
		catch (Exception^ e)
		{
			if (!_lexer.HasNext && (e->GetType() == FormatException::typeid || e->GetType() == ArgumentException::typeid))
			{
				auto match = _lexer.CurrentMatch;
				throw gcnew FormatException(String::Format("At position {0} (length {1}): {2}", match->Index, match->Length, e->Message));
			}

			throw;
		}
	}

	void TypeParser::GreedySkipTokens()
	{
		for (; ; )
		{
			_lexer.MoveNext();

			if (_lexer.Value == ":")
				return;

			if (_lexer.Value == ";" || _lexer.Value == "{" || _lexer.Value == "}" || _lexer.Value == ")")
			{
				--_lexer.Index;
				return;
			}
		}
	}

	int TypeParser::SizeOf(BaseType^ type)
	{
		return type->SizeOf(_dataModel);
	}

	void TypeParser::Parse()
	{
		int declIndex{ };
		bool insideLoop{ }, insideForLoop{ };

		for (; ; )
		{
			if (_lexer.TryMoveNext(); _lexer.HasNext)
			{
				if (frame.Count == 0)
					return;

				throw gcnew FormatException("Unexpected end of file while parsing the body of scope '" + frame[frame.Count - 1] + "'.");
			}

			switch (auto group = _lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				if (_permissive && LookupTypedef(_lexer.Value) == nullptr)
				{
					bool insideDoLoop = _lexer.Value == "do";
					insideForLoop = _lexer.Value == "for";
					
					if (insideDoLoop || insideForLoop || _lexer.Value == "while" || _lexer.Value == "switch")
					{
						insideLoop = true;
						frame.Add(String::Format("{0}_{1}", _lexer.Value, _lexer.TextIndex));

						if (!insideDoLoop)
							_lexer.MoveNext();
					}
					else
						GreedySkipTokens();

					break;
				}

			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Type:
			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Qualifier:
			case Lexer::RegexGroups::Specifier:
			declaration:
				for (auto initialType = ParseInitialType(true); ; )
				{
					FunctionType^ functionType{ };
					auto initialDecl = gcnew Declaration(initialType);
					auto postType = ParsePostType(initialDecl);
					bool isFnDefinition = false;

					switch (declIndex)
					{
					case 0:
						if (functionType = dynamic_cast<FunctionType^>(postType.Declaration->Type))
						{
							if (!functionType->HasParameters)
								break;

							for each (auto parameter in functionType->Parameters)
								if (parameter.Name == nullptr && dynamic_cast<EllipsisType^>(parameter.Declaration->Type) == nullptr)
									throw gcnew FormatException("Parameter name omitted.");
						}
						else
						{
							if (postType.Declaration->Inline)
								throw gcnew FormatException("Only functions can be inline.");

							if (auto nType = dynamic_cast<NamedType^>(postType.Declaration->Type); nType && (!nType->Instantiable || nType->Empty))
								throw gcnew FormatException("Incomplete type.");
						}
						
						break;

					case 1:
						if (postType.Declaration->Specifier != Declaration::StorageSpecifier::None || postType.Declaration->Inline)
							throw gcnew FormatException("Storage qualifiers are not allowed in a typedef declaration.");
					}

					if (postType.Name == nullptr)
					{
						if (_lexer.Value != ";")
							throw gcnew FormatException("Variable name omitted.");

						if (group != Lexer::RegexGroups::SType)
							goto nextVariable;

						if (auto sType = dynamic_cast<StructType^>(postType.Declaration->Type); sType == nullptr || !sType->Anonymous)
							goto nextVariable;

						postType.Name = "__decl_" + _lexer.TextIndex;
					}

					isFnDefinition = declIndex == 0 && functionType && _lexer.Value == "{";
					postType.Name = String::Join("__", Enumerable::Append(%frame, postType.Name));

					for each (auto declaration in DeclArray[declIndex])
						if (declaration.Name && declaration.Name == postType.Name)
						{
							if (isFnDefinition && declaration.Declaration->Type->ToString() == postType.Declaration->Type->ToString())
							{
								postType = declaration;
								frame.Add(postType.Name + "_" + _lexer.TextIndex);
								break;
							}

							throw gcnew ArgumentException("Duplicate variable '" + declaration.Name + "'.");
						}

					DeclArray[declIndex]->Add(postType);

				nextVariable:
					if (_lexer.Value != ",")
					{
						if (isFnDefinition)
							frame.Add(postType.Name + "_" + _lexer.TextIndex);
						else
						{
							if (_lexer.Value != ";")
								goto unexpectedToken;

							declIndex = 0;
						}

						break;
					}

					_lexer.MoveNext();
				}

				break;

			default:
			unexpectedToken:
				if (!_permissive)
					throw gcnew FormatException("Unexpected token '" + _lexer.Value + "'.");

				if (insideLoop && _lexer.Value == ")")
				{
					insideForLoop = false;
					_lexer.MoveNext();

					if (_lexer.Value == ";")
					{
						insideLoop = false;
						frame.RemoveAt(frame.Count - 1);
					}
					else
						--_lexer.Index;
				}
					
				GreedySkipTokens();
				break;

			case Lexer::RegexGroups::ScopeBrace:
				if (frame.Count == 0)
					throw gcnew FormatException("Scope braces are not expected in root scope.");

				if (_lexer.Value == "{")
				{
					if (insideLoop && !insideForLoop)
						insideLoop = false;
					else
						frame.Add("scope_" + _lexer.TextIndex);
				}
				else if (_lexer.Value == "}")
					frame.RemoveAt(frame.Count - 1);

				break;

			case Lexer::RegexGroups::Special:
				if (_lexer.Value != ";")
				{
					if (_lexer.Value != "typedef")
					{
						if (_lexer.Value != "inline")
							goto unexpectedToken;
						
						goto declaration;
					}

					declIndex = 1;
				}
				else if (_permissive && !insideForLoop && insideLoop && _lexer.Value == ";")
				{
					insideLoop = false;
					frame.RemoveAt(frame.Count - 1);
				}
			}
		}
	}

	Declaration^ TypeParser::LookupTypedef(String^ name)
	{
		for each (auto typeDef in DeclArray[1])
			if (typeDef.Name == name)
				return typeDef.Declaration;

		return nullptr;
	}
}