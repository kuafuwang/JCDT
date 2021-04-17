#ifndef AstEmptyStatement_def_INCLUDED
#define AstEmptyStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstEmptyStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void EmptyStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*EmptyStatement:#" << id << "*/";
    os << lex_stream -> NameString(semicolon_token->index) << endl;
    if (debug_unparse)
        os << "/*:EmptyStatement#" << id << "*/";
}
Ast* EmptyStatement::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenEmptyStatement(semicolon_token);
}

void   EmptyStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void EmptyStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (EmptyStatement):  "
            << lex_stream.NameString(semicolon_token->index)
            << endl;
}


} // Close namespace Jikes block


#endif // AstEmptyStatement_def_INCLUDED
