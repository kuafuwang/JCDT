#ifndef AstVariableDeclarator_INCLUDED
#define AstVariableDeclarator_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


	//
	// VariableDeclarator --> <VARIABLE_DECLARATOR, VariableDeclaratorId,
	//     VariableInitializer_opt>
	//
	// Technically, this is not a statement. But it is similar to local variable
	// declarations, which are, and treating it as a statement makes compiling
	// initializer blocks more uniform.
	//
	class AstVariableDeclarator : public ReachComleteAbleStatement
	{
	public:
		VariableSymbol* symbol;

		// when true, this variable signals that the variable_initializer_opt
		// for this variable is currently being evaluated
		bool pending;
		wstring getName();
		wstring getTypeName() const;
		static AstVariableDeclarator* VariableDeclaratorCast(Ast* node)
		{
			return DYNAMIC_CAST<AstVariableDeclarator*>
				(node->kind == VARIABLE_DECLARATOR ? node : NULL);
		}
		Ast* owner;
		bool isField() const {
			return  owner &&  owner->kind == FIELD;
		}
		AstVariableDeclaratorId* variable_declarator_name;
		Ast* variable_initializer_opt;
		AstType* Type();
		AstVariableDeclarator();

		~AstVariableDeclarator() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		 virtual Token* LeftToken();
		 virtual Token* RightToken();
	};




} // Close namespace Jikes block


#endif // AstVariableDeclarator_INCLUDED
