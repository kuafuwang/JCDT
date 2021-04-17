#ifndef AstSuperExpression_INCLUDED
#define AstSuperExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // Represents qualified and simple 'super'.
				  //
	class AstSuperExpression : public Expression
	{
	public:
		static AstSuperExpression* SuperExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstSuperExpression*>
				(node->kind == SUPER_EXPRESSION ? node : NULL);
		}
		AstTypeName* base_opt;
		Token* super_token;
		virtual  bool isSuper() {

			return true;
		}

		
		//
		// If this expression accesses an enclosing instance, the resolution
		// holds the needed chain of "this$0" traversals.
		//
		Expression* resolution_opt;

		AstSuperExpression(Token* token);

		~AstSuperExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken();

		virtual Token* RightToken() { return super_token; }
	};


} // Close namespace Jikes block


#endif // AstSuperExpression_INCLUDED
