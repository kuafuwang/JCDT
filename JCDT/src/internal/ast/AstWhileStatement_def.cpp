#include <JCDT_Lib/internal/ast/AstBlock.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstWhileStatement_def_INCLUDED
#define AstWhileStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstWhileStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void WhileStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*WhileStatement:#" << id << "*/";
    os << lex_stream -> NameString(while_token->index) << " (";
    expression -> Unparse(os, lex_stream);
    os << ')' << endl;
    statement -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:WhileStatement#" << id << "*/";
}
Ast* WhileStatement::Clone(AstStoragePool* ast_pool)
{
    WhileStatement* clone = ast_pool -> GenWhileStatement();
    clone -> while_token = while_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> statement = (AstBlock*) statement -> Clone(ast_pool);
    return clone;
}


	Token* WhileStatement::RightToken()
	{ return statement->right_brace_token; }

	void   WhileStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		expression->traverse(visitor, scope);
		statement->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void WhileStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (WhileStatement):  "
            << lex_stream.NameString(while_token->index)
            << " ( #" << expression -> id << " ) #" << statement -> id << endl;
    expression -> Print(os, lex_stream);
    statement -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstWhileStatement_def_INCLUDED
