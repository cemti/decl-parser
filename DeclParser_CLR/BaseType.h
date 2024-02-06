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
		enum class Qualifiers
		{
			None, __identifier(const), __identifier(volatile), __identifier(restrict) = 4
		};

		static System::String^ QualifiersToString(Qualifiers qualifiers);
		static System::String^ TypeToString(BaseType^ type, System::String^ name, bool anonProto);

		property Qualifiers Qualifier;

		virtual int SizeOf(DataModel dataModel);
		virtual void SetQualifier(Qualifiers qualifiers);

		virtual System::Object^ Clone();
		System::String^ ToString() override;
	};
}