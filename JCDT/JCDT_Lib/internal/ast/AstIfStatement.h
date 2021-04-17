#ifndef AstIfStatement_INCLUDED
#define AstIfStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // The parser always makes blocks for the enclosed statements, so we denote
				  // that here (even though any statement is legal).
				  //
	class IfStatement : public ReachComleteAbleStatement
	{
	public:
		static IfStatement* IfStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<IfStatement*>(node->kind == IF ? node : NULL);
		}
		Token* if_token;
		Expression* expression;
		AstBlock* true_statement;
		AstBlock* false_statement_opt;

		IfStatement();

		~IfStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return if_token; }

		virtual Token* RightToken();
	};

	inline IfStatement::IfStatement(): ReachComleteAbleStatement(IF), 
		if_token(nullptr), expression(nullptr), true_statement(nullptr), false_statement_opt(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstIfStatement_INCLUDED
