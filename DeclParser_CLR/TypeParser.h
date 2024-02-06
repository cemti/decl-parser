#pragma once
#include "Lexer.h"
#include "DataTypes.h"

namespace DeclParser
{
	public ref class TypeParser
	{
		System::Collections::Generic::Dictionary<System::String^, int> enums;
		System::Collections::Generic::List<System::String^> frame;

		Lexer _lexer;
		const BaseType::DataModel _dataModel;
		const bool _permissive; // allows ignoring other C statements

		void GreedySkipTokens();

		void Parse();
		Declaration^ ParseInitialType(bool allowDefinition);
		NamedDeclaration ParsePostType(Declaration^ declaration);
		System::Collections::Generic::IList<NamedDeclaration>^ ParseParameters();
		NamedType^ ParseNamedType(bool allowDefinition);
		StructType^ ParseStruct(bool isUnion, bool allowDefinition);
		EnumType^ ParseEnum(bool allowDefinition);

		Declaration^ LookupTypedef(System::String^ name);
	public:
		property System::Collections::Generic::IDictionary<System::String^, NamedType^>^ DeclaredTypes;

		property array<System::Collections::Generic::IList<NamedDeclaration>^>^ DeclArray;

		property System::Collections::Generic::IList<NamedDeclaration>^ Variables
		{
			System::Collections::Generic::IList<NamedDeclaration>^ get();
		}

		int SizeOf(BaseType^);

		TypeParser(BaseType::DataModel dataModel) : _dataModel(dataModel), _permissive() { }
		TypeParser(System::String^ input, BaseType::DataModel dataModel) : TypeParser(input, dataModel, false) { }
		TypeParser(System::String^, BaseType::DataModel, bool);
	};
}