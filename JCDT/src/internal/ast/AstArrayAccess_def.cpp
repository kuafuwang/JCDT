#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstArrayAccess_def_INCLUDED
#define AstArrayAccess_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstArrayAccess.h"
namespace Jikes { // Open namespace Jikes block

void ArrayReference::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ArrayReference:#" << id << "*/";
    base -> Unparse(os, lex_stream);
    os << '[';
    expression -> Unparse(os, lex_stream);
    os << ']';
    if (debug_unparse)
        os << "/*:ArrayReference#" << id << "*/";
}
Ast* ArrayReference::Clone(AstStoragePool* ast_pool)
{
    ArrayReference* clone = ast_pool -> GenArrayAccess();
    clone -> base = (Expression*) base -> Clone(ast_pool);
    clone -> left_bracket_token = left_bracket_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> right_bracket_token = right_bracket_token;
    return clone;
}

	ArrayReference::ArrayReference()
		: Expression(ARRAY_ACCESS), base(nullptr), 
		left_bracket_token(nullptr), expression(nullptr),
		right_bracket_token(nullptr)
	{}

	void ArrayReference::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ArrayAccess):  "
            << '#' << base -> id
            << " [ #" << expression -> id << " ]" << endl;

    base -> Print(os, lex_stream);
    expression -> Print(os, lex_stream);
}


void  ArrayReference::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		base->traverse(visitor,scope);
		expression->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}


} // Close namespace Jikes block


#endif // AstArrayAccess_def_INCLUDED
