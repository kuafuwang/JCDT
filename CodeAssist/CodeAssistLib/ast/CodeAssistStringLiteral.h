#ifndef Jikes_CodeAssist_AssistStringLiteral_INCLUDED
#define Jikes_CodeAssist_AssistStringLiteral_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


 namespace CodeAssist{
				  //
				  // AssistStringLiteral --> <STRING_LITERAL, Literal, value>
				  //
	class AssistStringLiteral : public AssistExpression
	{
	public:
		
		
		  AssistStringLiteral()
			: AssistExpression(STRING_LITERAL)
			
		{}
		~AssistStringLiteral() {}


	};

 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistStringLiteral_INCLUDED
