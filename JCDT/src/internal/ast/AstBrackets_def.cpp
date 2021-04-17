#ifndef AstBrackets_def_INCLUDED
#define AstBrackets_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstBrackets.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void AstBrackets::Unparse(Ostream& os, LexStream*)
{
    if (debug_unparse)
        os << "/*AstBrackets:#" << id << "*/";
    for (unsigned i = 0; i < dims; i++)
        os << "[]";
    if (debug_unparse)
        os << "/*:AstBrackets#" << id << "*/";
}

	Ast* AstBrackets::Clone(AstStoragePool* ast_pool)
	{
		AstBrackets* clone =
			ast_pool->GenBrackets(left_bracket_token, right_bracket_token);
		clone->dims = dims;
		return clone;
	}

	AstBrackets::AstBrackets(Token* l, Token* r): Ast(BRACKETS)
	                                              , left_bracket_token(l)
	                                              , right_bracket_token(r)
	                                              , dims(1)
	{}

	void AstBrackets::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (Brackets, dims=" << dims << "):  ";

    for (TokenIndex i = left_bracket_token->index; i <= right_bracket_token->index; i++)
        os << lex_stream.NameString(i);
    os << endl;
}
void  AstBrackets::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}

} // Close namespace Jikes block


#endif // AstBrackets_def_INCLUDED
