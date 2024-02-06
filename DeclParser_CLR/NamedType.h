#pragma once
#include "BaseType.h"

namespace DeclParser
{
	public ref class NamedType abstract : BaseType
	{
		const bool anon;
		System::Byte stage;
	public:
		property System::String^ Name;

		property bool Anonymous
		{
			bool get();
		}

		property bool Defined
		{
			bool get();
		}

		property bool Instantiable
		{
			bool get();
		}

		property bool Empty
		{
			virtual bool get() = 0;
		}

		property System::String^ Keyword
		{
			virtual System::String^ get() = 0;
		}

		NamedType(System::String^ name, bool instantiable);

		void Define();		
	};

	using DeclaredNamedTypes = System::Collections::Generic::Dictionary<System::String^, NamedType^>;
	using Declarations = System::Collections::Generic::List<NamedDeclaration>;
}