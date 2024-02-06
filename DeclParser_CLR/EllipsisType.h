#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class EllipsisType : BaseType
	{
	public:
		void SetQualifier(Qualifiers q) override;
	};
}