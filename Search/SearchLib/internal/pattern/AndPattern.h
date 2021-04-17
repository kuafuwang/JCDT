#ifndef  AndPattern_JIKES_SERACH_INCLUDE
#define AndPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	
	class AndPattern :public SearchPattern
	{
	public:
		AndPattern(int _matchMode, bool _isCaseSensitive);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif