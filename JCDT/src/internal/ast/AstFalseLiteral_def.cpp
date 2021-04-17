#ifndef AstFalseLiteral_def_INCLUDED
#define AstFalseLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFalseLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void FalseLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*FalseLiteral:#" << id << "*/";
    os << lex_stream -> NameString(false_literal_token->index);
    if (debug_unparse)
        os << "/*:FalseLiteral#" << id << "*/";
}


Ast* FalseLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenFalseLiteral(false_literal_token);
}


void   FalseLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void FalseLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (FalseLiteral):  "
            << lex_stream.NameString(false_literal_token->index)
            << endl;
}

} // Close namespace Jikes block


#endif // AstFalseLiteral_def_INCLUDED
