#ifndef Jikes_CodeAssist_AssistAstParenthesizedExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstParenthesizedExpression_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
				  //
				  // ParenthesizedExpression --> <PARENTHESIZED_EXPRESSION, (_token, AssistExpression,
				  // )_token>
				  //
	class AssistAstParenthesizedExpression : public AssistExpression
	{
	public:
		
		
		AssistExpression* expression;
	
		 AssistAstParenthesizedExpression()
			: AssistExpression(PARENTHESIZED_EXPRESSION)
		{}
		~AssistAstParenthesizedExpression() {}

	};

 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstParenthesizedExpression_INCLUDED
