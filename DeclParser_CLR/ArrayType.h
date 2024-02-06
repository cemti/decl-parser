#include "DataTypes.h"

namespace DeclParser
{
	public ref class ArrayType : CompositeType
	{
	public:
		property System::Nullable<int> Count;

		ArrayType();
		ArrayType(BaseType^ decayTo);

		void SetQualifier(Qualifiers qualifiers) override;
		int SizeOf(DataModel dataModel) override;
	};
}