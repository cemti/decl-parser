#include "pch.h"
#include "Lexer.h"
#include <stack>
using namespace System;
using namespace Text::RegularExpressions;

namespace DeclParser
{
	auto Lexer::Group::get() -> RegexGroups
	{
		return _groups[Index].Item1;
	}

	String^ Lexer::Value::get()
	{
		return _groups[Index].Item2->Value;
	}

	Match^ Lexer::CurrentMatch::get()
	{
		return _groups[Index].Item2;
	}

	bool Lexer::HasNext::get()
	{
		return Index < 0 || Index >= _groups.Count;
	}

	int Lexer::TextIndex::get()
	{
		return CurrentMatch->Index;
	}
	
	Lexer::Lexer(String^ str) : _str(str)
	{ 
		Index = -1;
		std::stack<int> stack;

		for (auto match = _regex.Match(str); match->Success; match = match->NextMatch())
			for (int i = 1; i < match->Groups->Count; ++i)
				if (match->Groups[i]->Success)
				{
					switch (auto rg = static_cast<RegexGroups>(i))
					{
					case RegexGroups::Brace:
						if (match->Value != "(")
						{
							if (!stack.empty())
							{
								int idx = stack.top();
								stack.pop();
								_skipIndices.Add(idx, _groups.Count);
							}
						}
						else
							stack.push(_groups.Count);

					default:
						_groups.Add(ValueTuple<RegexGroups, Match^>(rg, match));

					case RegexGroups::Comment:;
					}

					break;
				}
	}

	bool Lexer::TryMoveNext()
	{
		++Index;
		return !HasNext;
	}

	void Lexer::MoveNext()
	{
		if (!TryMoveNext())
			throw gcnew System::FormatException("Unexpected end of file.");
	}

	void Lexer::Skip()
	{
		if (int skipIndex; _skipIndices.TryGetValue(Index, skipIndex))
			Index = skipIndex;
		else
			MoveNext();
	}
}