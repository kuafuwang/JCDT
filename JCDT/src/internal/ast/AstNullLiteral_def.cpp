#ifndef AstNullLiteral_def_INCLUDED
#define AstNullLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstNullLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void NullLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*NullLiteral:#" << id << "*/";
    os << lex_stream -> NameString(null_token->index);
    if (debug_unparse)
        os << "/*:NullLiteral#" << id << "*/";
}
Ast* NullLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenNullLiteral(null_token);
}



void   NullLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void NullLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (NullLiteral):  "
            << lex_stream.NameString(null_token->index)
            << endl;
}

} // Close namespace Jikes block


#endif // AstNullLiteral_def_INCLUDED
