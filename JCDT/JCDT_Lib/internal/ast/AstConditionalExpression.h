#ifndef AstConditionalExpression_INCLUDED
#define AstConditionalExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/astExpression.h"
namespace Jikes { // Open namespace Jikes block

	//
	// ConditionalExpression --> <CONDITIONAL, Expression, ?_token, Expression,
	//                            :_token, Expression>
	//
	class ConditionalExpression : public Expression
	{
	public:
		static ConditionalExpression* ConditionalExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<ConditionalExpression*>
				(node->kind == CONDITIONAL ? node : NULL);
		}
		Expression* test_expression;
		Token* question_token;
		Expression* true_expression;
		Token* colon_token;
		Expression* false_expression;

		ConditionalExpression();

		~ConditionalExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return test_expression->LeftToken(); }
		virtual Token* RightToken()
		{
			return false_expression->RightToken();
		}
	};




} // Close namespace Jikes block


#endif // AstConditionalExpression_INCLUDED
