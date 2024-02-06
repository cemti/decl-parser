#pragma once
#include "Declaration.h"

namespace DeclParser
{
	public value struct NamedDeclaration
	{
		property System::String^ Name;
		property Declaration^ Declaration;

		NamedDeclaration(System::String^ name, DeclParser::Declaration^ declaration);

		void Deconstruct(
			[System::Runtime::InteropServices::Out]
			System::String^% name,
			[System::Runtime::InteropServices::Out]
			DeclParser::Declaration^% declaration);

		System::String^ ToString() override;
	};
}