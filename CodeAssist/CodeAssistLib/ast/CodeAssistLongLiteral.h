#ifndef Jikes_CodeAssist_AssistLongLiteral_INCLUDED
#define Jikes_CodeAssist_AssistLongLiteral_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
	//
	// AssistLongLiteral --> <LONG_LITERAL, long_literal_token, value>
	//
	class AssistLongLiteral : public AssistExpression
	{
	public:
		
	
		  AssistLongLiteral( )
			: AssistExpression(LONG_LITERAL)
		{}
		~AssistLongLiteral() {}


	};

 }
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistLongLiteral_INCLUDED
