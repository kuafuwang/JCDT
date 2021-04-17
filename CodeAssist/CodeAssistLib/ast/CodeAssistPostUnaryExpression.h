#ifndef Jikes_CodeAssist_AssistAstPostUnaryExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstPostUnaryExpression_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/ast/PostUnaryExpressionTag.h>

namespace Jikes { // Open namespace Jikes block
 namespace CodeAssist{
	//
	// UnaryExpression --> PreUnaryExpression
	//                   | PostUnaryExpression
	//                   | CastExpression
	//
	// PostUnaryExpression --> <POST_UNARY, PostUnaryTag, AssistExpression, PostOperator>
	//
	// PostUnaryTag --> PLUSPLUS | MINUSMINUS
	//
	// PostOperator --> ++_token | --_token
	//
	class AssistAstPostUnaryExpression : public AssistExpression
	{
	public:
	

		AssistExpression* expression;
	
		unsigned other_tag : 8;


		  AssistAstPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag)
			: AssistExpression(POST_UNARY)
		{
			other_tag = tag;
		}
		~AssistAstPostUnaryExpression() {}

		  PostUnaryExpressionEnum::PostUnaryExpressionTag Tag()
		{
			return (PostUnaryExpressionEnum::PostUnaryExpressionTag)other_tag;
		}


	};

 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstPostUnaryExpression_INCLUDED
