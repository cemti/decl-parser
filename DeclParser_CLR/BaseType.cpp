#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{
	String^ BaseType::QualifiersToString(Qualifiers qualifiers)
	{
		return qualifiers.ToString()->Replace(",", "");
	}

	String^ BaseType::TypeToString(BaseType^ type, String^ name, bool anonProto)
	{
		if (type == nullptr)
			return name;

		if (dynamic_cast<EllipsisType^>(type))
			return "...";

		auto chain = Enumerable::Empty<String^>();

		if (name)
			chain = Enumerable::Append(chain, name);

		bool hasPointer = false;

		while (auto cType = dynamic_cast<CompositeType^>(type))
		{
			if (auto dType = dynamic_cast<PointerType^>(type))
			{
				if (type->HasQualifier)
				{
					if (Enumerable::Any(chain))
						chain = Enumerable::Prepend<String^>(chain, " ");

					chain = Enumerable::Prepend(chain, QualifiersToString(type->Qualifier));
				}

				chain = Enumerable::Prepend<String^>(chain, "*");
				hasPointer = true;
			}
			else
			{
				if (hasPointer)
				{
					chain = Enumerable::Prepend<String^>(Enumerable::Append<String^>(chain, ")"), "(");
					hasPointer = false;
				}

				if (auto dType = dynamic_cast<FunctionType^>(type))
				{
					auto parameters = Enumerable::Empty<String^>();

					for each (auto p in dType->Parameters)
						parameters = Enumerable::Append(parameters, TypeToString(p.Declaration->Type, p.Name, false));

					chain = Enumerable::Concat<String^>(chain, gcnew array<String^> { "(", String::Join(L", ", parameters), ")" });
				}
				else if (auto dType = dynamic_cast<ArrayType^>(type))
					chain = Enumerable::Append(chain, String::Format("[{0}]", dType->Count));
			}

			type = cType->Decay;
		}

		chain = Enumerable::Repeat(String::Concat(chain), 1);

		if (auto dType = dynamic_cast<FundamentalType^>(type))
		{
			chain = Enumerable::Prepend(chain, dType->Type.ToString());

			if (dType->HasLength)
				chain = Enumerable::Prepend(chain, dType->Length == FundamentalType::TypeLength::LongLong ? "long long" : dType->Length.ToString());

			if (dType->HasSign)
				chain = Enumerable::Prepend(chain, dType->Sign.ToString());
		}
		else if (auto dType = dynamic_cast<NamedType^>(type))
		{
			anonProto &= name == nullptr;

			if (!dType->Anonymous || !anonProto)
				chain = Enumerable::Prepend(chain, dType->Name);

			if (!dType->Anonymous || anonProto)
				chain = Enumerable::Prepend(chain, dType->Keyword);
		}

		if (type && type->HasQualifier)
			chain = Enumerable::Prepend(chain, type->Qualifier.ToString());

		return String::Join(L' ', chain);
	}

	bool BaseType::HasQualifier::get()
	{
		return bool(Qualifier);
	}

	int BaseType::SizeOf(DataModel)
	{
		return 0;
	}

	void BaseType::SetQualifier(Qualifiers qualifiers)
	{
		Qualifier = Qualifier | qualifiers;
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