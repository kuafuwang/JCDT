#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstArguments.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#ifndef AstEnumConstant_def_INCLUDED
#define AstEnumConstant_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstEnumConstant.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstEnumConstant::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstEnumConstant:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(identifier_token->index);
    if (arguments_opt)
        arguments_opt -> Unparse(os, lex_stream);
    if (class_body_opt)
    {
        os << ' ';
        class_body_opt -> Unparse(os, lex_stream);
    }
    if (debug_unparse)
        os << "/*:AstEnumConstant#" << id << "*/";
}

Ast* AstEnumConstant::Clone(AstStoragePool* ast_pool)
{
    AstEnumConstant* clone = ast_pool -> GenEnumConstant(identifier_token);
    if (modifiers_opt)
        clone -> modifiers_opt =
            (AstModifiers*) modifiers_opt -> Clone(ast_pool);
    if (arguments_opt)
        clone -> arguments_opt =
            (AstArguments*) arguments_opt -> Clone(ast_pool);
    if (class_body_opt)
        clone -> class_body_opt =
            (AstClassBody*) class_body_opt -> Clone(ast_pool);
    return clone;
}


void   AstEnumConstant::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		if (arguments_opt)
			arguments_opt->traverse(visitor, scope);
		if (class_body_opt)
			class_body_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}


	Token* AstEnumConstant::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken() : identifier_token;
	}

	Token* AstEnumConstant::RightToken()
	{
		return class_body_opt ? class_body_opt->right_brace_token
			       : arguments_opt ? arguments_opt->right_parenthesis_token
			       : identifier_token;
	}

	AstEnumConstant::AstEnumConstant(Token* t): AstDeclared(ENUM_CONSTANT)
	                                            , identifier_token(t),
		arguments_opt(nullptr), class_body_opt(nullptr), 
		ordinal(0), field_symbol(nullptr), ctor_symbol(nullptr)
	{}

	void AstEnumConstant::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (EnumConstant):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << ' '
            << lex_stream.NameString(identifier_token->index) << " #"
            << (arguments_opt ? arguments_opt -> id : 0) << " #"
            << (class_body_opt ? class_body_opt -> id : 0) << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    if (arguments_opt)
        arguments_opt -> Print(os, lex_stream);
    if (class_body_opt)
        class_body_opt -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstEnumConstant_def_INCLUDED
