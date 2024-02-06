#include "pch.h"
#include "DataTypes.h"
using namespace System;

namespace DeclParser
{
	auto FundamentalType::Type::get() -> DataType
	{
		return _type;
	}

	auto FundamentalType::Sign::get() -> TypeSign
	{
		return _sign;
	}

	void FundamentalType::Sign::set(TypeSign value)
	{
		if (value != TypeSign::None)
			switch (_type)
			{
				case DataType::__identifier(char) :
					_sign = value;
					break;

				case DataType::__identifier(int) :
					if (value == TypeSign::__identifier(unsigned))
						_sign = value;

					break;

				default:
					throw gcnew ArgumentException("Unsupported sign qualifier '" + value.ToString() + "' for type '" + _type.ToString() + "'.");
			}
	}

	auto FundamentalType::Length::get() -> TypeLength
	{
		return _length;
	}

	void FundamentalType::Length::set(TypeLength value)
	{
		if (value != TypeLength::None)
			switch (_type)
			{
				case DataType::__identifier(char) :
					throw gcnew ArgumentException("'char' can't have a length qualifier.");

					case DataType::__identifier(int) :
						_length = value;
						break;

					case DataType::__identifier(double) :
						if (value == TypeLength::__identifier(long))
						{
							_length = value;
							break;
						}

					default:
						throw gcnew ArgumentException("Unsupported length qualifier '" + value.ToString() + "' for type '" + _type.ToString() + "'.");
			}
	}

	FundamentalType::FundamentalType(DataType type) : _type(type) { }

	FundamentalType::FundamentalType(DataType type, TypeSign signs, TypeLength lengths) : _type(type)
	{
		Sign = signs;
		Length = lengths;
	}

	int FundamentalType::SizeOf(DataModel dataModel)
	{
		switch (Type)
		{
			case DataType::__identifier(char) :
				return 1;

			case DataType::__identifier(float) :
				return 4;

			case DataType::__identifier(double) :
				switch (Length)
				{
					case TypeLength::__identifier(long) :
						return 12;

					default:
						return 8;
				}

			case DataType::__identifier(int) :
				switch (Length)
				{
					case TypeLength::__identifier(short) :
						return 2;

					case TypeLength::LongLong:
						return 8;

					case TypeLength::__identifier(long) :
						return dataModel == DataModel::LP64 ? 8 : 4;

					default:
						return 4;
				}

			default:
				return 0;
		}
	}	
}