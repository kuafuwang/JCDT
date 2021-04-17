#ifndef AstFieldAccess_INCLUDED
#define AstFieldAccess_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "JCDT_Lib/internal/ast/InvocationSite.h"
namespace Jikes { // Open namespace Jikes block


	//
	// FieldAccess --> <DOT, Primary, ._token, Identifier>
	//
	class FieldReference : public Expression,public InvocationSite
	{
	public:
		static FieldReference* FieldAccessCast(Ast* node)
		{
			return DYNAMIC_CAST<FieldReference*>(node->kind == DOT ? node : NULL);
		}
		Expression* base; // Not AstName.
		Token* identifier_token;

		//
		// If the base expression of FieldAccess expression is of the form
		// type.this.X, where X is a private variable that is a member of an
		// outer class, then we resolve it into a method call to the read_mehod
		// that gives access to X.
		//
		Expression* resolution_opt;

		FieldReference();

		~FieldReference() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return base->LeftToken(); }
		virtual Token* RightToken() { return identifier_token; }

		bool isSuperAccess() override;
		bool isTypeAccess() override;
	};



} // Close namespace Jikes block


#endif // AstFieldAccess_INCLUDED
