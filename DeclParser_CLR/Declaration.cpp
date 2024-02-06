#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{
    bool Declaration::HasSpecifier::get()
    {
        return bool(Specifier);
    }
    
	String^ Declaration::DeclarationToString(Declaration^ declaration, String^ name)
	{
		auto en = Enumerable::Repeat(BaseType::TypeToString(declaration->Type, name, false), 1);

		if (declaration->Inline)
			Enumerable::Prepend<String^>(en, "inline");

		if (declaration->HasSpecifier)
			Enumerable::Prepend(en, declaration->Specifier.ToString());

		return String::Join(L' ', en);
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