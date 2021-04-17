#ifndef AstDimExpr_INCLUDED
#define AstDimExpr_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block

	//
	// DimExpr --> <DIM, [_token, Expression, ]_token>
	//
	class AstDimExpr : public Ast
	{
	public:

		static AstDimExpr* DimExprCast(Ast* node)
		{
			return DYNAMIC_CAST<AstDimExpr*>(node->kind == DIM ? node : NULL);
		}
		Token* left_bracket_token;
		Expression* expression;
		Token* right_bracket_token;

		  AstDimExpr()
			: Ast(DIM), left_bracket_token(nullptr), expression(nullptr), right_bracket_token(nullptr)
		{}
		~AstDimExpr() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return left_bracket_token; }
		virtual Token* RightToken() { return right_bracket_token; }
	};



} // Close namespace Jikes block


#endif // AstDimExpr_INCLUDED
