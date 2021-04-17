#ifndef Jikes_CodeAssist_AssistDoubleLiteral_INCLUDED
#define Jikes_CodeAssist_AssistDoubleLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

	//
	// AssistDoubleLiteral --> <DOUBLE_LITERAL, Literal, value>
	//
	namespace CodeAssist {
		class AssistDoubleLiteral : public AssistExpression
	{
	public:
		

	
		  AssistDoubleLiteral()
			: AssistExpression(DOUBLE_LITERAL)
		{}
		~AssistDoubleLiteral() {}

	};


     }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistDoubleLiteral_INCLUDED
