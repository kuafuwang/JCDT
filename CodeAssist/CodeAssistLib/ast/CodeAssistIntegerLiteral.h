#ifndef Jikes_CodeAssist_AssistIntegerLiteral_INCLUDED
#define Jikes_CodeAssist_AssistIntegerLiteral_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


 namespace CodeAssist{
	//
	// Represents an int literal.
	//
	class AssistIntegerLiteral : public AssistExpression
	{
	public:
	
	
		AssistIntegerLiteral()
			: AssistExpression(INTEGER_LITERAL)
		{}
		~AssistIntegerLiteral() {}

	};

 }
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistIntegerLiteral_INCLUDED
