#pragma once
#include "NamedType.h"

namespace DeclParser
{
	public ref class EnumType : NamedType
	{
	public:
		using Enums = System::Collections::Generic::List<System::String^>;
		property Enums^ Enumerators;

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

		int SizeOf(DataModel) override;
	};
}