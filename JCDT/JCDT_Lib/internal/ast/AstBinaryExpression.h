#ifndef AstBinaryExpression_INCLUDED
#define AstBinaryExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "BinaryExpressionTag.h"

namespace Jikes { // Open namespace Jikes block



	//
	// BinaryExpression --> <BINARY, BinaryTag, Expression, BinaryOperator,
	//                      Expression>
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
	class BinaryExpression : public Expression
	{
	public:
		static BinaryExpression* BinaryExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<BinaryExpression*>(node->kind == BINARY ? node : NULL);
		}

		Expression* left_expression;
		Token* binary_operator_token;
		Expression* right_expression;

		BinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag);

		~BinaryExpression() {}

		  BinaryExpressionEnum::BinaryExpressionTag Tag()
		{
			return (BinaryExpressionEnum::BinaryExpressionTag)other_tag;
		}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return left_expression->LeftToken(); }
		virtual Token* RightToken()
		{
			return right_expression->RightToken();
		}
		

	};




} // Close namespace Jikes block


#endif // AstBinaryExpression_INCLUDED
