#ifndef AstParenthesizedExpression_def_INCLUDED
#define AstParenthesizedExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstParenthesizedExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void AstParenthesizedExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstParenthesizedExpression:#" << id << "*/";
    os << lex_stream -> NameString(left_parenthesis_token->index);
    expression -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(right_parenthesis_token->index);
    if (debug_unparse)
        os << "/*:AstParenthesizedExpression#" << id << "*/";
}

Ast* AstParenthesizedExpression::Clone(AstStoragePool* ast_pool)
{
    AstParenthesizedExpression* clone =
        ast_pool -> GenParenthesizedExpression();
    clone -> left_parenthesis_token = left_parenthesis_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> right_parenthesis_token = right_parenthesis_token;
    return clone;
}





void   AstParenthesizedExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstParenthesizedExpression::AstParenthesizedExpression(): Expression(PARENTHESIZED_EXPRESSION), left_parenthesis_token(nullptr), expression(nullptr), right_parenthesis_token(nullptr)
	{}

	void AstParenthesizedExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ParenthesizedExpression):  "
            << lex_stream.NameString(left_parenthesis_token->index)
            << '#' << expression -> id
            << lex_stream.NameString(right_parenthesis_token->index)
            << endl;
    expression -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstParenthesizedExpression_def_INCLUDED
