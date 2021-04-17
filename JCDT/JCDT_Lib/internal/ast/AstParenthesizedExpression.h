#ifndef AstParenthesizedExpression_INCLUDED
#define AstParenthesizedExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // ParenthesizedExpression --> <PARENTHESIZED_EXPRESSION, (_token, Expression,
				  // )_token>
				  //
	class AstParenthesizedExpression : public Expression
	{
	public:
		static AstParenthesizedExpression* ParenthesizedExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstParenthesizedExpression*>
				(node->kind == PARENTHESIZED_EXPRESSION ? node : NULL);
		}
		Token* left_parenthesis_token;
		Expression* expression;
		Token* right_parenthesis_token;

		AstParenthesizedExpression();

		~AstParenthesizedExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return left_parenthesis_token; }
		virtual Token* RightToken() { return right_parenthesis_token; }
	};



} // Close namespace Jikes block


#endif // AstParenthesizedExpression_INCLUDED
