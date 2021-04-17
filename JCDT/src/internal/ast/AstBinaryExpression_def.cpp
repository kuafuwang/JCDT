#ifndef AstBinaryExpression_def_INCLUDED
#define AstBinaryExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstBinaryExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void BinaryExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*BinaryExpression:#" << id << "*/";
    left_expression -> Unparse(os, lex_stream);
    os << ' ' << lex_stream -> NameString(binary_operator_token->index) << ' ';
    right_expression -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:BinaryExpression#" << id << "*/";
}

Ast* BinaryExpression::Clone(AstStoragePool* ast_pool)
{
    BinaryExpression* clone =
        ast_pool -> GenBinaryExpression((BinaryExpressionEnum::BinaryExpressionTag) other_tag);
    clone -> left_expression =
        (Expression*) left_expression -> Clone(ast_pool);
    clone -> binary_operator_token = binary_operator_token;
    clone -> right_expression =
        (Expression*) right_expression -> Clone(ast_pool);
    return clone;
}

	BinaryExpression::BinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag)
		: Expression(BINARY), left_expression(nullptr), binary_operator_token(nullptr), right_expression(nullptr)
	{
		other_tag = tag;
	}

	void BinaryExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (BinaryExpression):  "
            << '#' << left_expression -> id << ' '
            << lex_stream.NameString(binary_operator_token->index)
            << " #" << right_expression -> id << endl;

    left_expression -> Print(os, lex_stream);
    right_expression -> Print(os, lex_stream);
}
void  BinaryExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		left_expression->traverse(visitor,scope);
		right_expression->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}


} // Close namespace Jikes block


#endif // AstBinaryExpression_def_INCLUDED
