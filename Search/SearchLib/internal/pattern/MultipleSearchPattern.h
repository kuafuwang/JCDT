#ifndef  MultipleSearchPattern_JIKES_SERACH_INCLUDE
#define MultipleSearchPattern_JIKES_SERACH_INCLUDE
#include "AndPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class MultipleSearchPattern:public AndPattern
	{
	public:
		MultipleSearchPattern(int _matchMode, bool _isCaseSensitive);
		std::wstring currentTag;
		// virtual std::vector< std::wstring > getPossibleTags() = 0;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif