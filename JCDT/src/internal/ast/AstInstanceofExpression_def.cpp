#include <JCDT_Lib/internal/ast/AstType.h>
#ifndef AstInstanceofExpression_def_INCLUDED
#define AstInstanceofExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstInstanceofExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void InstanceofExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*InstanceofExpression:#" << id << "*/";
    expression -> Unparse(os, lex_stream);
    os << ' ' << lex_stream -> NameString(instanceof_token->index) << ' ';
    type -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:InstanceofExpression#" << id << "*/";
}
	
	Token* InstanceofExpression::RightToken()
	{ return type->RightToken(); }


	void   InstanceofExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		expression->traverse(visitor, scope);
		type->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void InstanceofExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (InstanceofExpression):  #"
            << expression -> id << ' '
            << lex_stream.NameString(instanceof_token->index)
            << " #" << type -> id << endl;
    expression -> Print(os, lex_stream);
    type -> Print(os, lex_stream);
}

Ast* InstanceofExpression::Clone(AstStoragePool* ast_pool)
{
    InstanceofExpression* clone = ast_pool -> GenInstanceofExpression();
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> instanceof_token = instanceof_token;
    clone -> type = (AstType*) type -> Clone(ast_pool);
    return clone;
}

} // Close namespace Jikes block


#endif // AstInstanceofExpression_def_INCLUDED
