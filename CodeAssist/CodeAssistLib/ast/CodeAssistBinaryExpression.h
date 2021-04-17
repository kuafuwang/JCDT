#ifndef Jikes_CodeAssist_AstBinaryExpression_INCLUDED
#define Jikes_CodeAssist_AstBinaryExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AssistExpression.h"
#include <JCDT_Lib/internal/ast/BinaryExpressionTag.h>
#include <JCDT_Lib/internal/lex/Token.h>


namespace Jikes { // Open namespace Jikes block

   namespace CodeAssist{

	//
	// AssistBinaryExpression --> <BINARY, BinaryTag, AssistExpression, BinaryOperator,
	//                      AssistExpression>
	//
	// BinaryTag --> STAR | SLASH | MOD | PLUS | MINUS | LEFT_SHIFT | RIGHT_SHIFT |
	//               UNSIGNED_RIGHT_SHIFT | LESS | GREATER |
	//               LESS_EQUAL | GREATER_EQUAL | EQUAL_EQUAL | NOT_EQUAL |
	//               AND | XOR | IOR | AND_AND | OR_OR
	//
	// BinaryOperator --> *_token | /_token | %_token | +_token | -_token |
	//                    <<_token | >>_token | >>>_token | <_token | >_token |
	//                    <=_token | >=_token | ==_token | !=_token | &_token |
	//                    ^_token | |_token | &&_token | ||_token
	//
	class AssistBinaryExpression : public AssistExpression
	{
	public:
	
		AssistExpression* left_expression = nullptr;
		
		AssistExpression* right_expression = nullptr;
		BinaryExpressionEnum::BinaryExpressionTag other_tag ;

		  AssistBinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag)
			: AssistExpression(BINARY), other_tag(tag)
		{
			
		}
		~AssistBinaryExpression() {}

		  BinaryExpressionEnum::BinaryExpressionTag Tag()
		{
			return other_tag;
		}


	};



   }
} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AstBinaryExpression_INCLUDED
