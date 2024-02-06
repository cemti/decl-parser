#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class CompositeType abstract : BaseType
	{
	protected:
		CompositeType(BaseType^ decayTo);
	public:
		property BaseType^ Decay;

		virtual System::Object^ Clone() override;
	};
}