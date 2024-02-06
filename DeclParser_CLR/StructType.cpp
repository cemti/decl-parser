#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Collections::Generic;

namespace DeclParser
{
	bool StructType::IsUnion::get()
	{
		return _isUnion;
	}

	bool StructType::Empty::get()
	{
		return Members->Count == 0;
	}

	String^ StructType::Keyword::get()
	{
		return IsUnion ? "union" : "struct";
	}
	
	StructType::StructType() : StructType(nullptr) { }
	StructType::StructType(String^ name) : StructType(name, false) { }
	StructType::StructType(String^ name, bool isUnion) : StructType(name, isUnion, false) { }

	StructType::StructType(String^ name, bool isUnion, bool complete) : NamedType(name, complete), _isUnion(isUnion)
	{
		Members = gcnew List<NamedDeclaration>;
	}

	int StructType::SizeOf(DataModel dataModel)
	{
		int size = 0;

		if (IsUnion)
		{
			for each (auto p in Members)
				if (auto sizeOf = p.Declaration->Type->SizeOf(dataModel); sizeOf > 0)
					size = Math::Max(sizeOf, size);
		}
		else
		{
			for each (auto p in Members)
				size += p.Declaration->Type->SizeOf(dataModel);
		}

		return size == 0 ? 1 : size;
	}
}