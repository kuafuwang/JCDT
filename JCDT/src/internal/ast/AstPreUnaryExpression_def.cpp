#ifndef AstPreUnaryExpression_def_INCLUDED
#define AstPreUnaryExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstPreUnaryExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block




void AstPreUnaryExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstPreUnaryExpression:#" << id << "*/";
    os << lex_stream -> NameString(pre_operator_token->index);
    expression -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstPreUnaryExpression#" << id << "*/";
}


void   AstPreUnaryExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstPreUnaryExpression::AstPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag)
		: Expression(PRE_UNARY), pre_operator_token(nullptr), expression(nullptr), write_method(nullptr)
	{
		other_tag = tag;
	}

	void AstPreUnaryExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (PreUnaryExpression):  "
            << lex_stream.NameString(pre_operator_token->index)
            << " #" << expression -> id << endl;

    expression -> Print(os, lex_stream);
}

Ast* AstPreUnaryExpression::Clone(AstStoragePool* ast_pool)
{
    AstPreUnaryExpression* clone =
        ast_pool -> GenPreUnaryExpression((PreUnaryExpressionEnum::PreUnaryExpressionTag) other_tag);
    clone -> pre_operator_token = pre_operator_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    return clone;
}

} // Close namespace Jikes block


#endif // AstPreUnaryExpression_def_INCLUDED
