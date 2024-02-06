#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	ArrayType::ArrayType() : ArrayType(nullptr) { }

	ArrayType::ArrayType(BaseType^ decaysTo) : CompositeType(decaysTo) { }

	void ArrayType::SetQualifier(Qualifiers q)
	{
		Decay->SetQualifier(q);
	}

	int ArrayType::SizeOf(DataModel dm)
	{
		return Count.HasValue && Decay ? Count.Value * Decay->SizeOf(dm) : 0;
	}
}