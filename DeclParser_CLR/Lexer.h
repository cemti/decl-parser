#pragma once

namespace DeclParser
{
	public ref class Lexer
	{
	public:
		enum class RegexGroups
		{
			Comment = 1, Sign, Length, Type, SType, Qualifier, Specifier, Ellipsis,
			Brace, ArrayBrace, ScopeBrace, Special, Integer, Name, Unknown
		};
	private:
		initonly static System::String^ pattern = R"((/\*((?'comment'/\*)|(?'-comment'\*/)|(?!\*/).)+\*/|(//|\#).*?(?<!\\\s*)$)|
\b((?'sign'(un)?signed)|
(?'length'short|long)|
(?'type'void|char|int|float|double)|
(?'sType'struct|union|enum)|
(?'qualifier'const|volatile|restrict)|
(?'specifier'static|auto|register|extern))\b|
(?'ellipsis'\.{3})|
(?'brace'[()])|
(?'arrayBrace'[[\]])|
(?'scopeBrace'[{}])|
(?'special'[,;*]|(typedef|inline)\b)|
(?'integer'\d+)|
(?'name'\w+)|
(?'unknown'""(\\""|.)*?""|\S))";

		static initonly System::Text::RegularExpressions::RegexOptions _options =
			System::Text::RegularExpressions::RegexOptions::Compiled |
			System::Text::RegularExpressions::RegexOptions::Multiline |
			System::Text::RegularExpressions::RegexOptions::IgnorePatternWhitespace |
			System::Text::RegularExpressions::RegexOptions::Singleline |
			System::Text::RegularExpressions::RegexOptions::ExplicitCapture;

		static System::Text::RegularExpressions::Regex _regex{ pattern, _options };

		System::Collections::Generic::Dictionary<int, int> _skipIndices;
		System::Collections::Generic::List<System::ValueTuple<RegexGroups, System::Text::RegularExpressions::Match^>> _groups;
		initonly System::String^ _input;
	public:
		property int Index;

		property RegexGroups Group
		{
			RegexGroups get();
		}

		property System::String^ Value
		{
			System::String^ get();
		}
		
		property System::Text::RegularExpressions::Match^ CurrentMatch
		{
			System::Text::RegularExpressions::Match^ get();
		}

		property bool HasNext
		{
			bool get();
		}

		property int TextIndex
		{
			int get();
		}

		Lexer();
		Lexer(System::String^);

		bool TryMoveNext();
		void MoveNext();
		void Skip();
	};
}