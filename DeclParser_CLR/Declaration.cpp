#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{
	String^ Declaration::DeclarationToString(Declaration^ declaration, String^ name)
	{
		auto chain = Enumerable::Repeat(BaseType::TypeToString(declaration->Type, name, false), 1);

		if (declaration->Inline)
			chain = Enumerable::Prepend<String^>(chain, "inline");

		if (declaration->Specifier != StorageSpecifier::None)
			chain = Enumerable::Prepend(chain, declaration->Specifier.ToString()->ToLower());

		return String::Join(L' ', chain);
	}

	Declaration::Declaration(Declaration^ declaration) : Declaration(declaration->Type, declaration->Specifier, declaration->Inline) { }

	Declaration::Declaration(BaseType^ type) : Declaration(type, StorageSpecifier(), false) { }

	Declaration::Declaration(BaseType^ type, StorageSpecifier specifier) : Declaration(type, specifier, false) { }

	Declaration::Declaration(BaseType^ type, StorageSpecifier specifier, bool isInline)
	{
		Type = type;
		Specifier = specifier;
		Inline = isInline;
	}

	String^ Declaration::ToString()
	{
		return DeclarationToString(this, nullptr);
	}
}