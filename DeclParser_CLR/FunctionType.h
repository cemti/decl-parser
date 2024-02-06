#pragma once
#include "NamedType.h"

namespace DeclParser
{
	public ref class FunctionType : CompositeType
	{
	public:
		property Declarations^ Parameters;

		property bool HasNoParams
		{
			bool get();
		}

		FunctionType();
		FunctionType(BaseType^ decaysTo, Declarations^ params);

		void SetQualifier(Qualifiers q) override;
	};
}