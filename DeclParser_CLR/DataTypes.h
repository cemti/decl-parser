#pragma once

namespace DeclParser
{
	ref class BaseType;
	ref class NamedType;
	ref class Declaration;
	value struct NamedDeclaration;

	using DeclaredNamedTypes = System::Collections::Generic::Dictionary<System::String^, NamedType^>;
	using Declarations = System::Collections::Generic::List<NamedDeclaration>;

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
			__identifier(const) = 1, __identifier(volatile), __identifier(restrict) = 4
		};

		static System::String^ QualifiersToString(Qualifiers);
		static System::String^ TypeToString(BaseType^, System::String^, bool);

		property Qualifiers Qualifier;

		property bool HasQualifier
		{
			bool get() { return bool(Qualifier); }
		}

		virtual int SizeOf(DataModel) { return 0; }

		virtual System::Object^ Clone() { return MemberwiseClone();	}

		virtual void SetQualifier(Qualifiers q) { Qualifier = Qualifier | q; }
		System::String^ ToString() override { return TypeToString(this, nullptr, false); }
	};

	public ref class Declaration
	{
	public:
		enum class StorageSpecifiers
		{
			__identifier(static) = 1, __identifier(auto),
			__identifier(register), __identifier(extern)
		};

		property BaseType^ Type;
		property StorageSpecifiers Specifier;
		property bool Inline;

		property bool HasSpecifier
		{
			bool get() { return bool(Specifier); }
		}

		Declaration(Declaration^ decl) : Declaration(decl->Type, decl->Specifier, decl->Inline) {}

		Declaration(BaseType^ type) : Declaration(type, StorageSpecifiers(), false) {}
		Declaration(BaseType^ type, StorageSpecifiers specifier) : Declaration(type, specifier, false) {}

		Declaration(BaseType^ type, StorageSpecifiers specifier, bool isInline)
		{
			Type = type;
			Specifier = specifier;
			Inline = isInline;
		}

		System::String^ ToString() override;
	};

	public value struct NamedDeclaration
	{
		property System::String^ Name;
		property Declaration^ Decl;

		NamedDeclaration(System::String^ name, Declaration^ decl)
		{
			Name = name;
			Decl = decl;
		}

		void Deconstruct(
			[System::Runtime::InteropServices::Out]
			System::String^% name, 
			[System::Runtime::InteropServices::Out]
			Declaration^% decl)
		{
			name = Name;
			decl = Decl;
		}

		System::String^ ToString() override;
	};

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
			Types get() { return type; }
		}

		property Signs Sign
		{
			Signs get() { return sign; }

			void set(Signs value)
			{
				if (bool(value))
					switch (type)
					{
					case Types::__identifier(char):
						sign = value;
						break;

					case Types::__identifier(int):
						if (value == Signs::__identifier(unsigned))
							sign = value;

						break;

					default:
						throw gcnew System::ArgumentException("Unsupported sign qualifier '" + value.ToString() + "' for type '" + type.ToString() + "'.");
					}
			}
		}

		property Lengths Length
		{
			Lengths get() { return length; }

			void set(Lengths value)
			{
				if (bool(value))
					switch (type)
					{
					case Types::__identifier(char):
						throw gcnew System::ArgumentException("'char' can't have a length qualifier.");

					case Types::__identifier(int):
						length = value;
						break;

					case Types::__identifier(double):
						if (value == Lengths::__identifier(long))
						{
							length = value;
							break;
						}

					default:
						throw gcnew System::ArgumentException("Unsupported length qualifier '" + value.ToString() + "' for type '" + type.ToString() + "'.");
					}
			}
		}

		property bool HasSign
		{
			bool get() { return bool(Sign); }
		}

		property bool HasLength
		{
			bool get() { return bool(Length); }
		}

		FundamentalType(Types type) : type(type) {}

		FundamentalType(Types type, Signs signs, Lengths lengths) : type(type)
		{
			Sign = signs;
			Length = lengths;
		}

		int SizeOf(DataModel dm) override;
	};

	public ref class CompositeType abstract : BaseType
	{
	protected:
		CompositeType(BaseType^ decaysTo) { Decay = decaysTo; }
	public:
		virtual System::Object^ Clone() override
		{
			auto clone = static_cast<CompositeType^>(BaseType::Clone());
			clone->Decay = static_cast<BaseType^>(clone->Decay->Clone());
			return clone;
		}

		property BaseType^ Decay;
	};

	public ref class PointerType : CompositeType
	{
	public:
		PointerType() : PointerType(nullptr) {}
		PointerType(BaseType^ decaysTo) : CompositeType(decaysTo) {}

		int SizeOf(DataModel dm) override;
	};

	public ref class ArrayType : CompositeType
	{
	public:
		property System::Nullable<int> Count;
		
		void SetQualifier(Qualifiers q) override { Decay->SetQualifier(q); }

		ArrayType() : ArrayType(nullptr) {}
		ArrayType(BaseType^ decaysTo) : CompositeType(decaysTo) {}

		int SizeOf(DataModel dm) override;
	};

	public ref class FunctionType : CompositeType
	{
	public:
		property Declarations^ Parameters;

		property bool HasNoParams
		{
			bool get()
			{
				if (Parameters->Count == 0)
					return true;

				auto fType = dynamic_cast<FundamentalType^>(Parameters[0].Decl->Type);
				return fType && fType->Type == FundamentalType::Types::__identifier(void);
			}
		}

		void SetQualifier(Qualifiers q) override
		{
			if (bool(q))
				throw gcnew System::ArgumentException("Function doesn't accept any qualifier.");
		}

		FunctionType() : FunctionType(nullptr, nullptr) {}
		FunctionType(BaseType^ decaysTo, Declarations^ params) : CompositeType(decaysTo) { Parameters = params; }
	};

	public ref class NamedType abstract : BaseType
	{
		const bool anon;
		System::Byte stage;
	public:
		property System::String^ Name;

		property bool Anonymous
		{
			bool get() { return anon; }
		}

		property bool Defined
		{
			bool get() { return stage > 0; }
		}

		property bool Instantiable
		{
			bool get() { return stage == 2; }
		}

		property bool Empty
		{
			virtual bool get() = 0;
		}

		property System::String^ Keyword
		{
			virtual System::String^ get() = 0;
		}

		void Define()
		{
			if (stage != 2)
				++stage;
		}

		NamedType(System::String^ name, bool instantiable) : anon(System::String::IsNullOrWhiteSpace(name)), stage(instantiable ? 2 : 0)
		{
			Name = name;
		}
	};

	public ref class StructType : NamedType
	{
		const bool isUnion;
	public:
		property Declarations^ Members;

		property bool IsUnion
		{
			bool get() { return isUnion; }
		}

		property bool Empty
		{
			virtual bool get() override { return Members->Count == 0; };
		}

		property System::String^ Keyword
		{
			System::String^ get() override
			{
				return IsUnion ? "union" : "struct";
			}
		}

		StructType() : StructType(nullptr) {}
		StructType(System::String^ name) : StructType(name, false) {}
		StructType(System::String^ name, bool isUnion) : StructType(name, isUnion, false) {}
		StructType(System::String^ name, bool isUnion, bool complete) : NamedType(name, complete), isUnion(isUnion) { Members = gcnew Declarations; }

		int SizeOf(DataModel dm) override;
	};

	public ref class EnumType : NamedType
	{
	public:
		using Enums = System::Collections::Generic::List<System::String^>;
		property Enums^ Enumerators;

		property bool Empty
		{
			virtual bool get() override { return Enumerators->Count == 0; };
		}

		property System::String^ Keyword
		{
			System::String^ get() override { return "enum"; }
		}

		EnumType() : EnumType(nullptr) {}
		EnumType(System::String^ name) : EnumType(name, false) {}
		EnumType(System::String^ name, bool complete) : NamedType(name, complete) { Enumerators = gcnew Enums; }

		int SizeOf(DataModel) override;
	};

	public ref class EllipsisType : BaseType
	{
	public:
		void SetQualifier(Qualifiers q) override
		{
			if (bool(q))
				throw gcnew System::ArgumentException("Ellipsis doesn't accept any qualifier.");
		}
	};
}