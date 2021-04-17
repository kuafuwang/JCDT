
#ifndef PrimaryRepairInfo_INCLUDED
#define PrimaryRepairInfo_INCLUDED
#include <JCDT_Lib/internal/problem/ParseErrorCode.h>

namespace Jikes { // Open namespace Jikes block




struct PrimaryRepairInfo
	{
		ParseErrorCode code;

		int distance,
			buffer_position,
			misspell_index,
			symbol;
	};



} // Close namespace Jikes block


#endif // PrimaryRepairInfo_INCLUDED
