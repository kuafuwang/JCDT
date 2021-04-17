#ifndef AstLongLiteral_def_INCLUDED
#define AstLongLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstLongLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void LongLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*LongLiteral:#" << id << "*/";
    os << lex_stream -> NameString(long_literal_token->index);
    if (debug_unparse)
        os << "/*:LongLiteral#" << id << "*/";
}

Ast* LongLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenLongLiteral(long_literal_token);
}


void   LongLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void LongLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (LongLiteral):  "
            << lex_stream.NameString(long_literal_token->index)
            << endl;
}


} // Close namespace Jikes block


#endif // AstLongLiteral_def_INCLUDED
