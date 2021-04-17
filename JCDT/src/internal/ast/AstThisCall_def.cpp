#include <JCDT_Lib/internal/ast/AstArguments.h>
#include <JCDT_Lib/internal/ast/AstTypeArguments.h>
#ifndef AstThisCall_def_INCLUDED
#define AstThisCall_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstThisCall.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void AstThisCall::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstThisCall:#" << id << "*/";
    if (type_arguments_opt)
        type_arguments_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(this_token->index);
    arguments -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(semicolon_token->index) << endl;
    if (debug_unparse)
        os << "/*:AstThisCall#" << id << "*/";
}

	Ast* AstThisCall::Clone(AstStoragePool* ast_pool)
	{
		AstThisCall* clone = ast_pool->GenThisCall();
		if (type_arguments_opt)
			clone->type_arguments_opt =
			(AstTypeArguments*)type_arguments_opt->Clone(ast_pool);
		clone->this_token = this_token;
		clone->arguments = (AstArguments*)arguments->Clone(ast_pool);
		clone->semicolon_token = semicolon_token;
		return clone;
	}


void   AstThisCall::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		if (type_arguments_opt)
			type_arguments_opt->traverse(visitor, scope);
		arguments->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	Token* AstThisCall::LeftToken()
	{
		return type_arguments_opt ? type_arguments_opt->left_angle_token
			       : this_token;
	}

	bool AstThisCall::isSuperAccess()
	{
		return false;
	}


	AstThisCall::AstThisCall(): ReachComleteAbleStatement(THIS_CALL, true, true),
		symbol(nullptr), type_arguments_opt(nullptr), this_token(nullptr), arguments(nullptr), semicolon_token(nullptr)
	{}

	void AstThisCall::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ThisCall):  #"
            << (type_arguments_opt ? type_arguments_opt -> id : 0)
            << lex_stream.NameString(this_token->index) << " #" << arguments -> id
            << endl;
    if (type_arguments_opt)
        type_arguments_opt -> Print(os, lex_stream);
    arguments -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstThisCall_def_INCLUDED
