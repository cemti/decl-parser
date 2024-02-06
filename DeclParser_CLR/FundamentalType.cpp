#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	auto FundamentalType::Type::get() -> Types
	{
		return type;
	}

	auto FundamentalType::Sign::get() -> Signs
	{
		return sign;
	}

	void FundamentalType::Sign::set(Signs value)
	{
		if (bool(value))
			switch (type)
			{
				case Types::__identifier(char) :
					sign = value;
					break;

				case Types::__identifier(int) :
					if (value == Signs::__identifier(unsigned))
						sign = value;

					break;

				default:
					throw gcnew ArgumentException("Unsupported sign qualifier '" + value.ToString() + "' for type '" + type.ToString() + "'.");
			}
	}

	auto FundamentalType::Length::get() -> Lengths
	{
		return length;
	}

	void FundamentalType::Length::set(Lengths value)
	{
		if (bool(value))
			switch (type)
			{
				case Types::__identifier(char) :
					throw gcnew ArgumentException("'char' can't have a length qualifier.");

					case Types::__identifier(int) :
						length = value;
						break;

					case Types::__identifier(double) :
						if (value == Lengths::__identifier(long))
						{
							length = value;
							break;
						}

					default:
						throw gcnew ArgumentException("Unsupported length qualifier '" + value.ToString() + "' for type '" + type.ToString() + "'.");
			}
	}

	bool FundamentalType::HasSign::get()
	{
		return bool(Sign);
	}

	bool FundamentalType::HasLength::get()
	{
		return bool(Length);
	}
	
	FundamentalType::FundamentalType(Types type) : type(type) { }

	FundamentalType::FundamentalType(Types type, Signs signs, Lengths lengths) : type(type)
	{
		Sign = signs;
		Length = lengths;
	}

	int FundamentalType::SizeOf(DataModel dm)
	{
		switch (Type)
		{
			case Types::__identifier(char) :
				return 1;

			case Types::__identifier(float) :
				return 4;

			case Types::__identifier(double) :
				switch (Length)
				{
					case Lengths::__identifier(long) :
						return 12;

					default:
						return 8;
				}

			case Types::__identifier(int) :
				switch (Length)
				{
					case Lengths::__identifier(short) :
						return 2;

					case Lengths::LongLong:
						return 8;

					case Lengths::__identifier(long) :
						return dm == DataModel::LP64 ? 8 : 4;

					default:
						return 4;
				}

			default:
				return 0;
		}
	}	
}