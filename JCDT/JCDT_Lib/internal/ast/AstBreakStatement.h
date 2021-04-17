#ifndef AstBreakStatement_INCLUDED
#define AstBreakStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


	//
	// BreakStatement --> <BREAK, Label_opt, break_token, identifier_token_opt,
	// ;_token>
	//
	class BreakStatement : public ReachComleteAbleStatement
	{
	public:
		static BreakStatement* BreakStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<BreakStatement*>(node->kind == BREAK ? node : NULL);
		}

		Token* break_token;
		Token* identifier_token_opt;
		Token* semicolon_token;
		unsigned nesting_level;

		BreakStatement();

		~BreakStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return break_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};


} // Close namespace Jikes block


#endif // AstBreakStatement_INCLUDED
