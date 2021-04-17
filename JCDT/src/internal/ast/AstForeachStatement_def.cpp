#include <JCDT_Lib/internal/ast/AstFormalParameter.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstBlock.h>
#ifndef AstForeachStatement_def_INCLUDED
#define AstForeachStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstForeachStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void ForeachStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ForeachStatement:#" << id << "*/";
    os << lex_stream -> NameString(for_token->index) << " (";
    formal_parameter -> Unparse(os, lex_stream);
    os << " : ";
    expression -> Unparse(os, lex_stream);
    os << ')' << endl;
    statement -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:ForeachStatement#" << id << "*/";
}


Ast* ForeachStatement::Clone(AstStoragePool* ast_pool)
{
    ForeachStatement* clone = ast_pool -> GenForeachStatement();
    clone -> for_token = for_token;
    clone -> formal_parameter =
        (AstFormalParameter*) formal_parameter -> Clone(ast_pool);
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> statement = (AstBlock*) statement -> Clone(ast_pool);
    return clone;
}




void   ForeachStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		formal_parameter->traverse(visitor, scope);
		expression->traverse(visitor, scope);
		statement->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	Token* ForeachStatement::RightToken()
	{ return statement->right_brace_token; }

	ForeachStatement::ForeachStatement(): ReachComleteAbleStatement(FOREACH), for_token(nullptr), formal_parameter(nullptr), expression(nullptr), statement(nullptr)
	{}

	void ForeachStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ForeachStatement):  ("
            << lex_stream.NameString(for_token->index) << "( #"
            << formal_parameter -> id << ": #" << expression -> id
            << ") #" << statement -> id << endl;
    formal_parameter -> Print(os, lex_stream);
    expression -> Print(os, lex_stream);
    statement -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstForeachStatement_def_INCLUDED
