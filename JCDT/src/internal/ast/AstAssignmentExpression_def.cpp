#ifndef AstAssignmentExpression_def_INCLUDED
#define AstAssignmentExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstAssignmentExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstAssignmentExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstAssignmentExpression:#" << id << "*/";
    left_hand_side -> Unparse(os, lex_stream);
    os << ' ' << lex_stream -> NameString(assignment_operator_token->index) << ' ';
    expression -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstAssignmentExpression#" << id << "*/";
}

Ast* AstAssignmentExpression::Clone(AstStoragePool* ast_pool)
{
    AstAssignmentExpression* clone = ast_pool ->
        GenAssignmentExpression((AssignmentExpressionEnum::AssignmentExpressionTag) other_tag,
                                assignment_operator_token);
    clone -> left_hand_side =
        (Expression*) left_hand_side -> Clone(ast_pool);
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    return clone;
}

	AstAssignmentExpression::AstAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag, Token* t)
		: Expression(ASSIGNMENT), write_method(nullptr), left_hand_side(nullptr)
		  , assignment_operator_token(t), expression(nullptr)
	{
		other_tag = tag;
	}

	void AstAssignmentExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (AssignmentExpression):  "
            << '#' << left_hand_side -> id << ' '
            << lex_stream.NameString(assignment_operator_token->index)
            << " #" << expression -> id << endl;

    left_hand_side -> Print(os, lex_stream);
    expression -> Print(os, lex_stream);
}
void  AstAssignmentExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		left_hand_side->traverse(visitor,scope);
		expression->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}

} // Close namespace Jikes block


#endif // AstAssignmentExpression_def_INCLUDED
