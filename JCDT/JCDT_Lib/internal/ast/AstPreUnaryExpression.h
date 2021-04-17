#ifndef AstPreUnaryExpression_INCLUDED
#define AstPreUnaryExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "PreUnaryExpressionTag.h"

namespace Jikes { // Open namespace Jikes block


	//
	// PreUnaryExpression -->  <PRE_UNARY, PreUnaryTag, PreOperator, Expression>
	//
	// PreUnaryTag --> PLUS | MINUS | TWIDDLE | NOT | PLUSPLUS | MINUSMINUS
	//
	// PreOperator --> +_token | -_token | ~_token | !_token | ++_token | --_token
	//
	class AstPreUnaryExpression : public Expression
	{
	public:
		static AstPreUnaryExpression* PreUnaryExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstPreUnaryExpression*>
				(node->kind == PRE_UNARY ? node : NULL);
		}
	

		Token* pre_operator_token;
		Expression* expression;

		//
		// When the left-hand side of an assignment is a name that refers
		// to a private field in an enclosing scope, the access method
		// that gives write-permission to that field is recorded here.
		//
		MethodSymbol* write_method;

		AstPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag);

		~AstPreUnaryExpression() {}

		  PreUnaryExpressionEnum::PreUnaryExpressionTag Tag()
		{
			return (PreUnaryExpressionEnum::PreUnaryExpressionTag)other_tag;
		}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken() { return pre_operator_token; }
		virtual Token* RightToken() { return expression->RightToken(); }
	};





} // Close namespace Jikes block


#endif // AstPreUnaryExpression_INCLUDED
