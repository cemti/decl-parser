#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class FundamentalType : BaseType
	{
	public:
		enum class TypeLength
		{
			None, Short, Long, LongLong
		};

		enum class TypeSign
		{
			None, Signed, Unsigned
		};

		enum class DataType
		{
			Void, Char, Int, Float, Double
		};
	private:
		const DataType _type;
		TypeSign _sign;
		TypeLength _length;
	public:
		property DataType Type
		{
			DataType get();
		}

		property TypeSign Sign
		{
			TypeSign get();
			void set(TypeSign value);			
		}

		property TypeLength Length
		{
			TypeLength get();
			void set(TypeLength value);
		}

		FundamentalType(DataType type);
		FundamentalType(DataType type, TypeSign signs, TypeLength lengths);

		int SizeOf(DataModel dataModel) override;
	};

}