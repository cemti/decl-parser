#include "DataTypes.h"

namespace DeclParser
{
	public ref class ArrayType : CompositeType
	{
	public:
		property System::Nullable<int> Count;

		ArrayType();
		ArrayType(BaseType^ decaysTo);

		void SetQualifier(Qualifiers q) override;
		int SizeOf(DataModel dm) override;
	};
}