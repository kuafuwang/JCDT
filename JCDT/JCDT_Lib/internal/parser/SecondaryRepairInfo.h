
#ifndef SecondaryRepairInfo_INCLUDED
#define SecondaryRepairInfo_INCLUDED
#include <JCDT_Lib/internal/problem/ParseErrorCode.h>


namespace Jikes { // Open namespace Jikes block




struct SecondaryRepairInfo
	{
		ParseErrorCode code;
		int distance,
			buffer_position,
			stack_position,
			num_deletions,
			symbol;

		bool recovery_on_next_stack;
	};




} // Close namespace Jikes block


#endif // SecondaryRepairInfo_INCLUDED
