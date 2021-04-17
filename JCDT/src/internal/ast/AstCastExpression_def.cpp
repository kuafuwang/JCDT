#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#ifndef AstCastExpression_def_INCLUDED
#define AstCastExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstCastExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void CastExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*CastExpression:#" << id << "*/";
    if (! generated)
    {
        os << '(';
        type -> Unparse(os, lex_stream);
        os << ')';
    }
    expression -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:CastExpression#" << id << "*/";
}
Ast* CastExpression::Clone(AstStoragePool* ast_pool)
{
    CastExpression* clone = ast_pool -> GenCastExpression();
    clone -> left_parenthesis_token = left_parenthesis_token;
    if (type)
        clone -> type = (AstType*) type -> Clone(ast_pool);
    clone -> right_parenthesis_token = right_parenthesis_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    return clone;
}

	CastExpression::CastExpression():
		Expression(CAST), left_parenthesis_token(nullptr), 
		type(nullptr), right_parenthesis_token(nullptr), expression(nullptr)
	{}

	void CastExpression::Print(Ostream& os,LexStream& lex_stream)
{
    if (type)
    {
        os << '#' << id << " #" << expression -> id << endl;
        type -> Print(os, lex_stream);
    }
    else
    {
        os << '#' << id << " (Java Semantic Cast to " << Type() -> Name()
                << "):  #" << expression -> id << endl;
    }
    expression -> Print(os, lex_stream);
}
void  CastExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (type){
			type->traverse(visitor,scope);
		}
		
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

} // Close namespace Jikes block


#endif // AstCastExpression_def_INCLUDED
