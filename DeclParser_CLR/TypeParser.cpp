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
							if (functionType->HasNoParameters)
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

	Declaration^ TypeParser::ParseInitialType(bool allowDefinition)
	{
		FundamentalType::TypeLength length{ };
		FundamentalType::TypeSign sign{ };
		BaseType::TypeQualifiers qualifiers{ };
		Declaration::StorageSpecifier specifiers{ };
		BaseType^ type{ };
		bool isSealed{ }, isInline{ };

		for (; ; _lexer.MoveNext())
		{
			switch (_lexer.Group)
			{
			case Lexer::RegexGroups::SType:
				if (type)
					throw gcnew FormatException("Type already specified.");

				type = ParseNamedType(allowDefinition);
				isSealed = true;
				break;

			case Lexer::RegexGroups::Type:
				if (type)
					throw gcnew FormatException("Type already specified.");

				type = gcnew FundamentalType(Enum::Parse<FundamentalType::DataType>(_lexer.Value, true));
				break;

			case Lexer::RegexGroups::Length:
			{
				auto newLength = Enum::Parse<FundamentalType::TypeLength>(_lexer.Value, true);

				if (length != FundamentalType::TypeLength::None)
				{
					if (newLength != FundamentalType::TypeLength::Long || newLength != length)
						throw gcnew FormatException("More than sufficient length specifiers.");

					length = FundamentalType::TypeLength::LongLong;
				}
				else
					length = newLength;

				break;
			}

			case Lexer::RegexGroups::Sign:
				if (sign != FundamentalType::TypeSign::None)
					throw gcnew FormatException("Duplicate sign specifier.");

				sign = Enum::Parse<FundamentalType::TypeSign>(_lexer.Value, true);
				break;

			case Lexer::RegexGroups::Qualifier:
				qualifiers = qualifiers | Enum::Parse<BaseType::TypeQualifiers>(_lexer.Value, true);
				break;

			case Lexer::RegexGroups::Specifier:
			{
				auto parsedSpecifier = Enum::Parse<Declaration::StorageSpecifier>(_lexer.Value, true);

				if (parsedSpecifier == specifiers)
					throw gcnew FormatException("Duplicate '" + parsedSpecifier.ToString()->ToLower() + "'.");

				specifiers = parsedSpecifier;
				break;
			}

			case Lexer::RegexGroups::Name:
				if (auto typeDef = LookupTypedef(_lexer.Value); typeDef && type == nullptr)
				{
					type = typeDef->Type;
					isSealed = true;
					break;
				}

			default:
				if (_lexer.Value == "inline")
				{
					isInline = true;
					break;
				}

				if (auto hasLS = length != FundamentalType::TypeLength::None || sign != FundamentalType::TypeSign::None; type == nullptr)
				{
					if (!hasLS)
						throw gcnew FormatException("No type specified.");

					type = gcnew FundamentalType(FundamentalType::DataType::Int, sign, length);
				}
				else if (auto fType = dynamic_cast<FundamentalType^>(type); fType && !isSealed)
				{
					fType->Length = length;
					fType->Sign = sign;
				}
				else if (hasLS)
					throw gcnew FormatException("Length and sign qualifiers can be applied to fundamental types only.");

				if (qualifiers != BaseType::TypeQualifiers::None)
				{
					if (isSealed)
						type = static_cast<BaseType^>(type->Clone());

					type->SetQualifier(qualifiers);
				}

				return gcnew Declaration(type, specifiers, isInline);
			}
		}
	}

	NamedDeclaration TypeParser::ParsePostType(Declaration^ declaration)
	{
		Stack<Lexer::RegexGroups> groups;
		Stack<Nullable<int>> counts;
		String^ name{ };
		
		for (int endIndex = -1, nextIndex = -1; ; _lexer.MoveNext())
		{
			switch (_lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				if (name || groups.Count > 0)
					throw gcnew FormatException("Invalid declaration.");

				name = _lexer.Value;
				continue;

			case Lexer::RegexGroups::Qualifier:
				if (name || groups.Count > 0)
					throw gcnew FormatException("Invalid declaration.");

				declaration->Type->SetQualifier(Enum::Parse<FundamentalType::TypeQualifiers>(_lexer.Value, true));
				continue;

			case Lexer::RegexGroups::ArrayBrace:
			{
				if (_lexer.Value != "[")
					throw gcnew FormatException("'[' expected.");

				auto group = _lexer.Group;

				if (_lexer.MoveNext(); _lexer.Group == Lexer::RegexGroups::Integer)
				{
					counts.Push(int::Parse(_lexer.Value));
					_lexer.MoveNext();
				}
				else
					counts.Push({ });

				groups.Push(group);

				if (_lexer.Value != "]")
					throw gcnew FormatException("']' expected.");

				continue;
			}

			case Lexer::RegexGroups::Brace:
				if (_lexer.Value == ")")
				{
					if (endIndex == -1)
						endIndex = _lexer.Index;
					
					break;
				}

				_lexer.MoveNext();

				if (_lexer.Group < Lexer::RegexGroups::Brace || _lexer.Value == ")" ||
					(_lexer.Group == Lexer::RegexGroups::Name && LookupTypedef(_lexer.Value) != nullptr))
				{
					counts.Push(_lexer.Index--);
					groups.Push(_lexer.Group);
				}
				else
				{
					if (name || nextIndex != -1)
						throw gcnew FormatException("Invalid declaration");

					nextIndex = --_lexer.Index;
				}

				_lexer.Skip();
				continue;

			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Type:
				throw gcnew FormatException("Type not expected here.");

			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Specifier:
				throw gcnew FormatException("Type length/sign/storage qualifiers not expected here.");

			case Lexer::RegexGroups::Special:
				if (_lexer.Value == "*")
				{
					if (name || groups.Count > 0)
						throw gcnew FormatException("Invalid declaration.");

					declaration->Type = gcnew PointerType(declaration->Type);
					continue;
				}

			default:
				if (endIndex != -1)
					throw gcnew FormatException("Invalid declaration");
			
				endIndex = _lexer.Index;
			}

			while (groups.Count > 0)
			{
				switch (groups.Pop())
				{
				case Lexer::RegexGroups::ArrayBrace:
				{
					if (dynamic_cast<FunctionType^>(declaration->Type))
						throw gcnew FormatException("An array can't hold functions.");

					auto arrayType = gcnew ArrayType(declaration->Type);
					arrayType->Count = counts.Pop();
					declaration->Type = arrayType;
					break;
				}

				case Lexer::RegexGroups::Brace:
				{
					if (dynamic_cast<FunctionType^>(declaration->Type) || dynamic_cast<ArrayType^>(declaration->Type))
						throw gcnew FormatException("Can't return functions or arrays.");

					int previousIndex = _lexer.Index;
					_lexer.Index = counts.Pop().Value;
					declaration->Type = gcnew FunctionType(declaration->Type, ParseParameters());
					_lexer.Index = previousIndex;
					break;
				}

				default:
					throw gcnew NotImplementedException("Internal error.");
				}
			}

			if (nextIndex == -1)
			{
				_lexer.Index = endIndex;
				break;
			}

			_lexer.Index = nextIndex;
			nextIndex = -1;
		}

		if (dynamic_cast<PointerType^>(declaration->Type) == nullptr && declaration->Type->Qualifiers == BaseType::TypeQualifiers::Restrict)
			throw gcnew FormatException("Only pointers can have 'restrict' qualifier.");

		if (dynamic_cast<FunctionType^>(declaration->Type))
		{
			if (declaration->Specifier == Declaration::StorageSpecifier::Auto)
				throw gcnew FormatException("Functions can't have 'auto' storage qualifier.");
			
			if (declaration->Specifier == Declaration::StorageSpecifier::Extern)
				declaration->Specifier = Declaration::StorageSpecifier();
		}

		if (name)
			if (auto fType = dynamic_cast<FundamentalType^>(declaration->Type); fType && fType->Type == FundamentalType::DataType::Void)
				throw gcnew FormatException("Invalid declaration.");

		return NamedDeclaration(name, declaration);
	}

	IList<NamedDeclaration>^ TypeParser::ParseParameters()
	{
		auto declarations = gcnew List<NamedDeclaration>;

		for (; ; )
		{
			switch (_lexer.Group)
			{
			case Lexer::RegexGroups::Ellipsis:
			case Lexer::RegexGroups::Name:
			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Type:
			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Qualifier:
			case Lexer::RegexGroups::Specifier:
				for (; ; )
				{
					if (_lexer.Group == Lexer::RegexGroups::Ellipsis)
					{
						if (declarations->Count == 0)
							throw gcnew FormatException("'...' can't be the first parameter.");
						
						declarations->Add(NamedDeclaration(nullptr, gcnew Declaration(gcnew EllipsisType, Declaration::StorageSpecifier())));
						_lexer.MoveNext();
						break;
					}
					
					auto initialType = ParseInitialType(false);

					if (initialType->Specifier != Declaration::StorageSpecifier::None)
						throw gcnew FormatException("Function parameters can't have storage qualifiers.");

					auto postType = ParsePostType(initialType);

					if (auto nType = dynamic_cast<NamedType^>(postType.Declaration->Type); nType && !nType->Instantiable)
						throw gcnew FormatException("Incomplete type.");

					if (auto aType = dynamic_cast<ArrayType^>(postType.Declaration->Type))
						postType.Declaration->Type = gcnew PointerType(aType->Decay);

					for each (auto d in declarations)
						if (d.Name && d.Name == postType.Name)
							throw gcnew ArgumentException("Duplicate variable '" + d.Name + "'.");

					declarations->Add(postType);

					if (_lexer.Value != ",")
						break;

					if (auto fundamentalType = dynamic_cast<FundamentalType^>(postType.Declaration->Type); fundamentalType && fundamentalType->Type == FundamentalType::DataType::Void)
						break;

					_lexer.MoveNext();
				}

			case Lexer::RegexGroups::Brace:
				if (_lexer.Value == ")")
					return declarations;

			default:
				throw gcnew FormatException("Unexpected token");
			}
		}
	}

	NamedType^ TypeParser::ParseNamedType(bool allowDefinition)
	{
		if (_lexer.Value == "struct")
			return ParseStruct(false, allowDefinition);
		
		if (_lexer.Value == "union")
			return ParseStruct(true, allowDefinition);
		
		if (_lexer.Value == "enum")
			return ParseEnum(allowDefinition);

		throw gcnew NotImplementedException;
	}

	StructType^ TypeParser::ParseStruct(bool isUnion, bool allowDefinition)
	{
		StructType^ structType{ };

		{
			String^ name{ };
			
			if (_lexer.MoveNext(); _lexer.Group == Lexer::RegexGroups::Name)
				name = _lexer.Value;

			if (name && DeclaredTypes->ContainsKey(name))
			{
				if (structType = dynamic_cast<StructType^>(DeclaredTypes[name]); structType == nullptr || structType->IsUnion != isUnion)
					throw gcnew FormatException("Attempt to redeclare type '" + name + "'.");

				if (structType->Defined)
					return structType;

				_lexer.MoveNext();
			}
			else
			{
				if (structType = gcnew StructType(name, isUnion); structType->Anonymous)
					name = "__custom_" + _lexer.TextIndex;
				else
				{
					DeclaredTypes[name] = structType;
					_lexer.MoveNext();
				}

				structType->Name = String::Join("__", Enumerable::Append(%frame, name));
			}
		}

		if (_lexer.Value != "{")
		{
			if (structType->Anonymous)
				throw gcnew FormatException("Anonymous structure has to be defined.");

			--_lexer.Index;
			return structType;
		}

		if (!allowDefinition)
			throw gcnew FormatException("Structure definition not allowed here.");

		structType->Define();

		for (bool flexibleArrayMember{ }; ; )
		{
			_lexer.MoveNext();

			switch (auto group = _lexer.Group)
			{
			case Lexer::RegexGroups::Name:
			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Type:
			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Qualifier:
			case Lexer::RegexGroups::Specifier:
			{
				auto initialType = ParseInitialType(true);

				if (initialType->Specifier != Declaration::StorageSpecifier::None)
					throw gcnew FormatException("Members can't have storage qualifiers.");

				for (; ; )
				{
					auto postType = ParsePostType(gcnew Declaration(initialType));

					if (SizeOf(postType.Declaration->Type) == 0)
					{
						if (flexibleArrayMember || dynamic_cast<ArrayType^>(postType.Declaration) == nullptr || structType->Members->Count == 0)
							throw gcnew FormatException("A type with an unspecified length is not allowed here.");

						flexibleArrayMember = true;
					}

					if (postType.Name == nullptr)
					{
						if (_lexer.Value != ";")
							throw gcnew FormatException("Variable name omitted.");
						
						if (group != Lexer::RegexGroups::SType)
							goto nextVar;

						auto tempStructType = dynamic_cast<StructType^>(postType.Declaration->Type);

						if (tempStructType == nullptr || !tempStructType->Anonymous)
							goto nextVar;

						postType.Name = "__decl_" + _lexer.TextIndex;
					}

					for each (auto member in structType->Members)
						if (member.Name && member.Name == postType.Name)
							throw gcnew ArgumentException("Duplicate variable '" + member.Name + "'.");

					structType->Members->Add(postType);

				nextVar:
					if (_lexer.Value != ",")
						break;

					_lexer.MoveNext();
				}
			}

			default:
				if (_lexer.Value == ";")
					break;

				if (_lexer.Value == "}")
				{
					structType->Define();
					return structType;
				}

				throw gcnew FormatException("Unexpected token");
			}
		}
	}

	EnumType^ TypeParser::ParseEnum(bool allowDefinition)
	{
		EnumType^ enumType{ };

		{
			String^ name{ };

			if (_lexer.MoveNext(); _lexer.Group == Lexer::RegexGroups::Name)
				name = _lexer.Value;

			if (name && DeclaredTypes->ContainsKey(name))
			{
				if (enumType = dynamic_cast<EnumType^>(DeclaredTypes[name]); enumType == nullptr)
					throw gcnew FormatException("Attempt to redeclare type '" + name + "'.");

				if (enumType->Defined)
					return enumType;

				_lexer.MoveNext();
			}
			else
			{
				if (enumType = gcnew EnumType(name); enumType->Anonymous)
					name = "enum_" + _lexer.TextIndex;
				else
				{
					name = enumType->Name;
					DeclaredTypes[name] = enumType;
					_lexer.MoveNext();
				}

				enumType->Name = String::Join("__", Enumerable::Append(% frame, name));
			}
		}

		if (_lexer.Value != "{")
			throw gcnew FormatException("Enumeration has to be defined.");

		if (!allowDefinition)
			throw gcnew FormatException("Enumeration definition not allowed here.");

		enumType->Define();

		for (; ; )
		{
			_lexer.MoveNext();

			switch (_lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				for (; ; )
				{
					if (enumType->Enumerators->Contains(_lexer.Value))
						throw gcnew ArgumentException("Duplicate enumerator '" + _lexer.Value + "'.");

					for each (auto declarations in DeclArray)
						for each (auto declaration in declarations)
							if (declaration.Name == _lexer.Value)
								throw gcnew FormatException("This name had been used before.");
					
					if (!enums.TryAdd(_lexer.Value, enumType->Enumerators->Count))
						throw gcnew ArgumentException("Enumerator already declared.");

					enumType->Enumerators->Add(_lexer.Value);
					
					if (_lexer.MoveNext(); _lexer.Value != ",")
						break;

					_lexer.MoveNext();
				}

			default:
				if (_lexer.Value == "}")
				{
					if (enumType->Enumerators->Count == 0)
						throw gcnew FormatException("'enum' can't be empty.");

					enumType->Define();
					return enumType;
				}

				throw gcnew FormatException("Unexpected token");
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