#pragma once
#include "Lexer.h"
#include "DataTypes.h"

namespace DeclParser
{
	public ref class TypeParser
	{
		System::Collections::Generic::Dictionary<System::String^, int> enums;
		System::Collections::Generic::List<System::String^> frame;

		Lexer lexer;
		const BaseType::DataModel dataModel;
		const bool permissive; // allows ignoring other C statements

		void GreedySkipTokens();

		void Parse();
		Declaration^ ParseInitialType(bool allowDef);
		NamedDeclaration ParsePostType(Declaration^ decl);
		Declarations^ ParseParameters();
		NamedType^ ParseNamedType(bool allowDef);
		StructType^ ParseStruct(bool isUnion, bool allowDef);
		EnumType^ ParseEnum(bool allowDef);

		Declaration^ LookupTypedef(System::String^ name);
	public:
		property DeclaredNamedTypes^ DeclaredNamedTypes;

		property array<Declarations^>^ DeclArray;

		property Declarations^ Variables
		{
			Declarations^ get();
		}

		int SizeOf(BaseType^);

		TypeParser(BaseType::DataModel dm) : dataModel(dm), permissive() { }
		TypeParser(System::String^ str, BaseType::DataModel dm) : TypeParser(str, dm, false) { }
		TypeParser(System::String^, BaseType::DataModel, bool);
	};
}