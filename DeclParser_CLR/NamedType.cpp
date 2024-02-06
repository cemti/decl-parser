#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	bool NamedType::Anonymous::get()
	{
		return anon;
	}

	bool NamedType::Defined::get()
	{
		return stage > 0;
	}

	bool NamedType::Instantiable::get()
	{
		return stage == 2;
	}
	
	NamedType::NamedType(String^ name, bool instantiable) : anon(String::IsNullOrWhiteSpace(name)), stage(instantiable ? 2 : 0)
	{
		Name = name;
	}

	void NamedType::Define()
	{
		if (stage != 2)
			++stage;
	}
}