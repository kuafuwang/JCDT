#ifndef AstDoubleLiteral_def_INCLUDED
#define AstDoubleLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstDoubleLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void DoubleLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*DoubleLiteral:#" << id << "*/";
    os << lex_stream -> NameString(double_literal_token->index);
    if (debug_unparse)
        os << "/*:DoubleLiteral#" << id << "*/";
}

Ast* DoubleLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenDoubleLiteral(double_literal_token);
}


void   DoubleLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void DoubleLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (DoubleLiteral):  "
            << lex_stream.NameString(double_literal_token->index)
            << endl;
}



} // Close namespace Jikes block


#endif // AstDoubleLiteral_def_INCLUDED
