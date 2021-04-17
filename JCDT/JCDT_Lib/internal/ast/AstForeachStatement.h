#ifndef AstForeachStatement_INCLUDED
#define AstForeachStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block

	//
	// ForeachStatement is added in JDK 1.5 by JSR 201.  It has the syntax
	// "for (FormalParameter : expression) statement", where expression must
	// be an array type or an instance of java.lang.Iterable. The parser already
	// wrapped the statement in a block.
	//
	class ForeachStatement : public ReachComleteAbleStatement
	{
	public:
		static ForeachStatement* ForeachStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<ForeachStatement*>(node->kind == FOREACH ? node : NULL);
		}
		Token* for_token;
		AstFormalParameter* formal_parameter;
		Expression* expression;
		AstBlock* statement;

		ForeachStatement();

		~ForeachStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return for_token; }

		virtual Token* RightToken();
	};


} // Close namespace Jikes block


#endif // AstForeachStatement_INCLUDED
