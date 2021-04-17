#include <JCDT_Lib/internal/ast/AstTypeArguments.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>
#include <JCDT_Lib/internal/ast/AstArguments.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#ifndef AstClassCreationExpression_def_INCLUDED
#define AstClassCreationExpression_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstClassCreationExpression.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void AstClassCreationExpression::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstClassCreationExpression:#" << id << "*/";
    if (base_opt && ! base_opt -> generated)
    {
        base_opt -> Unparse(os, lex_stream);
        os << '.';
    }
    os << lex_stream -> NameString(new_token->index) << ' ';
    if (type_arguments_opt)
        type_arguments_opt -> Unparse(os, lex_stream);
    class_type -> Unparse(os, lex_stream);
    arguments -> Unparse(os, lex_stream);
    if (anonymous_type_opt)
		anonymous_type_opt-> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstClassCreationExpression#" << id << "*/";
}


Ast* AstClassCreationExpression::Clone(AstStoragePool* ast_pool)
{
    AstClassCreationExpression* clone =
        ast_pool -> GenClassCreationExpression();
    if (base_opt)
        clone -> base_opt = (Expression*) base_opt -> Clone(ast_pool);
    clone -> new_token = new_token;
    if (type_arguments_opt)
        clone -> type_arguments_opt =
            (AstTypeArguments*) type_arguments_opt -> Clone(ast_pool);
    clone -> class_type = (AstTypeName*) class_type -> Clone(ast_pool);
    clone -> arguments = (AstArguments*) arguments -> Clone(ast_pool);
    if (anonymous_type_opt)
    {
		clone->anonymous_type_opt =
			(AstAnonymousTypeDeclaration*)anonymous_type_opt->Clone(ast_pool);
		clone->anonymous_type_opt->create_expr = this;
    }
       
    if (resolution_opt)
        clone -> resolution_opt =
            (AstClassCreationExpression*) resolution_opt -> Clone(ast_pool);
    return clone;
}

	AstClassCreationExpression::AstClassCreationExpression()
		: Expression(CLASS_CREATION), base_opt(nullptr), new_token(nullptr), 
		type_arguments_opt(nullptr), class_type(nullptr), arguments(nullptr),
		anonymous_type_opt(nullptr), resolution_opt(nullptr)
	{}

	void AstClassCreationExpression::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ClassCreationExpression):  #"
            << (base_opt ? base_opt -> id : 0) << ' '
            << lex_stream.NameString(new_token->index) << " #"
            << (type_arguments_opt ? type_arguments_opt -> id : 0) << " #"
            << class_type -> id << " #" << arguments -> id << " #"
            << (anonymous_type_opt ? anonymous_type_opt-> id : 0) << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
    if (type_arguments_opt)
        type_arguments_opt -> Print(os, lex_stream);
    class_type -> Print(os, lex_stream);
    arguments -> Print(os, lex_stream);
    if (anonymous_type_opt)
		anonymous_type_opt-> Print(os, lex_stream);
}
void  AstClassCreationExpression::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (base_opt)
			base_opt->traverse(visitor,scope);
		if (type_arguments_opt)
			type_arguments_opt->traverse(visitor,scope);
		class_type->traverse(visitor,scope);
		arguments->traverse(visitor,scope);
		if (anonymous_type_opt)
			anonymous_type_opt->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}


	Token* AstClassCreationExpression::RightToken()
	{
		return anonymous_type_opt ? anonymous_type_opt->RightToken()
			       : arguments->right_parenthesis_token;
	}

	bool AstClassCreationExpression::isSuperAccess()
	{
		return anonymous_type_opt != nullptr;
	}

	bool AstClassCreationExpression::isTypeAccess()
	{
		return true;
	}
} // Close namespace Jikes block


#endif // AstClassCreationExpression_def_INCLUDED
