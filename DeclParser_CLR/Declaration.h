#pragma once
#include "DataTypes.h"

namespace DeclParser
{
	public ref class Declaration
	{
	public:
		enum class StorageSpecifier
		{
			None, Static, Auto, Register, Extern
		};

		property BaseType^ Type;
		property StorageSpecifier Specifier;
		property bool Inline;

		static System::String^ DeclarationToString(Declaration^ declaration, System::String^ name);

		Declaration(Declaration^ declaration);
		Declaration(BaseType^ type);
		Declaration(BaseType^ type, StorageSpecifier specifier);
		Declaration(BaseType^ type, StorageSpecifier specifier, bool isInline);

		System::String^ ToString() override;
	};
}