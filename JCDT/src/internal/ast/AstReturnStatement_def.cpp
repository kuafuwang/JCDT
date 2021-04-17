#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstReturnStatement_def_INCLUDED
#define AstReturnStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstReturnStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void ReturnStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ReturnStatement:#" << id << "*/";
    if (! generated)
    {
        os << lex_stream -> NameString(return_token->index);
        if (expression_opt)
        {
            os << ' ';
            expression_opt -> Unparse(os, lex_stream);
        }
        os << ';' << endl;
    }
    if (debug_unparse)
        os << "/*:ReturnStatement#" << id << "*/";
}

Ast* ReturnStatement::Clone(AstStoragePool* ast_pool)
{
    ReturnStatement* clone = ast_pool -> GenReturnStatement();
    clone -> return_token = return_token;
    if (expression_opt)
        clone -> expression_opt =
            (Expression*) expression_opt -> Clone(ast_pool);
    clone -> semicolon_token = semicolon_token;
    return clone;
}



void   ReturnStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		if (expression_opt)
			expression_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	ReturnStatement::ReturnStatement(): ReachComleteAbleStatement(RETURN), return_token(nullptr), expression_opt(nullptr), semicolon_token(nullptr)
	{}

	void ReturnStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ReturnStatement):  "
            << lex_stream.NameString(return_token->index)
            << ' '
            << " #" << (expression_opt ? expression_opt -> id : 0) << endl;
    if (expression_opt)
        expression_opt -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstReturnStatement_def_INCLUDED
