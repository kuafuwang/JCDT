#ifndef Jikes_CodeAssist_AssistFloatLiteral_INCLUDED
#define Jikes_CodeAssist_AssistFloatLiteral_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

	// AssistFloatLiteral --> <FLOAT_LITERAL, Literal, value>
     namespace CodeAssist{
	class AssistFloatLiteral : public AssistExpression
	{
	public:
		
		 AssistFloatLiteral()
			: AssistExpression(FLOAT_LITERAL)
		{}
		~AssistFloatLiteral() {}


	};

     }



} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistFloatLiteral_INCLUDED
