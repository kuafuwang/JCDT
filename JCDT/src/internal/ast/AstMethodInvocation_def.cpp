#ifndef AstMethodInvocation_Jikes_def_INCLUDED
#define AstMethodInvocation_Jikes_def_INCLUDED
#include <JCDT_Lib/internal/ast/AstTypeArguments.h>
#include <JCDT_Lib/internal/ast/AstArguments.h>
#include "JCDT_Lib/internal/ast/AstMethodInvocation.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block




void AstMethodInvocation::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstMethodInvocation:#" << id << "*/";
    if (base_opt && ! base_opt -> generated)
    {
        base_opt -> Unparse(os, lex_stream);
        os << '.';
    }
    if (type_arguments_opt)
        type_arguments_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(identifier_token->index);
    arguments -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstMethodInvocation#" << id << "*/";
}
Ast* AstMethodInvocation::Clone(AstStoragePool* ast_pool)
{
    AstMethodInvocation* clone =
        ast_pool -> GenMethodInvocation(identifier_token);
    if (base_opt)
        clone -> base_opt = (Expression*) base_opt -> Clone(ast_pool);
    if (type_arguments_opt)
        clone -> type_arguments_opt =
            (AstTypeArguments*) type_arguments_opt -> Clone(ast_pool);
    clone -> identifier_token = identifier_token;
    clone -> arguments = (AstArguments*) arguments -> Clone(ast_pool);
    if (resolution_opt)
        clone -> resolution_opt =
            (Expression*) resolution_opt -> Clone(ast_pool);
    return clone;
}


void   AstMethodInvocation::traverse(ASTVisitor* visitor,AstNodeScope* scope)
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

	Token* AstMethodInvocation::LeftToken()
	{
		if (type_arguments_opt)
		assert(base_opt);
		return base_opt ? base_opt->LeftToken() : identifier_token;
	}

	Token* AstMethodInvocation::RightToken()
	{
		return arguments->right_parenthesis_token;
	}

	bool AstMethodInvocation::isTypeAccess()
	{
		if (base_opt)
			return base_opt->isTypeReference();
		return false;
	}

	bool AstMethodInvocation::isSuperAccess()
	{
		if (base_opt)
			return base_opt->isSuper();
		return false;
	}

	AstMethodInvocation::AstMethodInvocation(Token* t): Expression(CALL), base_opt(nullptr), type_arguments_opt(nullptr)
	                                                    , identifier_token(t), arguments(nullptr), resolution_opt(nullptr)
	{}

	void AstMethodInvocation::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (MethodInvocation):  #"
            << (base_opt ? base_opt -> id : 0) << ".#"
            << (type_arguments_opt ? type_arguments_opt -> id : 0) << ' '
            << lex_stream.NameString(identifier_token->index)
            << " #" << arguments -> id << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
    if (type_arguments_opt)
        type_arguments_opt -> Print(os, lex_stream);
    arguments -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstMethodInvocation_def_INCLUDED
