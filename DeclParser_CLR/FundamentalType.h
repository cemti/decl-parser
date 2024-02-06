#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class FundamentalType : BaseType
	{
	public:
		enum class Lengths
		{
			__identifier(short) = 1, __identifier(long), LongLong
		};

		enum class Signs
		{
			__identifier(signed) = 1, __identifier(unsigned)
		};

		enum class Types
		{
			__identifier(void), __identifier(char),
			__identifier(int), __identifier(float),
			__identifier(double)
		};
	private:
		const Types type;
		Signs sign;
		Lengths length;
	public:
		property Types Type
		{
			Types get();
		}

		property Signs Sign
		{
			Signs get();
			void set(Signs value);			
		}

		property Lengths Length
		{
			Lengths get();
			void set(Lengths value);
		}

		property bool HasSign
		{
			bool get();
		}

		property bool HasLength
		{
			bool get();
		}

		FundamentalType(Types type);
		FundamentalType(Types type, Signs signs, Lengths lengths);

		int SizeOf(DataModel dm) override;
	};

}