#include "stdafx.h"
#include "OrPattern.h"
#include <algorithm>
#include <cassert>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	std::wstring OrPattern::indexEntryPrefix()
	{
		return wstring();
	}

	int OrPattern::matchLevel(Ast* node, bool resolve)
	{
		
		for (auto i = 0; i < num; ++i) {
			switch (patterns[i]->matchLevel(node,resolve))
			{
			case IMPOSSIBLE_MATCH:
				break;
			case POSSIBLE_MATCH:
				return POSSIBLE_MATCH;
			case ACCURATE_MATCH:
				return ACCURATE_MATCH;
			default:
				break;
			}
		}
		return IMPOSSIBLE_MATCH;
	}

	int OrPattern::matchLevel(Symbol* binding)
	{
		for (auto i = 0; i < num; ++i) {
			switch (patterns[i]->matchLevel(binding))
			{
			case IMPOSSIBLE_MATCH:
				break;
			case POSSIBLE_MATCH:
				return POSSIBLE_MATCH;
			case ACCURATE_MATCH:
				return ACCURATE_MATCH;
			default:
				break;
			}
		}
		return IMPOSSIBLE_MATCH;
	}

	bool OrPattern::matchesBinary(std::pair<void*, BinaryInfoType>& binaryInfo, std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
	
		for (auto i = 0; i < num; ++i) {
			if (patterns[i]->matchesBinary(binaryInfo, class_data))
			{
				return true;
			}
		}
		return false;
	}

	bool OrPattern::IsMayInTheFile(BaseLexStream* lex){
		for (auto i = 0; i < num; ++i){
			if( patterns[i]->IsMayInTheFile(lex) )
			{
				return true;
			}
		}
		return false;
	}

	bool OrPattern::matchesBinaryPackge(const std::wstring& compoundPackageName)
	{

		for (auto i = 0; i < num; ++i) {
			if (patterns[i]->matchesBinaryPackge(compoundPackageName))
			{
				return true;
			}
		}
		return false;

	}

	bool OrPattern::matchesBinaryClass(const std::wstring& mainTypeName)
	{

		for (auto i = 0; i < num; ++i) {
			if (patterns[i]->matchesBinaryClass(mainTypeName)){
				return true;
			}
		}
		return false;

	}

	OrPattern::~OrPattern()
	{
		for (auto i = 0; i < num; ++i)
		{
			delete patterns[i];
		}
		delete[] patterns;
	}

	OrPattern::OrPattern(SearchPattern* _leftPattern, SearchPattern* _rightPattern):
		SearchPattern(-1,false)
	{
		if (!_leftPattern || !_rightPattern)
			return;

		patterns = new SearchPattern*[2];
		matchMode = /*std::*/min(_leftPattern->matchMode, _rightPattern->matchMode);
		isCaseSensitive = _leftPattern->isCaseSensitive || _rightPattern->isCaseSensitive;

		patterns[0] = _leftPattern;
		patterns[1] = _rightPattern;
		num = 2;
		for (auto i = 0; i <num; ++i)
		{
			_container |= patterns[i]->matchContainer();
		}
	}

	OrPattern::OrPattern(SearchPattern** _patterns, int _num):SearchPattern(-1, false)
	{

		assert(_patterns);

		patterns = _patterns;
		num = _num;
		matchMode = 0;
		for (auto i = 0; i < num; ++i)
		{
			_container |= patterns[i]->matchContainer();
			if (patterns[i]->isCaseSensitive)
				isCaseSensitive = patterns[i]->isCaseSensitive;

			if(matchMode > patterns[i]->matchMode)
			{
				matchMode = patterns[i]->matchMode;
			}
		}
	}

	int OrPattern::matchContainer()
	{
		return _container;
	}
}


} // Close namespace Jikes block


