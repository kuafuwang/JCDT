#ifndef Jikes_CodeAssist_AssistCastExpression_INCLUDED
#define AssistCastExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

   namespace CodeAssist{
	//
	// AssistCastExpression --> <castkind, (_token_opt, Type, )_token_opt, AssistExpression>
	//
	// NOTE that the optional symbols above are absent only when the compiler
	// inserts a CAST conversion node into the program.
	//
	class AssistCastExpression : public AssistExpression
	{
	public:
		
		
		AstType* type;
		
		AssistExpression* expression;

		  AssistCastExpression()
			: AssistExpression(CAST)
		{}
		~AssistCastExpression() {}


	};

   }
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistCastExpression_INCLUDED
