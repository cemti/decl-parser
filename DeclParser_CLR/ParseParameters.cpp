#include "pch.h"
#include "TypeParser.h"
using namespace System;
using namespace Collections::Generic;

namespace DeclParser
{
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
}