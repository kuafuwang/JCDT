#ifndef AstWhileStatement_INCLUDED
#define AstWhileStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // WhileStatement --> <WHILE, Label_opt, while_token, Expression, Statement>
				  //
	class WhileStatement : public ReachComleteAbleStatement
	{
	public:
		static  WhileStatement* WhileStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<WhileStatement*>(node->kind == WHILE ? node : NULL);
		}
		Token* while_token;
		Expression* expression;
		AstBlock* statement;

		  WhileStatement()
			: ReachComleteAbleStatement(WHILE), 
			  while_token(nullptr),
			  expression(nullptr),
			  statement(nullptr)
		{}
		~WhileStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return while_token; }

		virtual Token* RightToken();
	};




} // Close namespace Jikes block


#endif // AstWhileStatement_INCLUDED
