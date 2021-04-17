#ifndef AstReturnStatement_INCLUDED
#define AstReturnStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // ReturnStatement --> <RETURN, Label_opt, return_token, Expression_opt,
				  // ;_token>
				  //
	class ReturnStatement : public ReachComleteAbleStatement
	{
	public:
		static ReturnStatement* ReturnStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<ReturnStatement*>(node->kind == RETURN ? node : NULL);
		}
		Token* return_token;
		Expression* expression_opt;
		Token* semicolon_token;

		ReturnStatement();

		~ReturnStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return return_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};



} // Close namespace Jikes block


#endif // AstReturnStatement_INCLUDED
