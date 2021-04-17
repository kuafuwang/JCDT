#ifndef Jikes_CodeAssist_AssistAstThisExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstThisExpression_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
	//
	// Represents qualified and simple 'this'.
	//
	class AssistAstThisExpression : public AssistExpression
	{
	public:
		

		  AssistAstThisExpression()
			: AssistExpression(THIS_EXPRESSION)
			
		{}
		~AssistAstThisExpression() {}


	};

 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstThisExpression_INCLUDED
