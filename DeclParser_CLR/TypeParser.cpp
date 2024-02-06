#include "pch.h"
#include "TypeParser.h"
using namespace System;
using namespace Collections::Generic;
using namespace Linq;

namespace DeclParser
{
	Declarations^ TypeParser::Variables::get()
	{
		return DeclArray[0];
	}
	
	TypeParser::TypeParser(String^ str, BaseType::DataModel dm, bool permissive) : lexer(str), permissive(permissive), dataModel(dm)
	{
		FundamentalType::Lengths length{ };

		if (dm >= BaseType::DataModel::LLP64)
			length = FundamentalType::Lengths::LongLong;
		
		DeclArray = gcnew array<Declarations^> { gcnew Declarations, gcnew Declarations };
		DeclaredNamedTypes = gcnew DeclParser::DeclaredNamedTypes;
		
		try
		{
			Parse();
		}
		catch (Exception^ ex)
		{
			if (!lexer.HasNext && (ex->GetType() == FormatException::typeid || ex->GetType() == ArgumentException::typeid))
			{
				auto match = lexer.CurrentMatch;
				throw gcnew FormatException("At position " + match->Index + L" (length " + match->Length + "): " + ex->Message);
			}

			throw;
		}
	}

	void TypeParser::GreedySkipTokens()
	{
		for (; ; )
		{
			lexer.MoveNext();

			if (lexer.Value == ":")
				return;

			if (lexer.Value == ";" || lexer.Value == "{" || lexer.Value == "}" || lexer.Value == ")")
			{
				--lexer.Index;
				return;
			}
		}
	}

	int TypeParser::SizeOf(BaseType^ type)
	{
		return type->SizeOf(dataModel);
	}

	void TypeParser::Parse()
	{
		int declIndex{ };
		bool insideLoop{ }, insideFor{ };

		for (; ; )
		{
			if (lexer.TryMoveNext(); lexer.HasNext)
			{
				if (frame.Count == 0)
					return;

				throw gcnew FormatException("Unexpected end of file while parsing the body of scope '" + frame[frame.Count - 1] + "'.");
			}

			switch (auto group = lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				if (permissive && LookupTypedef(lexer.Value) == nullptr)
				{
					bool isDo = lexer.Value == "do";
					insideFor = lexer.Value == "for";
					
					if (isDo || insideFor || lexer.Value == "while" || lexer.Value == "switch")
					{
						insideLoop = true;
						frame.Add(String::Format("{0}_{1}", lexer.Value, lexer.TextIndex));

						if (!isDo)
							lexer.MoveNext();
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
			decl:
				for (auto initial = ParseInitialType(true); ; )
				{
					auto initDecl = gcnew Declaration(initial);
					auto post = ParsePostType(initDecl);
					FunctionType^ fType{ };
					bool fnDef = false;

					switch (declIndex)
					{
					case 0:
						if (fType = dynamic_cast<FunctionType^>(post.Decl->Type))
						{
							if (fType->HasNoParams)
								break;

							for each (auto d in fType->Parameters)
								if (d.Name == nullptr && dynamic_cast<EllipsisType^>(d.Decl->Type) == nullptr)
									throw gcnew FormatException("Parameter name omitted.");
						}
						else
						{
							if (post.Decl->Inline)
								throw gcnew FormatException("Only functions can be inline.");

							if (auto nType = dynamic_cast<NamedType^>(post.Decl->Type); nType && (!nType->Instantiable || nType->Empty))
								throw gcnew FormatException("Incomplete type.");
						}
						
						break;

					case 1:
						if (post.Decl->HasSpecifier || post.Decl->Inline)
							throw gcnew FormatException("Storage qualifiers are not allowed in a typedef declaration.");
					}

					if (post.Name == nullptr)
					{
						if (lexer.Value != ";")
							throw gcnew FormatException("Variable name omitted.");

						if (group != Lexer::RegexGroups::SType)
							goto nextVar;

						if (auto sType = dynamic_cast<StructType^>(post.Decl->Type); sType == nullptr || !sType->Anonymous)
							goto nextVar;

						post.Name = "__decl_" + lexer.TextIndex;
					}

					fnDef = declIndex == 0 && fType && lexer.Value == "{";
					post.Name = String::Join("__", Enumerable::Append(%frame, post.Name));

					for each (auto d in DeclArray[declIndex])
						if (d.Name && d.Name == post.Name)
						{
							if (fnDef && d.Decl->Type->ToString() == post.Decl->Type->ToString())
							{
								post = d;
								frame.Add(post.Name + "_" + lexer.TextIndex);
								break;
							}

							throw gcnew ArgumentException("Duplicate variable '" + d.Name + "'.");
						}

					DeclArray[declIndex]->Add(post);

				nextVar:
					if (lexer.Value != ",")
					{
						if (fnDef)
							frame.Add(post.Name + "_" + lexer.TextIndex);
						else
						{
							if (lexer.Value != ";")
								goto unexpectedToken;

							declIndex = 0;
						}

						break;
					}

					lexer.MoveNext();
				}

				break;

			default:
			unexpectedToken:
				if (!permissive)
					throw gcnew FormatException("Unexpected token '" + lexer.Value + "'.");

				if (insideLoop && lexer.Value == ")")
				{
					insideFor = false;
					lexer.MoveNext();

					if (lexer.Value == ";")
					{
						insideLoop = false;
						frame.RemoveAt(frame.Count - 1);
					}
					else
						--lexer.Index;
				}
					
				GreedySkipTokens();
				break;

			case Lexer::RegexGroups::ScopeBrace:
				if (frame.Count == 0)
					throw gcnew FormatException("Scope braces are not expected in root scope.");

				if (lexer.Value == "{")
				{
					if (insideLoop && !insideFor)
						insideLoop = false;
					else
						frame.Add("scope_" + lexer.TextIndex);
				}
				else if (lexer.Value == "}")
					frame.RemoveAt(frame.Count - 1);

				break;

			case Lexer::RegexGroups::Special:
				if (lexer.Value != ";")
				{
					if (lexer.Value != "typedef")
					{
						if (lexer.Value != "inline")
							goto unexpectedToken;
						
						goto decl;
					}

					declIndex = 1;
				}
				else if (permissive && !insideFor && insideLoop && lexer.Value == ";")
				{
					insideLoop = false;
					frame.RemoveAt(frame.Count - 1);
				}
			}
		}
	}

	Declaration^ TypeParser::ParseInitialType(bool allowDef)
	{
		FundamentalType::Lengths lengths{ };
		FundamentalType::Signs signs{ };
		BaseType::Qualifiers qualifiers{ };
		Declaration::StorageSpecifiers specifiers{ };
		BaseType^ type{ };
		bool sealedType{ }, isInline{ };

		for (; ; lexer.MoveNext())
		{
			switch (lexer.Group)
			{
			case Lexer::RegexGroups::SType:
				if (type)
					throw gcnew FormatException("Type already specified.");

				type = ParseNamedType(allowDef);
				sealedType = true;
				break;

			case Lexer::RegexGroups::Type:
				if (type)
					throw gcnew FormatException("Type already specified.");

				type = gcnew FundamentalType(Enum::Parse<FundamentalType::Types>(lexer.Value));
				break;

			case Lexer::RegexGroups::Length:
			{
				auto flag = Enum::Parse<FundamentalType::Lengths>(lexer.Value);

				if (bool(lengths))
				{
					if (flag != FundamentalType::Lengths::__identifier(long) || flag != lengths)
						throw gcnew FormatException("More than sufficient length specifiers.");

					lengths = FundamentalType::Lengths::LongLong;
				}
				else
					lengths = flag;

				break;
			}

			case Lexer::RegexGroups::Sign:
				if (bool(signs))
					throw gcnew FormatException("Duplicate sign specifier.");

				signs = Enum::Parse<FundamentalType::Signs>(lexer.Value);
				break;

			case Lexer::RegexGroups::Qualifier:
				qualifiers = qualifiers | Enum::Parse<BaseType::Qualifiers>(lexer.Value);
				break;

			case Lexer::RegexGroups::Specifier:
			{
				auto flag = Enum::Parse<Declaration::StorageSpecifiers>(lexer.Value);

				if (flag == specifiers)
					throw gcnew FormatException("Duplicate '" + flag.ToString() + "'.");

				specifiers = flag;
				break;
			}

			case Lexer::RegexGroups::Name:
				if (auto tType = LookupTypedef(lexer.Value); tType && type == nullptr)
				{
					type = tType->Type;
					sealedType = true;
					break;
				}

			default:
				if (lexer.Value == "inline")
				{
					isInline = true;
					break;
				}

				if (auto hasLS = bool(lengths) || bool(signs); type == nullptr)
				{
					if (!hasLS)
						throw gcnew FormatException("No type specified.");

					type = gcnew FundamentalType(FundamentalType::Types::__identifier(int), signs, lengths);
				}
				else if (auto fType = dynamic_cast<FundamentalType^>(type); fType && !sealedType)
				{
					fType->Length = lengths;
					fType->Sign = signs;
				}
				else if (hasLS)
					throw gcnew FormatException("Length and sign qualifiers can be applied to fundamental types only.");

				if (bool(qualifiers))
				{
					if (sealedType)
						type = static_cast<BaseType^>(type->Clone());

					type->SetQualifier(qualifiers);
				}

				return gcnew Declaration(type, specifiers, isInline);
			}
		}
	}

	NamedDeclaration TypeParser::ParsePostType(Declaration^ decl)
	{
		Stack<Lexer::RegexGroups> groups;
		Stack<Nullable<int>> counts;
		String^ name{ };
		
		for (int endIdx = -1, nextIdx = -1; ; lexer.MoveNext())
		{
			switch (lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				if (name || groups.Count > 0)
					throw gcnew FormatException("Invalid declaration.");

				name = lexer.Value;
				continue;

			case Lexer::RegexGroups::Qualifier:
				if (name || groups.Count > 0)
					throw gcnew FormatException("Invalid declaration.");

				decl->Type->SetQualifier(Enum::Parse<FundamentalType::Qualifiers>(lexer.Value));
				continue;

			case Lexer::RegexGroups::ArrayBrace:
			{
				if (lexer.Value != "[")
					throw gcnew FormatException("'[' expected.");

				auto group = lexer.Group;

				if (lexer.MoveNext(); lexer.Group == Lexer::RegexGroups::Integer)
				{
					counts.Push(int::Parse(lexer.Value));
					lexer.MoveNext();
				}
				else
					counts.Push({ });

				groups.Push(group);

				if (lexer.Value != "]")
					throw gcnew FormatException("']' expected.");

				continue;
			}

			case Lexer::RegexGroups::Brace:
				if (lexer.Value == ")")
				{
					if (endIdx == -1)
						endIdx = lexer.Index;
					
					break;
				}

				lexer.MoveNext();

				if (lexer.Group < Lexer::RegexGroups::Brace || lexer.Value == ")" ||
					(lexer.Group == Lexer::RegexGroups::Name && LookupTypedef(lexer.Value) != nullptr))
				{
					counts.Push(lexer.Index--);
					groups.Push(lexer.Group);
				}
				else
				{
					if (name || nextIdx != -1)
						throw gcnew FormatException("Invalid declaration");

					nextIdx = --lexer.Index;
				}

				lexer.Skip();
				continue;

			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Type:
				throw gcnew FormatException("Type not expected here.");

			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Specifier:
				throw gcnew FormatException("Type length/sign/storage qualifiers not expected here.");

			case Lexer::RegexGroups::Special:
				if (lexer.Value == "*")
				{
					if (name || groups.Count > 0)
						throw gcnew FormatException("Invalid declaration.");

					decl->Type = gcnew PointerType(decl->Type);
					continue;
				}

			default:
				if (endIdx != -1)
					throw gcnew FormatException("Invalid declaration");
			
				endIdx = lexer.Index;
			}

			while (groups.Count > 0)
			{
				switch (groups.Pop())
				{
				case Lexer::RegexGroups::ArrayBrace:
				{
					if (dynamic_cast<FunctionType^>(decl->Type))
						throw gcnew FormatException("An array can't hold functions.");

					auto arr = gcnew ArrayType(decl->Type);
					arr->Count = counts.Pop();
					decl->Type = arr;
					break;
				}

				case Lexer::RegexGroups::Brace:
				{
					if (dynamic_cast<FunctionType^>(decl->Type) || dynamic_cast<ArrayType^>(decl->Type))
						throw gcnew FormatException("Can't return functions or arrays.");

					int bakIdx = lexer.Index;
					lexer.Index = counts.Pop().Value;
					decl->Type = gcnew FunctionType(decl->Type, ParseParameters());
					lexer.Index = bakIdx;
					break;
				}

				default:
					throw gcnew NotImplementedException("Internal error.");
				}
			}

			if (nextIdx == -1)
			{
				lexer.Index = endIdx;
				break;
			}

			lexer.Index = nextIdx;
			nextIdx = -1;
		}

		if (dynamic_cast<PointerType^>(decl->Type) == nullptr && decl->Type->Qualifier == BaseType::Qualifiers::__identifier(restrict))
			throw gcnew FormatException("Only pointers can have 'restrict' qualifier.");

		if (dynamic_cast<FunctionType^>(decl->Type))
		{
			if (decl->Specifier == Declaration::StorageSpecifiers::__identifier(auto))
				throw gcnew FormatException("Functions can't have 'auto' storage qualifier.");
			
			if (decl->Specifier == Declaration::StorageSpecifiers::__identifier(extern))
				decl->Specifier = Declaration::StorageSpecifiers();
		}

		if (name)
			if (auto fType = dynamic_cast<FundamentalType^>(decl->Type); fType && fType->Type == FundamentalType::Types::__identifier(void))
				throw gcnew FormatException("Invalid declaration.");

		return NamedDeclaration(name, decl);
	}

	Declarations^ TypeParser::ParseParameters()
	{
		auto decls = gcnew Declarations;

		for (; ; )
		{
			switch (lexer.Group)
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
					if (lexer.Group == Lexer::RegexGroups::Ellipsis)
					{
						if (decls->Count == 0)
							throw gcnew FormatException("'...' can't be the first parameter.");
						
						decls->Add(NamedDeclaration(nullptr, gcnew Declaration(gcnew EllipsisType, Declaration::StorageSpecifiers())));
						lexer.MoveNext();
						break;
					}
					
					auto initial = ParseInitialType(false);

					if (bool(initial->Specifier))
						throw gcnew FormatException("Function parameters can't have storage qualifiers.");

					auto post = ParsePostType(initial);

					if (auto nType = dynamic_cast<NamedType^>(post.Decl->Type); nType && !nType->Instantiable)
						throw gcnew FormatException("Incomplete type.");

					if (auto aType = dynamic_cast<ArrayType^>(post.Decl->Type))
						post.Decl->Type = gcnew PointerType(aType->Decay);

					for each (auto d in decls)
						if (d.Name && d.Name == post.Name)
							throw gcnew ArgumentException("Duplicate variable '" + d.Name + "'.");

					decls->Add(post);

					if (lexer.Value != ",")
						break;

					if (auto fType = dynamic_cast<FundamentalType^>(post.Decl->Type); fType && fType->Type == FundamentalType::Types::__identifier(void))
						break;

					lexer.MoveNext();
				}

			case Lexer::RegexGroups::Brace:
				if (lexer.Value == ")")
					return decls;

			default:
				throw gcnew FormatException("Unexpected token");
			}
		}
	}

	NamedType^ TypeParser::ParseNamedType(bool allowDef)
	{
		if (lexer.Value == "struct")
			return ParseStruct(false, allowDef);
		
		if (lexer.Value == "union")
			return ParseStruct(true, allowDef);
		
		if (lexer.Value == "enum")
			return ParseEnum(allowDef);

		throw gcnew NotImplementedException;
	}

	StructType^ TypeParser::ParseStruct(bool isUnion, bool allowDef)
	{
		StructType^ st{ };

		{
			String^ name{ };
			
			if (lexer.MoveNext(); lexer.Group == Lexer::RegexGroups::Name)
				name = lexer.Value;

			if (name && DeclaredNamedTypes->ContainsKey(name))
			{
				if (st = dynamic_cast<StructType^>(DeclaredNamedTypes[name]); st == nullptr || st->IsUnion != isUnion)
					throw gcnew FormatException("Attempt to redeclare type '" + name + "'.");

				if (st->Defined)
					return st;

				lexer.MoveNext();
			}
			else
			{
				if (st = gcnew StructType(name, isUnion); st->Anonymous)
					name = "__custom_" + lexer.TextIndex;
				else
				{
					DeclaredNamedTypes[name] = st;
					lexer.MoveNext();
				}

				st->Name = String::Join("__", Enumerable::Append(%frame, name));
			}
		}

		if (lexer.Value != "{")
		{
			if (st->Anonymous)
				throw gcnew FormatException("Anonymous structure has to be defined.");

			--lexer.Index;
			return st;
		}

		if (!allowDef)
			throw gcnew FormatException("Structure definition not allowed here.");

		st->Define();

		for (bool fam{ }; ; )
		{
			lexer.MoveNext();

			switch (auto group = lexer.Group)
			{
			case Lexer::RegexGroups::Name:
			case Lexer::RegexGroups::Sign:
			case Lexer::RegexGroups::Length:
			case Lexer::RegexGroups::Type:
			case Lexer::RegexGroups::SType:
			case Lexer::RegexGroups::Qualifier:
			case Lexer::RegexGroups::Specifier:
			{
				auto initial = ParseInitialType(true);

				if (bool(initial->Specifier))
					throw gcnew FormatException("Members can't have storage qualifiers.");

				for (; ; )
				{
					auto post = ParsePostType(gcnew Declaration(initial));

					if (SizeOf(post.Decl->Type) == 0)
					{
						if (fam || dynamic_cast<ArrayType^>(post.Decl) == nullptr || st->Members->Count == 0)
							throw gcnew FormatException("A type with an unspecified length is not allowed here.");

						fam = true;
					}

					if (post.Name == nullptr)
					{
						if (lexer.Value != ";")
							throw gcnew FormatException("Variable name omitted.");
						
						if (group != Lexer::RegexGroups::SType)
							goto nextVar;

						auto sType = dynamic_cast<StructType^>(post.Decl->Type);

						if (sType == nullptr || !sType->Anonymous)
							goto nextVar;

						post.Name = "__decl_" + lexer.TextIndex;
					}

					for each (auto d in st->Members)
						if (d.Name && d.Name == post.Name)
							throw gcnew ArgumentException("Duplicate variable '" + d.Name + "'.");

					st->Members->Add(post);

				nextVar:
					if (lexer.Value != ",")
						break;

					lexer.MoveNext();
				}
			}

			default:
				if (lexer.Value == ";")
					break;

				if (lexer.Value == "}")
				{
					st->Define();
					return st;
				}

				throw gcnew FormatException("Unexpected token");
			}
		}
	}

	EnumType^ TypeParser::ParseEnum(bool allowDef)
	{
		EnumType^ st{ };

		{
			String^ name{ };

			if (lexer.MoveNext(); lexer.Group == Lexer::RegexGroups::Name)
				name = lexer.Value;

			if (name && DeclaredNamedTypes->ContainsKey(name))
			{
				if (st = dynamic_cast<EnumType^>(DeclaredNamedTypes[name]); st == nullptr)
					throw gcnew FormatException("Attempt to redeclare type '" + name + "'.");

				if (st->Defined)
					return st;

				lexer.MoveNext();
			}
			else
			{
				if (st = gcnew EnumType(name); st->Anonymous)
					name = "enum_" + lexer.TextIndex;
				else
				{
					name = st->Name;
					DeclaredNamedTypes[name] = st;
					lexer.MoveNext();
				}

				st->Name = String::Join("__", Enumerable::Append(% frame, name));
			}
		}

		if (lexer.Value != "{")
			throw gcnew FormatException("Enumeration has to be defined.");

		if (!allowDef)
			throw gcnew FormatException("Enumeration definition not allowed here.");

		st->Define();

		for (; ; )
		{
			lexer.MoveNext();

			switch (lexer.Group)
			{
			case Lexer::RegexGroups::Name:
				for (; ; )
				{
					if (st->Enumerators->Contains(lexer.Value))
						throw gcnew ArgumentException("Duplicate enumerator '" + lexer.Value + "'.");

					for each (auto decls in DeclArray)
						for each (auto decl in decls)
							if (decl.Name == lexer.Value)
								throw gcnew FormatException("This name had been used before.");
					
					if (!enums.TryAdd(lexer.Value, st->Enumerators->Count))
						throw gcnew ArgumentException("Enumerator already declared.");

					st->Enumerators->Add(lexer.Value);
					
					if (lexer.MoveNext(); lexer.Value != ",")
						break;

					lexer.MoveNext();
				}

			default:
				if (lexer.Value == "}")
				{
					if (st->Enumerators->Count == 0)
						throw gcnew FormatException("'enum' can't be empty.");

					st->Define();
					return st;
				}

				throw gcnew FormatException("Unexpected token");
			}
		}
	}

	Declaration^ TypeParser::LookupTypedef(String^ name)
	{
		for each (auto x in DeclArray[1])
			if (x.Name == name)
				return x.Decl;

		return nullptr;
	}
}