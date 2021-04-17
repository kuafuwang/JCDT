#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#ifndef AstFieldAccess_def_INCLUDED
#define AstFieldAccess_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFieldAccess.h"
namespace Jikes { // Open namespace Jikes block


void FieldReference::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*FieldReference:#" << id << "*/";
    base -> Unparse(os, lex_stream);
    os << '.' << lex_stream -> NameString(identifier_token->index);
    if (debug_unparse)
        os << "/*:FieldReference#" << id << "*/";
}


void   FieldReference::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		base->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	bool FieldReference::isSuperAccess()
	{
		return base && base->isSuper();
	}

	bool FieldReference::isTypeAccess()
	{
		return base && base->isTypeReference();
	}

	FieldReference::FieldReference(): Expression(DOT), base(nullptr), identifier_token(nullptr), resolution_opt(nullptr)
	{}

	void FieldReference::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (FieldAccess):  "
            << " #" << base -> id << ' '
            << lex_stream.NameString(identifier_token->index)
            << endl;

    base -> Print(os, lex_stream);
}

Ast* FieldReference::Clone(AstStoragePool* ast_pool)
{
    FieldReference* clone = ast_pool -> GenFieldAccess();
    clone -> base = (Expression*) base -> Clone(ast_pool);
    clone -> identifier_token = identifier_token;
    if (resolution_opt)
        clone -> resolution_opt =
            (Expression*) resolution_opt -> Clone(ast_pool);
    return clone;
}

} // Close namespace Jikes block


#endif // AstFieldAccess_def_INCLUDED
