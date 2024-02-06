#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	ArrayType::ArrayType() : ArrayType(nullptr) { }

	ArrayType::ArrayType(BaseType^ decayTo) : CompositeType(decayTo) { }

	void ArrayType::SetQualifier(TypeQualifiers qualifiers)
	{
		Decay->SetQualifier(qualifiers);
	}

	int ArrayType::SizeOf(DataModel dataModel)
	{
		return Count.HasValue && Decay ? Count.Value * Decay->SizeOf(dataModel) : 0;
	}
}