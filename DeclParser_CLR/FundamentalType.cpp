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
				case DataType::Char:
					_sign = value;
					break;

				case DataType::Int:
					if (value == TypeSign::Unsigned)
						_sign = value;

					break;

				default:
					throw gcnew ArgumentException("Unsupported sign qualifier '" + value.ToString()->ToLower() + "' for type '" + _type.ToString()->ToLower() + "'.");
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
				case DataType::Char:
					throw gcnew ArgumentException("'char' can't have a length qualifier.");

					case DataType::Int:
						_length = value;
						break;

					case DataType::Double:
						if (value == TypeLength::Long)
						{
							_length = value;
							break;
						}

					default:
						throw gcnew ArgumentException("Unsupported length qualifier '" + value.ToString()->ToLower() + "' for type '" + _type.ToString()->ToLower() + "'.");
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
			case DataType::Char:
				return 1;

			case DataType::Float:
				return 4;

			case DataType::Double:
				switch (Length)
				{
					case TypeLength::Long:
						return 12;

					default:
						return 8;
				}

			case DataType::Int:
				switch (Length)
				{
					case TypeLength::Short:
						return 2;

					case TypeLength::LongLong:
						return 8;

					case TypeLength::Long:
						return dataModel == DataModel::LP64 ? 8 : 4;

					default:
						return dataModel == DataModel::LP32 ? 2 : 4;
				}

			default:
				return 0;
		}
	}	
}