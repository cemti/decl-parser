#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	void EllipsisType::SetQualifier(TypeQualifiers qualifiers)
	{
		if (qualifiers != TypeQualifiers::None)
			throw gcnew ArgumentException("Ellipsis doesn't accept any qualifier.");
	}
}