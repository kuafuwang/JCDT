#ifndef AstContinueStatement_INCLUDED
#define AstContinueStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block

	//
	// ContinueStatement --> <CONTINUE, Label_opt, continue_token, SimpleName_opt,
	// ;_token>
	//
	class ContinueStatement : public ReachComleteAbleStatement
	{
	public:
		static ContinueStatement* ContinueStatementCast(Ast*node)
		{
			return DYNAMIC_CAST<ContinueStatement*>
				(node->kind == CONTINUE ? node : NULL);
		}
		Token* continue_token;
		Token* identifier_token_opt;
		Token* semicolon_token;
		unsigned nesting_level;

		ContinueStatement();

		~ContinueStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 

		virtual Token* LeftToken() { return continue_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};



} // Close namespace Jikes block


#endif // AstContinueStatement_INCLUDED
