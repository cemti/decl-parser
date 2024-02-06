#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	int PointerType::SizeOf(DataModel dataModel)
	{
		return dataModel == DataModel::LLP64 || dataModel == DataModel::LP64 ? 8 : 4;
	}
}