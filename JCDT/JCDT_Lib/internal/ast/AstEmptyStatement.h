#ifndef AstEmptyStatement_INCLUDED
#define AstEmptyStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block

	//
	// EmptyStatement --> <EMPTY_STATEMENT, Label_opt, ;_token>
	//
	class EmptyStatement : public ReachComleteAbleStatement
	{
	public:
		static  EmptyStatement* EmptyStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<EmptyStatement*>
				(node->kind == EMPTY_STATEMENT ? node : NULL);
		}

		Token* semicolon_token;

		  EmptyStatement(Token* token)
			: ReachComleteAbleStatement(EMPTY_STATEMENT)
			, semicolon_token(token)
		{}
		~EmptyStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return semicolon_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};



} // Close namespace Jikes block


#endif // AstEmptyStatement_INCLUDED
