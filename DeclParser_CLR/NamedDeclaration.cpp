#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	NamedDeclaration::NamedDeclaration(String^ name, Declaration^ decl)
	{
		Name = name;
		Decl = decl;
	}

	void NamedDeclaration::Deconstruct(String^% name, Declaration^% decl)
	{
		name = Name;
		decl = Decl;
	}

	String^ NamedDeclaration::ToString()
	{
		return Declaration::DeclarationToString(Decl, Name);
	}
}