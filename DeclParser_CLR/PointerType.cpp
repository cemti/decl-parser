#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	int PointerType::SizeOf(DataModel dm)
	{
		return dm == DataModel::LLP64 || dm == DataModel::LP64 ? 8 : 4;
	}
}