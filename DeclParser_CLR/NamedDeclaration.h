#pragma once
#include "Declaration.h"

namespace DeclParser
{
	public value struct NamedDeclaration
	{
		property System::String^ Name;
		property Declaration^ Decl;

		NamedDeclaration(System::String^ name, Declaration^ decl);

		void Deconstruct(
			[System::Runtime::InteropServices::Out]
			System::String^% name,
			[System::Runtime::InteropServices::Out]
			Declaration^% decl);

		System::String^ ToString() override;
	};
}