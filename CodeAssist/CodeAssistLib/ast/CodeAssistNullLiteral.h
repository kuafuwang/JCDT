#ifndef Jikes_CodeAssist_AssistNullLiteral_INCLUDED
#define Jikes_CodeAssist_AssistNullLiteral_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
				  //
				  // AssistNullLiteral --> <NULL_EXPRESSION, null_token>
				  //
	class AssistNullLiteral : public AssistExpression
	{
	public:
		
		
		  AssistNullLiteral()
			: AssistExpression(NULL_LITERAL)
		{}
		~AssistNullLiteral() {}


	};

 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistNullLiteral_INCLUDED
