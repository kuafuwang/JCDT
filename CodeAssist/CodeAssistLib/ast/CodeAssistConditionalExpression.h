#ifndef Jikes_CodeAssist_AssistConditionalExpression_INCLUDED
#define Jikes_CodeAssist_AssistConditionalExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block

	//
	// AssistConditionalExpression --> <CONDITIONAL, AssistExpression, ?_token, AssistExpression,
	//                            :_token, AssistExpression>
	//
     namespace CodeAssist{
	class AssistConditionalExpression : public AssistExpression
	{
	public:
		
		AssistExpression* test_expression = nullptr;
	
		AssistExpression* true_expression = nullptr;
	
		AssistExpression* false_expression = nullptr;

		  AssistConditionalExpression()
			: AssistExpression(CONDITIONAL)
		{}
		~AssistConditionalExpression() {}


	};

     }


} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistConditionalExpression_INCLUDED
