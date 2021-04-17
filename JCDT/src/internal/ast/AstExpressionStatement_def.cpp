
#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstExpressionStatement_def_INCLUDED
#define AstExpressionStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstExpressionStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstExpressionStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstExpressionStatement:#" << id << "*/";
    expression -> Unparse(os, lex_stream);
    if (semicolon_token_opt)
        os << ';' << endl;
    if (debug_unparse)
        os << "/*:AstExpressionStatement#" << id << "*/";
}
Ast* AstExpressionStatement::Clone(AstStoragePool* ast_pool)
{
    AstExpressionStatement* clone = ast_pool -> GenExpressionStatement();
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> semicolon_token_opt = semicolon_token_opt;
    return clone;
}



	Token* AstExpressionStatement::LeftToken()
	{ return expression->LeftToken(); }

	Token* AstExpressionStatement::RightToken()
	{
		return semicolon_token_opt ? semicolon_token_opt
			       : expression->RightToken();
	}

	void   AstExpressionStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstExpressionStatement::AstExpressionStatement(): ReachComleteAbleStatement(EXPRESSION_STATEMENT), expression(nullptr), semicolon_token_opt(nullptr)
	{}

	void AstExpressionStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ExpressionStatement):  #" << expression -> id
            << endl;
    expression -> Print(os, lex_stream);
}




} // Close namespace Jikes block


#endif // AstExpressionStatement_def_INCLUDED
