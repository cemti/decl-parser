#include "pch.h"
#include "Lexer.h"
#include <stack>
using namespace System;
using namespace Text::RegularExpressions;

namespace DeclParser
{
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
}