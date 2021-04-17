#ifndef AstPostUnaryExpression_INCLUDED
#define AstPostUnaryExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "PostUnaryExpressionTag.h"

namespace Jikes { // Open namespace Jikes block

	//
	// UnaryExpression --> PreUnaryExpression
	//                   | PostUnaryExpression
	//                   | CastExpression
	//
	// PostUnaryExpression --> <POST_UNARY, PostUnaryTag, Expression, PostOperator>
	//
	// PostUnaryTag --> PLUSPLUS | MINUSMINUS
	//
	// PostOperator --> ++_token | --_token
	//
	class AstPostUnaryExpression : public Expression
	{
	public:
		static AstPostUnaryExpression* PostUnaryExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstPostUnaryExpression*>
				(node->kind == POST_UNARY ? node : NULL);
		}
		

		Expression* expression;
		Token* post_operator_token;

		//
		// When the left-hand side of an assignment is a name that refers
		// to a private field in an enclosing scope, the access method
		// that gives write-permission to that field is recorded here.
		//
		MethodSymbol* write_method;

		AstPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag);

		~AstPostUnaryExpression() {}

		  PostUnaryExpressionEnum::PostUnaryExpressionTag Tag()
		{
			return (PostUnaryExpressionEnum::PostUnaryExpressionTag)other_tag;
		}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return expression->LeftToken(); }
		virtual Token* RightToken() { return post_operator_token; }
	};



} // Close namespace Jikes block


#endif // AstPostUnaryExpression_INCLUDED
