#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	void EllipsisType::SetQualifier(Qualifiers q)
	{
		if (bool(q))
			throw gcnew ArgumentException("Ellipsis doesn't accept any qualifier.");
	}
}