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
		initonly static System::String^ pattern = R"((?'comment'/(\*.*?\*/|/.*?$))|
((?'sign'(un)?signed)|
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

		static initonly System::Text::RegularExpressions::RegexOptions s_options =
			System::Text::RegularExpressions::RegexOptions::Compiled |
			System::Text::RegularExpressions::RegexOptions::Multiline |
			System::Text::RegularExpressions::RegexOptions::IgnorePatternWhitespace |
			System::Text::RegularExpressions::RegexOptions::Singleline |
			System::Text::RegularExpressions::RegexOptions::ExplicitCapture;

		static System::Text::RegularExpressions::Regex _regex{ pattern, s_options };

		System::Collections::Generic::Dictionary<int, int> _skipIndices;
		System::Collections::Generic::List<System::ValueTuple<RegexGroups, System::Text::RegularExpressions::Match^>> _groups;
		initonly System::String^ _str;
	public:
		property int Index;

		property RegexGroups Group
		{
			RegexGroups get() { return _groups[Index].Item1; }
		}

		property System::String^ Value
		{
			System::String^ get() { return _groups[Index].Item2->Value; }
		}
		
		property System::Text::RegularExpressions::Match^ CurrentMatch
		{
			System::Text::RegularExpressions::Match^ get() { return _groups[Index].Item2; }
		}

		property bool HasNext
		{
			bool get() { return Index < 0 || Index >= _groups.Count; }
		}

		property int TextIndex
		{
			int get() { return CurrentMatch->Index; }
		}

		Lexer() {}
		Lexer(System::String^);

		bool TryMoveNext()
		{
			++Index;
			return !HasNext;
		}

		void MoveNext()
		{
			if (!TryMoveNext())
				throw gcnew System::FormatException("Unexpected end of file.");
		}

		void Skip()
		{
			if (int skipIndex; _skipIndices.TryGetValue(Index, skipIndex))
				Index = skipIndex;
			else
				MoveNext();			
		}
	};
}