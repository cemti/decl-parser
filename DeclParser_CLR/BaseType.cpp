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

	bool BaseType::HasQualifier::get()
	{
		return bool(Qualifier);
	}

	int BaseType::SizeOf(DataModel)
	{
		return 0;
	}

	void BaseType::SetQualifier(Qualifiers q)
	{
		Qualifier = Qualifier | q;
	}

	Object^ BaseType::Clone()
	{
		return MemberwiseClone();
	}

	String^ BaseType::ToString()
	{
		return TypeToString(this, nullptr, false);
	}
}