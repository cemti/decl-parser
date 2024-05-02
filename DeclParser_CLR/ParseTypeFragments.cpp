#include "pch.h"
#include "TypeParser.h"
using namespace System;
using namespace Collections::Generic;

namespace DeclParser
{
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
}