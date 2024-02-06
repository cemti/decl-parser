#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
    CompositeType::CompositeType(BaseType^ decaysTo)
	{
		Decay = decaysTo;
	}

	Object^ CompositeType::Clone()
	{
		auto clone = static_cast<CompositeType^>(BaseType::Clone());
		clone->Decay = static_cast<BaseType^>(clone->Decay->Clone());
		return clone;
	}
}