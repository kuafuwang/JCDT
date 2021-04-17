#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstThrowStatement_def_INCLUDED
#define AstThrowStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstThrowStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block




void ThrowStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ThrowStatement:#" << id << "*/";
    os << lex_stream -> NameString(throw_token->index) << ' ';
    expression -> Unparse(os, lex_stream);
    os << ';' << endl;
    if (debug_unparse)
        os << "/*:ThrowStatement#" << id << "*/";
}
Ast* ThrowStatement::Clone(AstStoragePool* ast_pool)
{
    ThrowStatement* clone = ast_pool -> GenThrowStatement();
    clone -> throw_token = throw_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> semicolon_token = semicolon_token;
    return clone;
}

void   ThrowStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void ThrowStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ThrowStatement):  "
            << lex_stream.NameString(throw_token->index)
            << ' '
            << " #" << expression -> id << endl;
    expression -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstThrowStatement_def_INCLUDED
