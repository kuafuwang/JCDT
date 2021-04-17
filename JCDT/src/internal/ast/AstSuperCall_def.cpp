#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstTypeArguments.h>
#include <JCDT_Lib/internal/ast/AstArguments.h>
#ifndef AstSuperCall_def_INCLUDED
#define AstSuperCall_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSuperCall.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstSuperCall::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstSuperCall:#" << id << "*/";
    if (! generated)
    {
        if (base_opt && ! base_opt -> generated)
        {
            base_opt -> Unparse(os, lex_stream);
            os << '.';
        }
        if (type_arguments_opt)
            type_arguments_opt -> Unparse(os, lex_stream);
        os << lex_stream -> NameString(super_token->index);
        arguments -> Unparse(os, lex_stream);
        os << lex_stream -> NameString(semicolon_token->index) << endl;
    }
    if (debug_unparse)
         os << "/*:AstSuperCall#" << id << "*/";
}

	Ast* AstSuperCall::Clone(AstStoragePool* ast_pool)
	{
		AstSuperCall* clone = ast_pool->GenSuperCall();
		if (base_opt)
			clone->base_opt = (Expression*)base_opt->Clone(ast_pool);
		if (type_arguments_opt)
			clone->type_arguments_opt =
			(AstTypeArguments*)type_arguments_opt->Clone(ast_pool);
		clone->super_token = super_token;
		clone->arguments = (AstArguments*)arguments->Clone(ast_pool);
		clone->semicolon_token = semicolon_token;
		return clone;
	}



	Token* AstSuperCall::LeftToken()
	{
		return base_opt ? base_opt->LeftToken()
			       : type_arguments_opt ? type_arguments_opt->left_angle_token
			       : super_token;
	}

	bool AstSuperCall::isSuperAccess()
	{
		return true;
	}

	void   AstSuperCall::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		if (base_opt)
			base_opt->traverse(visitor, scope);
		if (type_arguments_opt)
			type_arguments_opt->traverse(visitor, scope);
		arguments->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstSuperCall::AstSuperCall():
		ReachComleteAbleStatement(SUPER_CALL, true, true),
		symbol(nullptr), base_opt(nullptr), type_arguments_opt(nullptr), 
		super_token(nullptr), arguments(nullptr), semicolon_token(nullptr)
	{}

	void AstSuperCall::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (SuperCall):  #"
            << (base_opt ? base_opt -> id : 0) << ".#"
            << (type_arguments_opt ? type_arguments_opt -> id : 0)
            << lex_stream.NameString(super_token->index) << " #" << arguments -> id
            << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
    if (type_arguments_opt)
        type_arguments_opt -> Print(os, lex_stream);
    arguments -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstSuperCall_def_INCLUDED
