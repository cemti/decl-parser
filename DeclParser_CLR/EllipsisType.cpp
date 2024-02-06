#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	void EllipsisType::SetQualifier(Qualifiers qualifiers)
	{
		if (qualifiers != Qualifiers::None)
			throw gcnew ArgumentException("Ellipsis doesn't accept any qualifier.");
	}
}