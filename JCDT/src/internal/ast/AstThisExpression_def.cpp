#include <JCDT_Lib/internal/ast/AstTypeName.h>
#ifndef AstThisExpression_def_INCLUDED
#define AstThisExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstThisExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void AstThisExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstThisExpression:#" << id << "*/";
    if (base_opt)
    {
        base_opt -> Unparse(os, lex_stream);
        os << '.';
    }
    os << lex_stream -> NameString(this_token->index);
    if (debug_unparse)
        os << "/*:AstThisExpression#" << id << "*/";
}


Ast* AstThisExpression::Clone(AstStoragePool* ast_pool)
{
    AstThisExpression* clone = ast_pool -> GenThisExpression(this_token);
    if (base_opt)
        clone -> base_opt = (AstTypeName*) base_opt -> Clone(ast_pool);
    if (resolution_opt)
        clone -> resolution_opt =
            (Expression*) resolution_opt -> Clone(ast_pool);
    return clone;
}



	Token* AstThisExpression::LeftToken()
	{
		return base_opt ? base_opt->LeftToken() : this_token;
	}

	void   AstThisExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		if (base_opt)
			base_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstThisExpression::AstThisExpression(Token* token): 
		Expression(THIS_EXPRESSION), base_opt(nullptr) , this_token(token),
		resolution_opt(nullptr)
	{}

	void AstThisExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ThisExpression):  ";
    if (base_opt)
        os << '#' << base_opt -> id << ". ";
    os << lex_stream.NameString(this_token->index) << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstThisExpression_def_INCLUDED
