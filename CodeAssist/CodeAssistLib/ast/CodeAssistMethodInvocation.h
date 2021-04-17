#ifndef Jikes_CodeAssist_AssistAstMethodInvocation_INCLUDED
#define Jikes_CodeAssist_AssistAstMethodInvocation_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block
	// Represents a method call.  Sometimes, during semantic analysis an
	// artificial base_opt expression is constructed. In such a case, the user
	// can determine this condition by testing base_opt -> generated.
	//
     namespace CodeAssist{
	class AssistAstMethodInvocation : public AssistExpression
	{
	public:

		
	
		
		  AssistAstMethodInvocation()
			: AssistExpression(CALL)
		{}
		~AssistAstMethodInvocation() {}


	};
     }


} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstMethodInvocation_INCLUDED
