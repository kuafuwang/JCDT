#ifndef Jikes_CodeAssist_AssistAstPreUnaryExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstPreUnaryExpression_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/ast/PreUnaryExpressionTag.h>


namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
	//
	// PreUnaryExpression -->  <PRE_UNARY, PreUnaryTag, PreOperator, AssistExpression>
	//
	// PreUnaryTag --> PLUS | MINUS | TWIDDLE | NOT | PLUSPLUS | MINUSMINUS
	//
	// PreOperator --> +_token | -_token | ~_token | !_token | ++_token | --_token
	//
	class AssistAstPreUnaryExpression : public AssistExpression
	{
	public:
		
		AssistExpression* expression;

		PreUnaryExpressionEnum::PreUnaryExpressionTag other_tag ;


		
		  AssistAstPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag)
			: AssistExpression(PRE_UNARY)
		{
			other_tag = tag;
		}
		~AssistAstPreUnaryExpression() {}

		  PreUnaryExpressionEnum::PreUnaryExpressionTag Tag()
		{
			return other_tag;
		}


	};



 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstPreUnaryExpression_INCLUDED
