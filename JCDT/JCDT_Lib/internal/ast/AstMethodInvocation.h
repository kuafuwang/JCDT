#ifndef AstMethodInvocation_INCLUDED
#define AstMethodInvocation_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "InvocationSite.h"

namespace Jikes { // Open namespace Jikes block
	// Represents a method call.  Sometimes, during semantic analysis an
	// artificial base_opt expression is constructed. In such a case, the user
	// can determine this condition by testing base_opt -> generated.
	//
	class AstMethodInvocation : public Expression,public InvocationSite
	{
	public:

		Expression* base_opt;
		AstTypeArguments* type_arguments_opt;
		Token* identifier_token;
		AstArguments* arguments;
		//
		// When a method refers to a member in an enclosing scope,
		// it is mapped into a new expression that creates a path to
		// the member in question.
		//
		Expression* resolution_opt;

		
		static AstMethodInvocation* MethodInvocationCast(Ast* node){
			return DYNAMIC_CAST<AstMethodInvocation*>(node->kind == CALL ? node : NULL);
		}
	
		

		TypeSymbol*	receiverType(){
			if (!base_opt)
				return nullptr;
			return  base_opt->Type();
			
		}
		

		AstMethodInvocation(Token* t);

		~AstMethodInvocation() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken();
		bool isTypeAccess() override;
		bool isSuperAccess() override;
	};



} // Close namespace Jikes block


#endif // AstMethodInvocation_INCLUDED

