#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{	
	String^ BaseType::QualifiersToString(Qualifiers q)
	{
		return q.ToString()->Replace(",", "");
	}
	
	String^ BaseType::TypeToString(BaseType^ type, String^ name, bool anonProto)
	{
		if (type == nullptr)
			return name;

		if (dynamic_cast<EllipsisType^>(type))
			return "...";
		
		auto en = Enumerable::Empty<String^>();

		if (name)
			en = Enumerable::Append(en, name);

		bool prevFlag = false;

		while (auto cType = dynamic_cast<CompositeType^>(type))
		{
			if (auto dType = dynamic_cast<PointerType^>(type))
			{
				if (type->HasQualifier)
				{
					if (Enumerable::Any(en))
						en = Enumerable::Prepend<String^>(en, " ");

					en = Enumerable::Prepend(en, QualifiersToString(type->Qualifier));
				}

				en = Enumerable::Prepend<String^>(en, "*");
				prevFlag = true;
			}
			else
			{
				if (prevFlag)
				{
					en = Enumerable::Prepend<String^>(Enumerable::Append<String^>(en, ")"), "(");
					prevFlag = false;
				}

				if (auto dType = dynamic_cast<FunctionType^>(type))
				{
					auto params = Enumerable::Empty<String^>();

					for each (auto p in dType->Parameters)
						params = Enumerable::Append(params, TypeToString(p.Decl->Type, p.Name, false));

					en = Enumerable::Concat<String^>(en, gcnew array<String^> { "(", String::Join(L", ", params), ")" });
				}
				else if (auto dType = dynamic_cast<ArrayType^>(type))
					en = Enumerable::Append(en, String::Format("[{0}]", dType->Count));
			}

			type = cType->Decay;
		}

		en = Enumerable::Repeat(String::Concat(en), 1);

		if (auto dType = dynamic_cast<FundamentalType^>(type))
		{
			en = Enumerable::Prepend(en, dType->Type.ToString());

			if (dType->HasLength)
				en = Enumerable::Prepend(en, dType->Length == FundamentalType::Lengths::LongLong ? "long long" : dType->Length.ToString());

			if (dType->HasSign)
				en = Enumerable::Prepend(en, dType->Sign.ToString());
		}
		else if (auto dType = dynamic_cast<NamedType^>(type))
		{
			anonProto &= name == nullptr;

			if (!dType->Anonymous || !anonProto)
				en = Enumerable::Prepend(en, dType->Name);

			if (!dType->Anonymous || anonProto)
				en = Enumerable::Prepend(en, dType->Keyword);
		}

		if (type && type->HasQualifier)
			en = Enumerable::Prepend(en, type->Qualifier.ToString());

		return String::Join(L' ', en);
	}

	static String^ DeclarationToString(Declaration^ decl, String^ name)
	{
		auto en = Enumerable::Repeat(BaseType::TypeToString(decl->Type, name, false), 1);

		if (decl->Inline)
			Enumerable::Prepend<String^>(en, "inline");

		if (decl->HasSpecifier)
			Enumerable::Prepend(en, decl->Specifier.ToString());

		return String::Join(L' ', en);
	}

	String^ Declaration::ToString() { return DeclarationToString(this, nullptr); }
	String^ NamedDeclaration::ToString() { return DeclarationToString(Decl, Name); }

	int FundamentalType::SizeOf(DataModel dm)
	{
		switch (Type)
		{
		case Types::__identifier(char):
			return 1;

		case Types::__identifier(float):
			return 4;

		case Types::__identifier(double):
			switch (Length)
			{
			case Lengths::__identifier(long):
				return 12;

			default:
				return 8;
			}

		case Types::__identifier(int):
			switch (Length)
			{
			case Lengths::__identifier(short):
				return 2;

			case Lengths::LongLong:
				return 8;

			case Lengths::__identifier(long):
				return dm == DataModel::LP64 ? 8 : 4;

			default:
				return 4;
			}

		default:
			return 0;
		}
	}

	int PointerType::SizeOf(DataModel dm)
	{
		return dm == DataModel::LLP64 || dm == DataModel::LP64 ? 8 : 4;
	}

	int ArrayType::SizeOf(DataModel dm)
	{
		return Count.HasValue && Decay ? Count.Value * Decay->SizeOf(dm) : 0;
	}

	int StructType::SizeOf(DataModel dm)
	{
		int size = 0;

		if (IsUnion)
		{
			for each (auto p in Members)
				if (auto temp = p.Decl->Type->SizeOf(dm); temp > 0)
					size = Math::Max(temp, size);
		}
		else
		{
			for each (auto p in Members)
				size += p.Decl->Type->SizeOf(dm);
		}

		return size == 0 ? 1 : size;
	}

	int EnumType::SizeOf(DataModel) { return 4; }
}