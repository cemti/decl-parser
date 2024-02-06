#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	NamedDeclaration::NamedDeclaration(String^ name, DeclParser::Declaration^ declaration)
	{
		Name = name;
		Declaration = declaration;
	}

	void NamedDeclaration::Deconstruct(String^% name, DeclParser::Declaration^% declaration)
	{
		name = Name;
		declaration = Declaration;
	}

	String^ NamedDeclaration::ToString()
	{
		return DeclParser::Declaration::DeclarationToString(Declaration, Name);
	}
}