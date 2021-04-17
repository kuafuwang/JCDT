#ifndef AstInstanceofExpression_INCLUDED
#define AstInstanceofExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // Represents instanceof expressions.
				  //
	class InstanceofExpression : public Expression
	{
	public:

		static InstanceofExpression* InstanceofExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<InstanceofExpression*>
				(node->kind == INSTANCEOF ? node : NULL);
		}
		Expression* expression;
		Token* instanceof_token;
		AstType* type; // AstArrayType, AstTypeName

		InstanceofExpression();

		~InstanceofExpression() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken() { return expression->LeftToken(); }

		virtual Token* RightToken();
	};

	inline InstanceofExpression::InstanceofExpression(): Expression(INSTANCEOF),
		expression(nullptr), instanceof_token(nullptr), type(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstInstanceofExpression_INCLUDED
