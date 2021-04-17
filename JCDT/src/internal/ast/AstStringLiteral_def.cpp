#ifndef AstStringLiteral_def_INCLUDED
#define AstStringLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstStringLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void StringLiteral::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*StringLiteral:#" << id << "*/";
    bool old_expand = os.ExpandWchar();
    os.SetExpandWchar(true);
    os << lex_stream -> NameString(string_literal_token->index);
    os.SetExpandWchar(old_expand);
    if (debug_unparse)
        os << "/*:StringLiteral#" << id << "*/";
}
Ast* StringLiteral::Clone(AstStoragePool* ast_pool)
{
    return ast_pool -> GenStringLiteral(string_literal_token);
}


void   StringLiteral::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}
void StringLiteral::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (StringLiteral):  "
            << lex_stream.NameString(string_literal_token->index)
            << endl;
}

} // Close namespace Jikes block


#endif // AstStringLiteral_def_INCLUDED
