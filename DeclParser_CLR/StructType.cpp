#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	bool StructType::IsUnion::get()
	{
		return isUnion;
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
	StructType::StructType(String^ name, bool isUnion, bool complete) : NamedType(name, complete), isUnion(isUnion) { Members = gcnew Declarations; }

	int StructType::SizeOf(DataModel dm)
	{
		int size = 0;

		if (IsUnion)
		{
			for each (auto p in Members)
				if (auto temp = p.Decl->Type->SizeOf(dm); temp > 0)
					size = Math::Max(temp, size);
		}
		else
		{
			for each (auto p in Members)
				size += p.Decl->Type->SizeOf(dm);
		}

		return size == 0 ? 1 : size;
	}
}