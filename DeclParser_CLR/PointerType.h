#include "DataTypes.h"

namespace DeclParser
{
	public ref class PointerType : CompositeType
	{
	public:
		PointerType() : PointerType(nullptr) { }
		PointerType(BaseType^ decayTo) : CompositeType(decayTo) { }

		int SizeOf(DataModel dataModel) override;
	};
}