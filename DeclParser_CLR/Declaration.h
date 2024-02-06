#pragma once
#include "DataTypes.h"

namespace DeclParser
{
	public ref class Declaration
	{
	public:
		enum class StorageSpecifiers
		{
			__identifier(static) = 1, __identifier(auto),
			__identifier(register), __identifier(extern)
		};

		property BaseType^ Type;
		property StorageSpecifiers Specifier;
		property bool Inline;

		property bool HasSpecifier
		{
			bool get();
		}

		static System::String^ DeclarationToString(Declaration^ decl, System::String^ name);

		Declaration(Declaration^ decl);
		Declaration(BaseType^ type);
		Declaration(BaseType^ type, StorageSpecifiers specifier);
		Declaration(BaseType^ type, StorageSpecifiers specifier, bool isInline);

		System::String^ ToString() override;
	};
}