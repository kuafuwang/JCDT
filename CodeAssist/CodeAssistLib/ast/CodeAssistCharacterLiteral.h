#ifndef Jikes_CodeAssist_AssistCharLiteral_INCLUDED
#define Jikes_CodeAssist_AssistCharLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block
   namespace CodeAssist{
	//
	// CharacterLiteral --> <CHARACTER_LITERAL, literal_token, value>
	//
	   class AssistCharLiteral : public AssistExpression
	{
	public:
		
	
		  AssistCharLiteral()
			: AssistExpression(CHARACTER_LITERAL)
		{}
		~AssistCharLiteral() {}


	};


   }
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistCharLiteral_INCLUDED
