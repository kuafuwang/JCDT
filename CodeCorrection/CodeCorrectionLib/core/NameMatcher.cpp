#include "stdafx.h"

#include "NameMatcher.h"
#include <JCDT_Lib/internal/util/spell.h>
bool Jikes::Correction::NameMatcher::isSimilarName(const std::wstring& name1, const std::wstring& name2)
{
	int index = Spell::Index(name1.c_str(), name2.c_str());
	if (index > 7)
	{
		return index * 20;
	}
	return getSimilarity(name1, name2) >= 0;
}

int Jikes::Correction::NameMatcher::getSimilarity(std::wstring name1, std::wstring name2)
{
	int name1len = name1.length();
	int name2len = name2.length();
	if (name1len > name2len)
	{
		if (name1len > name2len * 2)
		{
			return -1;
		}
		std::wstring tmp = name1;
		name1 = name2;
		name2 = tmp;
	}
	else
	{
		if (name1len * 2 < name2len)
		{
			return -1;
		}
	}
	name1len = name1.length();
	name2len = name2.length();

	int nMatched = 0;

	int i = 0;
	while (i < name1len && isSimilarChar(name1[i], name2[i]))
	{
		i++;
		nMatched++;
	}

	int k = name1len;
	int diff = name2len - name1len;
	while (k > i && isSimilarChar(name1[k - 1], name2[(k + diff - 1)]))
	{
		k--;
		nMatched++;
	}

	if (nMatched == name1len)
	{
		return 200;
	}

	if (name2len - nMatched > nMatched)
	{
		return -1;
	}

	int tolerance = name1len / 4 + 1;
	return (tolerance - (k - i)) * 256 / tolerance;
}

bool Jikes::Correction::NameMatcher::isSimilarChar(int ch1, int ch2)
{
	return ::tolower(ch1) == ::tolower(ch2);
}
