#ifndef AstAssertStatement_INCLUDED
#define AstAssertStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"

namespace Jikes { // Open namespace Jikes block



//
// AssertStatement --> <ASSERT, Label_opt, assert_token, Expression, ;_token>
//                 --> <ASSERT, Label_opt, assert_token, Expression, :_token,
// Expression, ;_token>
//
	class AssertStatement : public ReachComleteAbleStatement
	{
	public:
		static AssertStatement* AssertStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<AssertStatement*>(node->kind == ASSERT_KIND ? node : NULL);
		}
		Token* assert_token;
		Token* semicolon_token;
		Expression* condition;
		Expression* message_opt;

		VariableSymbol* assert_variable;

		AssertStatement();

		~AssertStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return assert_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};


} // Close namespace Jikes block


#endif // AstAssertStatement_INCLUDED
