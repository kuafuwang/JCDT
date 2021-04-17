#ifndef AstThisExpression_INCLUDED
#define AstThisExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // Represents qualified and simple 'this'.
				  //
	class AstThisExpression : public Expression
	{
	public:
		static AstThisExpression* ThisExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstThisExpression*>
				(node->kind == THIS_EXPRESSION ? node : NULL);
		}
		
		virtual bool isThis() {

			return true;
		}
		AstTypeName* base_opt;
		Token* this_token;

		//
		// If this expression accesses an enclosing instance, the resolution
		// holds the needed chain of "this$0" traversals.
		//
		Expression* resolution_opt;

		AstThisExpression(Token* token);

		~AstThisExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken();

		virtual Token* RightToken() { return this_token; }
	};



} // Close namespace Jikes block


#endif // AstThisExpression_INCLUDED
