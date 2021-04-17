#include <JCDT_Lib/internal/ast/AstBlock.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstDoStatement_def_INCLUDED
#define AstDoStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstDoStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void DoStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*DoStatement:#" << id << "*/";
    os << lex_stream -> NameString(do_token->index) << endl;
    statement -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(while_token->index) << " (";
    expression -> Unparse(os, lex_stream);
    os << ')' << lex_stream -> NameString(semicolon_token->index) << endl;
    if (debug_unparse)
        os << "/*:DoStatement#" << id << "*/";
}

Ast* DoStatement::Clone(AstStoragePool* ast_pool)
{
    DoStatement* clone = ast_pool -> GenDoStatement();
    clone -> do_token = do_token;
    clone -> statement = (AstBlock*) statement -> Clone(ast_pool);
    clone -> while_token = while_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> semicolon_token = semicolon_token;
    return clone;
}



void   DoStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		statement->traverse(visitor, scope);
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	DoStatement::DoStatement(): ReachComleteAbleStatement(DO), do_token(nullptr), statement(nullptr), while_token(nullptr), expression(nullptr), semicolon_token(nullptr)
	{}

	void DoStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (DoStatement):  "
            << lex_stream.NameString(do_token->index)
            << " { #" << statement -> id << " } "
            << lex_stream.NameString(while_token->index)
            << " ( #" << expression -> id << " ) #" << endl;

    statement -> Print(os, lex_stream);
    expression -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstDoStatement_def_INCLUDED
