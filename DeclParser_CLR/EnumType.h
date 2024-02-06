#pragma once
#include "NamedType.h"

namespace DeclParser
{
	public ref class EnumType : NamedType
	{
	public:
		property System::Collections::Generic::IList<System::String^>^ Enumerators;

		property bool Empty
		{
			virtual bool get() override;
		}

		property System::String^ Keyword
		{
			System::String^ get() override;
		}

		EnumType();
		EnumType(System::String^ name);
		EnumType(System::String^ name, bool complete);

		int SizeOf(DataModel dataModel) override;
	};
}