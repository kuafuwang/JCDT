#ifndef AstDoStatement_INCLUDED
#define AstDoStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block

	//
	// DoStatement --> <DO, Label_opt, do_token, Expression, Statement, ;_token>
	//
	class DoStatement : public ReachComleteAbleStatement
	{
	public:
		static DoStatement* DoStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<DoStatement*>(node->kind == DO ? node : NULL);
		}
		Token* do_token;
		AstBlock* statement;
		Token* while_token;
		Expression* expression;
		Token* semicolon_token;

		DoStatement();

		~DoStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken() { return do_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};



} // Close namespace Jikes block


#endif // AstDoStatement_INCLUDED
