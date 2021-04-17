#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstDimExpr_def_INCLUDED
#define AstDimExpr_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstDimExpr.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstDimExpr::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstDimExpr:#" << id << "*/";
    os << '[';
    expression -> Unparse(os, lex_stream);
    os << ']';
    if (debug_unparse)
        os << "/*:AstDimExpr#" << id << "*/";
}

void   AstDimExpr::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		expression->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}


void AstDimExpr::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (DimExpr):  [ #" << expression -> id << " ]"
            << endl;
    expression -> Print(os, lex_stream);
}


Ast* AstDimExpr::Clone(AstStoragePool* ast_pool)
{
    AstDimExpr* clone = ast_pool -> GenDimExpr();
    clone -> left_bracket_token = left_bracket_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> right_bracket_token = right_bracket_token;
    return clone;
}


} // Close namespace Jikes block


#endif // AstDimExpr_def_INCLUDED
