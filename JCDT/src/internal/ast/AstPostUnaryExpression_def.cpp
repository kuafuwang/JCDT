#ifndef AstPostUnaryExpression_def_INCLUDED
#define AstPostUnaryExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstPostUnaryExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void AstPostUnaryExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstPostUnaryExpression:#" << id << "*/";
    expression -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(post_operator_token->index);
    if (debug_unparse)
        os << "/*:AstPostUnaryExpression#" << id << "*/";
}

Ast* AstPostUnaryExpression::Clone(AstStoragePool* ast_pool)
{
    AstPostUnaryExpression* clone =
        ast_pool -> GenPostUnaryExpression((PostUnaryExpressionEnum::PostUnaryExpressionTag) other_tag);
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> post_operator_token = post_operator_token;
    return clone;
}



void   AstPostUnaryExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstPostUnaryExpression::AstPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag)
		: Expression(POST_UNARY), expression(nullptr),
		post_operator_token(nullptr), write_method(nullptr)
	{
		other_tag = tag;
	}

	void AstPostUnaryExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (PostUnaryExpression):  "
            << '#' << expression -> id
            << lex_stream.NameString(post_operator_token->index)
            << endl;

	expression->Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstPostUnaryExpression_def_INCLUDED
