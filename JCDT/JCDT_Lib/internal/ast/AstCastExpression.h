#ifndef AstCastExpression_INCLUDED
#define AstCastExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


	//
	// CastExpression --> <castkind, (_token_opt, Type, )_token_opt, Expression>
	//
	// NOTE that the optional symbols above are absent only when the compiler
	// inserts a CAST conversion node into the program.
	//
	class CastExpression : public Expression
	{
	public:
		static CastExpression* CastExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<CastExpression*>(node->kind == CAST ? node : NULL);
		}

		Token* left_parenthesis_token;
		AstType* type;
		Token* right_parenthesis_token;
		Expression* expression;

		CastExpression();

		~CastExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;
		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return left_parenthesis_token; }
		virtual Token* RightToken() { return expression->RightToken(); }
	};



} // Close namespace Jikes block


#endif // AstCastExpression_INCLUDED
