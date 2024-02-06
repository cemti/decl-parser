#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	bool NamedType::Anonymous::get()
	{
		return _anonymous;
	}

	bool NamedType::Defined::get()
	{
		return _stage > 0;
	}

	bool NamedType::Instantiable::get()
	{
		return _stage == 2;
	}
	
	NamedType::NamedType(String^ name, bool instantiable) : _anonymous(String::IsNullOrWhiteSpace(name)), _stage(instantiable ? 2 : 0)
	{
		Name = name;
	}

	void NamedType::Define()
	{
		if (_stage != 2)
			++_stage;
	}
}