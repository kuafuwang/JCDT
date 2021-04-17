#ifndef AstClassLiteral_INCLUDED
#define AstClassLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/astExpression.h"
namespace Jikes { // Open namespace Jikes block


	//
	// Represents class literals.
	//
	class ClassLiteralAccess : public Expression
	{
	public:

		static ClassLiteralAccess* ClassLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<ClassLiteralAccess*>
				(node->kind == CLASS_LITERAL ? node : NULL);
		}

		AstType* type;
		Token* class_token;

		//
		// If this expression requires a caching variable and a call to class$(),
		// the resolution holds the needed class$xxx or array$xxx cache.
		//
		Expression* resolution_opt;

		ClassLiteralAccess(Token* token);

		~ClassLiteralAccess() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken() { return class_token; }
	};




} // Close namespace Jikes block


#endif // AstClassLiteral_INCLUDED
