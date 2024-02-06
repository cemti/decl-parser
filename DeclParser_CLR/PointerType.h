#include "DataTypes.h"

namespace DeclParser
{
	public ref class PointerType : CompositeType
	{
	public:
		PointerType() : PointerType(nullptr) { }
		PointerType(BaseType^ decaysTo) : CompositeType(decaysTo) { }

		int SizeOf(DataModel dm) override;
	};
}