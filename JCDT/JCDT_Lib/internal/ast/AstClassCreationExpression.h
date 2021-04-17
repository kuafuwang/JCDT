#ifndef AstClassCreationExpression_INCLUDED
#define AstClassCreationExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "InvocationSite.h"

namespace Jikes { // Open namespace Jikes block


	//
	// ClassCreationExpression represents a class instance creation (keyword new,
	// including anonymous classes). Also see ArrayCreationExpression. Sometimes,
	// during semantic analysis an artificial base_opt expression is constructed.
	// In such a case, the user can determine this condition by testing
	// base_opt -> generated.
	//
	class AstClassCreationExpression : public Expression,public InvocationSite
	{
	public:
		static AstClassCreationExpression* ClassCreationExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstClassCreationExpression*>
				(node->kind == CLASS_CREATION ? node : NULL);
		}
		Expression* base_opt;
		Token* new_token;
		AstTypeArguments* type_arguments_opt;
		AstTypeName* class_type;
		AstArguments* arguments;
		AstAnonymousTypeDeclaration* anonymous_type_opt;

		//
		// For anonymous classes, we resolve the original statement into a new
		// one that does not have a class_body_opt. This is necessary to get
		// the parameters called in the correct order.
		//
		AstClassCreationExpression* resolution_opt;

		AstClassCreationExpression();

		~AstClassCreationExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken()
		{
			return base_opt ? base_opt->LeftToken() : new_token;
		}

		virtual Token* RightToken();
		bool isSuperAccess() override;
		bool isTypeAccess() override;
	};




} // Close namespace Jikes block


#endif // AstClassCreationExpression_INCLUDED
