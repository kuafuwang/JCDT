#include <JCDT_Lib/internal/ast/AstVariableDeclarator.h>
#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#ifndef AstFormalParameter_def_INCLUDED
#define AstFormalParameter_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFormalParameter.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstFormalParameter::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstFormalParameter:#" << id << "*/";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    type -> Unparse(os, lex_stream);
    os << (ellipsis_token_opt ? "... " : " ");
    formal_declarator -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstFormalParameter#" << id << "*/";
}

	Ast* AstFormalParameter::Clone(AstStoragePool* ast_pool)
	{
		AstFormalParameter* clone = ast_pool->GenFormalParameter();
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		clone->type = (AstType*)type->Clone(ast_pool);
		clone->ellipsis_token_opt = ellipsis_token_opt;
		clone->formal_declarator =
			(AstVariableDeclarator*)formal_declarator->Clone(ast_pool);
		clone->formal_declarator->owner = clone;
		return clone;
	}



void   AstFormalParameter::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		type->traverse(visitor, scope);
		
		formal_declarator->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	Token* AstFormalParameter::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : type->LeftToken();
	}

	Token* AstFormalParameter::RightToken()
	{
		return formal_declarator->RightToken();
	}

	AstFormalParameter::AstFormalParameter(): Ast(PARAMETER), modifiers_opt(nullptr), type(nullptr), ellipsis_token_opt(nullptr), formal_declarator(nullptr)
	{}

	void AstFormalParameter::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (FormalParameter):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0)
            << " #" << type -> id << " #" << formal_declarator -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    type -> Print(os, lex_stream);
    if (ellipsis_token_opt)
        os << lex_stream.NameString(ellipsis_token_opt->index);
    formal_declarator -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstFormalParameter_def_INCLUDED
