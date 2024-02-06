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
    
	String^ Declaration::DeclarationToString(Declaration^ decl, String^ name)
	{
		auto en = Enumerable::Repeat(BaseType::TypeToString(decl->Type, name, false), 1);

		if (decl->Inline)
			Enumerable::Prepend<String^>(en, "inline");

		if (decl->HasSpecifier)
			Enumerable::Prepend(en, decl->Specifier.ToString());

		return String::Join(L' ', en);
	}

	Declaration::Declaration(Declaration^ decl) : Declaration(decl->Type, decl->Specifier, decl->Inline) { }

	Declaration::Declaration(BaseType^ type) : Declaration(type, StorageSpecifiers(), false) { }

	Declaration::Declaration(BaseType^ type, StorageSpecifiers specifier) : Declaration(type, specifier, false) { }

	Declaration::Declaration(BaseType^ type, StorageSpecifiers specifier, bool isInline)
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