#ifndef CaseElement_INCLUDED
#define CaseElement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"

namespace Jikes { // Open namespace Jikes block

	// This structure allows a switch statement to sort its case labels. It should
	// be a plain-old-data type (POD) for efficient copying.
	struct CaseElement
	{
		unsigned block_index; // which SwitchBlockStatement
		unsigned case_index; // which case label within the block
		i4 value; // the value of the case's expression

				  //
				  // This keeps the sort stable, so that duplicates stay later in the list.
				  //
		  bool operator<(CaseElement& right)
		{
			return value < right.value ||
				(value == right.value &&
					(block_index < right.block_index ||
						(block_index == right.block_index &&
							case_index < right.case_index)));
		}
	};





} // Close namespace Jikes block


#endif // CaseElement_INCLUDED