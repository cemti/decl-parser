#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class CompositeType abstract : BaseType
	{
	protected:
		CompositeType(BaseType^ decaysTo);
	public:
		property BaseType^ Decay;

		virtual System::Object^ Clone() override;
	};
}