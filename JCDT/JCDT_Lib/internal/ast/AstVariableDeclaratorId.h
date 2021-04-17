#ifndef AstVariableDeclaratorId_INCLUDED
#define AstVariableDeclaratorId_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // VariableDeclaratorId --> <VARIABLE_DECLARATOR_NAME, identifier_token,
				  // Brackets>
				  //
	class AstVariableDeclaratorId : public Ast
	{
	public:
		Token* identifier_token;
		AstBrackets* brackets_opt;
		static AstVariableDeclaratorId* VariableDeclaratorIdCast(Ast* node)
		{
			return DYNAMIC_CAST<AstVariableDeclaratorId*>
				(node->kind == VARIABLE_DECLARATOR_NAME ? node : NULL);
		}
		  AstVariableDeclaratorId()
			: Ast(VARIABLE_DECLARATOR_NAME), identifier_token(nullptr), brackets_opt(nullptr)
		{}
		~AstVariableDeclaratorId() {}

		  unsigned NumBrackets();

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return identifier_token; }

		virtual Token* RightToken();
	};






} // Close namespace Jikes block


#endif // AstVariableDeclaratorId_INCLUDED
