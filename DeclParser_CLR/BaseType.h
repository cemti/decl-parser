#pragma once

namespace DeclParser
{
	public ref class BaseType abstract : System::ICloneable
	{
	public:
		enum class DataModel
		{
			LP32, ILP32, LLP64, LP64
		};

		[System::FlagsAttribute]
		enum class TypeQualifiers
		{
			None, Const, Volatile, Restrict = 4
		};

		static System::String^ QualifiersToString(TypeQualifiers qualifiers);
		static System::String^ TypeToString(BaseType^ type, System::String^ name, bool anonProto);

		property TypeQualifiers Qualifiers;

		virtual int SizeOf(DataModel dataModel);
		virtual void SetQualifier(TypeQualifiers qualifiers);

		virtual System::Object^ Clone();
		System::String^ ToString() override;
	};
}