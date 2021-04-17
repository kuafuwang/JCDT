#ifndef AstExpression_INCLUDED
#define AstExpression_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstMemberValue.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // This is the superclass of constructs which represent an expression.
				  //
	class Expression : public AstMemberValue
	{
	public:
		LiteralValue* value; // The compile-time constant value of the expression.

		static Expression* ExpressionCast(Ast *node)
		{
			return DYNAMIC_CAST<Expression*>
				(node->class_tag == EXPRESSION ? node : NULL);
		}

		  Expression(AstKind k)
			: AstMemberValue(k, EXPRESSION), value(nullptr)
		{
			
		}
		~Expression()
		{
			
		}
		virtual bool isTypeReference() {
			return false;
		}
		  bool IsConstant()
		{
			return value != NULL;
		}
		 virtual  bool isSuper() {

			  return false;
		  }

		 virtual bool isThis() {

			  return false;
		  }
	};


} // Close namespace Jikes block


#endif // AstExpression_INCLUDED
