#ifndef AstExpressionStatement_INCLUDED
#define AstExpressionStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block

				  //
				  // ExpressionStatement --> <EXPRESSION_STATEMENT, Label_opt, Expression,
				  // ;_token_opt>
				  //
	class AstExpressionStatement : public ReachComleteAbleStatement
	{
	public:
		static AstExpressionStatement* ExpressionStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<AstExpressionStatement*>
				(node->kind == EXPRESSION_STATEMENT ? node : NULL);
		}
		Expression* expression;
		Token* semicolon_token_opt;

		AstExpressionStatement();

		~AstExpressionStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken();

		virtual Token* RightToken();
	};



} // Close namespace Jikes block


#endif // AstExpressionStatement_INCLUDED
