#include "stdafx.h"
#include "MultipleSearchPattern.h"
namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	MultipleSearchPattern::MultipleSearchPattern(int _matchMode, bool _isCaseSensitive)
		:AndPattern(_matchMode, _isCaseSensitive)
	{
	}
}


} // Close namespace Jikes block


