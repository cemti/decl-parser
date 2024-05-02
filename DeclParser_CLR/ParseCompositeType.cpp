#include "pch.h"
#include "TypeParser.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{
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

				structType->Name = String::Join("__", Enumerable::Append(% frame, name));
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
}