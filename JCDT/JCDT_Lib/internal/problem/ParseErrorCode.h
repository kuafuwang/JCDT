
#ifndef ParseErrorCode_INCLUDED
#define ParseErrorCode_INCLUDED


namespace Jikes { // Open namespace Jikes block



	enum ParseErrorCode
	{
		ERROR_CODE,
		BEFORE_CODE,
		INSERTION_CODE,
		INVALID_CODE,
		SUBSTITUTION_CODE,
		DELETION_CODE,
		MERGE_CODE,
		MISPLACED_CODE,
		SCOPE_CODE,
		MANUAL_CODE,
		SECONDARY_CODE,
		EOF_CODE
	};




} // Close namespace Jikes block


#endif // ParseErrorCode_INCLUDED
