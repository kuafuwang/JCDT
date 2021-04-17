#ifndef AstConditionalExpression_def_INCLUDED
#define AstConditionalExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstConditionalExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void ConditionalExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ConditionalExpression:#" << id << "*/";
    test_expression -> Unparse(os, lex_stream);
    os << " ? ";
    true_expression -> Unparse(os, lex_stream);
    os << " : ";
    false_expression -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:ConditionalExpression#" << id << "*/";
}

Ast* ConditionalExpression::Clone(AstStoragePool* ast_pool)
{
    ConditionalExpression* clone = ast_pool -> GenConditionalExpression();
    clone -> test_expression =
        (Expression*) test_expression -> Clone(ast_pool);
    clone -> question_token = question_token;
    clone -> true_expression =
        (Expression*) true_expression -> Clone(ast_pool);
    clone -> colon_token = colon_token;
    clone -> false_expression =
        (Expression*) false_expression -> Clone(ast_pool);
    return clone;
}

	ConditionalExpression::ConditionalExpression(): Expression(CONDITIONAL), test_expression(nullptr), question_token(nullptr), true_expression(nullptr), colon_token(nullptr), false_expression(nullptr)
	{}

	void ConditionalExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ConditionalExpression):  "
            << '#' << test_expression -> id
            << " ? #" << true_expression -> id
            << " : #" << false_expression -> id << endl;

    test_expression -> Print(os, lex_stream);
    true_expression -> Print(os, lex_stream);
    false_expression -> Print(os, lex_stream);
}
void  ConditionalExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		test_expression->traverse(visitor, scope);
		true_expression->traverse(visitor, scope);
		false_expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}



} // Close namespace Jikes block


#endif // AstConditionalExpression_def_INCLUDED
