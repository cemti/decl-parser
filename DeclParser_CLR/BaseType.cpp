#include "pch.h"
#include "DataTypes.h"
using namespace System;
using namespace Linq;

namespace DeclParser
{
	String^ BaseType::QualifiersToString(TypeQualifiers qualifiers)
	{
		return String::Join(" ", Enumerable::Except<String^>(
			qualifiers.ToString()->Split(", ", StringSplitOptions::None),
			gcnew array<String^> { "None" })
		)->ToLower();
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
				if (type->Qualifiers != TypeQualifiers::None)
				{
					if (Enumerable::Any(chain))
						chain = Enumerable::Prepend<String^>(chain, " ");

					chain = Enumerable::Prepend(chain, QualifiersToString(type->Qualifiers));
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
			chain = Enumerable::Prepend(chain, dType->Type.ToString()->ToLower());

			if (dType->Length != FundamentalType::TypeLength::None)
				chain = Enumerable::Prepend(chain, dType->Length == FundamentalType::TypeLength::LongLong ? "long long" : dType->Length.ToString()->ToLower());

			if (dType->Sign != FundamentalType::TypeSign::None)
				chain = Enumerable::Prepend(chain, dType->Sign.ToString()->ToLower());
		}
		else if (auto dType = dynamic_cast<NamedType^>(type))
		{
			anonProto &= name == nullptr;

			if (!dType->Anonymous || !anonProto)
				chain = Enumerable::Prepend(chain, dType->Name);

			if (!dType->Anonymous || anonProto)
				chain = Enumerable::Prepend(chain, dType->Keyword);
		}

		if (type && type->Qualifiers != TypeQualifiers::None)
			chain = Enumerable::Prepend(chain, QualifiersToString(type->Qualifiers));

		return String::Join(L' ', chain);
	}

	int BaseType::SizeOf(DataModel)
	{
		return 0;
	}

	void BaseType::SetQualifier(TypeQualifiers qualifiers)
	{
		Qualifiers = Qualifiers | qualifiers;
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