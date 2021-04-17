#ifndef AstFinallyClause_INCLUDED
#define AstFinallyClause_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block



	//
	// FinallyClause --> <FINALLY, finally_token, Block>
	//
	class AstFinallyClause : public Ast
	{
	public:
		static AstFinallyClause* FinallyClauseCast(Ast* node)
		{
			return DYNAMIC_CAST<AstFinallyClause*>(node->kind == FINALLY ? node : NULL);
		}
		Token* finally_token;
		AstBlock* block;

		AstFinallyClause();

		~AstFinallyClause() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return finally_token; }

		virtual Token* RightToken();
	};




} // Close namespace Jikes block


#endif // AstFinallyClause_INCLUDED
