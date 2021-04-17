#include <JCDT_Lib/internal/ast/AstTypeName.h>
#ifndef AstSuperExpression_def_INCLUDED
#define AstSuperExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSuperExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstSuperExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstSuperExpression:#" << id << "*/";
    if (base_opt)
    {
        base_opt -> Unparse(os, lex_stream);
        os << '.';
    }
    os << lex_stream -> NameString(super_token->index);
    if (debug_unparse)
        os << "/*:AstSuperExpression#" << id << "*/";
}


Ast* AstSuperExpression::Clone(AstStoragePool* ast_pool)
{
    AstSuperExpression* clone = ast_pool -> GenSuperExpression(super_token);
    if (base_opt)
        clone -> base_opt = (AstTypeName*) base_opt -> Clone(ast_pool);
    if (resolution_opt)
        clone -> resolution_opt =
            (Expression*) resolution_opt -> Clone(ast_pool);
    return clone;
}




	Token* AstSuperExpression::LeftToken()
	{
		return base_opt ? base_opt->LeftToken() : super_token;
	}

	void   AstSuperExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		if (base_opt)
			base_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstSuperExpression::AstSuperExpression(Token* token): 
		Expression(SUPER_EXPRESSION), base_opt(nullptr)
	 , super_token(token), resolution_opt(nullptr)
	{}

	void AstSuperExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (SuperExpression):  ";
    if (base_opt)
        os << '#' << base_opt -> id << ". ";
    os << lex_stream.NameString(super_token->index) << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstSuperExpression_def_INCLUDED
