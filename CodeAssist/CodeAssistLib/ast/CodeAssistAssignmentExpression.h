#ifndef Jikes_CodeAssist_AssistAstAssignmentExpression_INCLUDED
#define Jikes_CodeAssist_AssistAstAssignmentExpression_INCLUDED
#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/ast/AssignmentExpressionTag.h>


namespace Jikes { // Open namespace Jikes block

	//
	// Assignment --> <ASSIGNMENT, AssignmentTag, LeftHandSide, AssignmentOperator,
	//                AssistExpression>
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
       namespace CodeAssist{
	class AssistAstAssignmentExpression : public AssistExpression
	{
	public:
		

		
		AssistExpression* left_hand_side;
	
		AssistExpression* expression;
		AssignmentExpressionEnum::AssignmentExpressionTag other_tag;

		  AssistAstAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag)
			: AssistExpression(ASSIGNMENT)
		{
			other_tag = tag;
		}
		~AssistAstAssignmentExpression() {}

		  AssignmentExpressionEnum::AssignmentExpressionTag Tag()
		{
			return other_tag;
		}
		  bool SimpleAssignment() { return other_tag == AssignmentExpressionEnum::SIMPLE_EQUAL; }


	};


       }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstAssignmentExpression_INCLUDED
