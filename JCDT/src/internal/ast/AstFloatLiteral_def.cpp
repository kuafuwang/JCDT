#ifndef AstFloatLiteral_def_INCLUDED
#define AstFloatLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFloatLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void FloatLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*FloatLiteral:#" << id << "*/";
    os << lex_stream -> NameString(float_literal_token->index);
    if (debug_unparse)
        os << "/*:FloatLiteral#" << id << "*/";
}

Ast* FloatLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenFloatLiteral(float_literal_token);
}


void   FloatLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void FloatLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (FloatLiteral):  "
            << lex_stream.NameString(float_literal_token->index)
            << endl;
}

} // Close namespace Jikes block


#endif // AstFloatLiteral_def_INCLUDED
