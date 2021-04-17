#ifndef AstCatchClause_INCLUDED
#define AstCatchClause_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block


	//
	// CatchClause --> <CATCH, catch_token, FormalParameter, Block>
	//
	class AstCatchClause : public Ast
	{
	public:
		static AstCatchClause* CatchClauseCast(Ast* node)
		{
			return DYNAMIC_CAST<AstCatchClause*>(node->kind == CATCH ? node : NULL);
		}
		VariableSymbol* parameter_symbol;

		Token* catch_token;
		AstFormalParameter* formal_parameter;
		AstBlock* block;

		AstCatchClause();

		~AstCatchClause() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return catch_token; }

		virtual Token* RightToken();
	};




} // Close namespace Jikes block


#endif // AstCatchClause_INCLUDED
