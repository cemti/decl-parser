#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class FundamentalType : BaseType
	{
	public:
		enum class TypeLength
		{
			__identifier(short) = 1, __identifier(long), LongLong
		};

		enum class TypeSign
		{
			__identifier(signed) = 1, __identifier(unsigned)
		};

		enum class DataType
		{
			__identifier(void), __identifier(char),
			__identifier(int), __identifier(float),
			__identifier(double)
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

		property bool HasSign
		{
			bool get();
		}

		property bool HasLength
		{
			bool get();
		}

		FundamentalType(DataType type);
		FundamentalType(DataType type, TypeSign signs, TypeLength lengths);

		int SizeOf(DataModel dataModel) override;
	};

}