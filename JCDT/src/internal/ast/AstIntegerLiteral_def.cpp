#ifndef AstIntegerLiteral_def_INCLUDED
#define AstIntegerLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstIntegerLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void IntegerLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*IntegerLiteral:#" << id << "*/";
    os << lex_stream -> NameString(integer_literal_token->index);
    if (debug_unparse)
        os << "/*:IntegerLiteral#" << id << "*/";
}

Ast* IntegerLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenIntegerLiteral(integer_literal_token);
}


void   IntegerLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void IntegerLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (IntegerLiteral):  "
            << lex_stream.NameString(integer_literal_token->index)
            << endl;
}



} // Close namespace Jikes block


#endif // AstIntegerLiteral_def_INCLUDED
