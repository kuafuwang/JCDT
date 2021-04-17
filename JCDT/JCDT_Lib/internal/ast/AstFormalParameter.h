#ifndef AstFormalParameter_INCLUDED
#define AstFormalParameter_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block

				  //
				  // FormalParameter --> <PARAMETER, Type, VariableDeclaratorId>
				  //
	class AstFormalParameter : public Ast
	{
	public:
		AstModifiers* modifiers_opt;
		AstType* type;
		Token* ellipsis_token_opt;
		AstVariableDeclarator* formal_declarator;
		static AstFormalParameter* FormalParameterCast(Ast* node)
		{
			return DYNAMIC_CAST<AstFormalParameter*>(node->kind == PARAMETER ? node : NULL);
		}

		AstFormalParameter();

		~AstFormalParameter() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken();
	};



} // Close namespace Jikes block


#endif // AstFormalParameter_INCLUDED
