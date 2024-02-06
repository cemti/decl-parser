#pragma once
#include "NamedType.h"

namespace DeclParser
{
	public ref class FunctionType : CompositeType
	{
	public:
		property System::Collections::Generic::IList<NamedDeclaration>^ Parameters;

		property bool HasParameters
		{
			bool get();
		}

		FunctionType();
		FunctionType(BaseType^ decayTo, System::Collections::Generic::IList<NamedDeclaration>^ parameters);

		void SetQualifier(TypeQualifiers qualifiers) override;
	};
}