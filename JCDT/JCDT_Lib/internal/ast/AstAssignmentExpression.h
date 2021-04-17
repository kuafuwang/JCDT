#ifndef AstAssignmentExpression_INCLUDED
#define AstAssignmentExpression_INCLUDED
#include "JCDT_Lib/internal/ast/AstExpression.h"
#include "AssignmentExpressionTag.h"

namespace Jikes { // Open namespace Jikes block

	//
	// Assignment --> <ASSIGNMENT, AssignmentTag, LeftHandSide, AssignmentOperator,
	//                Expression>
	//
	// AssignmentTag --> EQUAL | STAR_EQUAL | SLASH_EQUAL | MOD_EQUAL |
	//                   PLUS_EQUAL | MINUS_EQUAL | LEFT_SHIFT_EQUAL |
	//                   RIGHT_SHIFT_EQUAL | UNSIGNED_RIGHT_SHIFT_EQUAL |
	//                   AND_EQUAL | XOR_EQUAL | IOR_EQUAL
	//
	// LeftHandSide --> Name | FieldAccess | ArrayAccess | ParenthesizedExpression
	//                  | CastExpression
	//
	// NOTE: that a LeftHandSide appears as a cast node only when the
	// assignment_operator in question is of the form "op=" and the application
	// of the operator requires a casting of the value of the left-hand side.
	//
	// AssignmentOperator --> =_token | *=_token | /=_token | %=_token | +=_token |
	//                        -=_token | <<=_token | >>=_token | >>>=_token |
	//                        &=_token | ^=_token | |=_token
	//
	class AstAssignmentExpression : public Expression
	{
	public:
		static  AstAssignmentExpression* AssignmentExpressionCast(Ast* node)
		{
			return DYNAMIC_CAST<AstAssignmentExpression*>
				(node->kind == ASSIGNMENT ? node : NULL);
		}
		

		//
		// When the left-hand side of an assignment is a name that refers
		// to a private field in an enclosing scope, the access method
		// that gives write-permission to that field is recorded here.
		//
		MethodSymbol* write_method;

		Expression* left_hand_side;
		Token* assignment_operator_token;
		Expression* expression;

		AstAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag, Token* t);

		~AstAssignmentExpression() {}

		  AssignmentExpressionEnum::AssignmentExpressionTag Tag()
		{
			return (AssignmentExpressionEnum::AssignmentExpressionTag)other_tag;
		}
		  bool SimpleAssignment() { return other_tag == AssignmentExpressionEnum::SIMPLE_EQUAL; }

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken()
		{
			return left_hand_side->LeftToken();
		}
		virtual Token* RightToken()
		{
			return expression->RightToken();
		}
	};




} // Close namespace Jikes block


#endif // AstAssignmentExpression_INCLUDED
