#ifndef AstTrueLiteral_def_INCLUDED
#define AstTrueLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTrueLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void TrueLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*TrueLiteral:#" << id << "*/";
    os << lex_stream -> NameString(true_literal_token->index);
    if (debug_unparse)
        os << "/*:TrueLiteral#" << id << "*/";
}


Ast* TrueLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenTrueLiteral(true_literal_token);
}




void   TrueLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void TrueLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (TrueLiteral):  "
            << lex_stream.NameString(true_literal_token->index)
            << endl;
}

} // Close namespace Jikes block


#endif // AstTrueLiteral_def_INCLUDED
