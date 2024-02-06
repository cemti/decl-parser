#pragma once
#include "NamedType.h"

namespace DeclParser
{
	public ref class StructType : NamedType
	{
		const bool _isUnion;
	public:
		property System::Collections::Generic::IList<NamedDeclaration>^ Members;

		property bool IsUnion
		{
			bool get();
		}

		property bool Empty
		{
			virtual bool get() override;
		}

		property System::String^ Keyword
		{
			System::String^ get() override;
		}

		StructType();
		StructType(System::String^ name);
		StructType(System::String^ name, bool isUnion);
		StructType(System::String^ name, bool isUnion, bool complete);

		int SizeOf(DataModel dataModel) override;
	};
}