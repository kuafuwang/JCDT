#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstVariableDeclarator.h>
#ifndef AstLocalVariableStatement_def_INCLUDED
#define AstLocalVariableStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstLocalVariableStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

void LocalDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*LocalDeclaration:#" << id << "*/";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    type -> Unparse(os, lex_stream);
    os << ' ';
    VariableDeclarator(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumVariableDeclarators(); i++)
    {
        os << ',';
        VariableDeclarator(i) -> Unparse(os, lex_stream);
    }
    if (semicolon_token_opt)
        os << ';' << endl;
    if (debug_unparse)
        os << "/*:LocalDeclaration#" << id << "*/";
}
Ast* LocalDeclaration::Clone(AstStoragePool* ast_pool)
{
    LocalDeclaration* clone = ast_pool -> GenLocalVariableStatement();
    if (modifiers_opt)
        clone -> modifiers_opt =
            (AstModifiers*) modifiers_opt -> Clone(ast_pool);
    clone -> type = (AstType*) type -> Clone(ast_pool);
    clone -> AllocateVariableDeclarators(NumVariableDeclarators());
    for (unsigned i = 0; i < NumVariableDeclarators(); i++)
        clone -> AddVariableDeclarator((AstVariableDeclarator*)
                                       VariableDeclarator(i) ->
                                       Clone(ast_pool));
    clone -> semicolon_token_opt = semicolon_token_opt;
    return clone;
}

	Token* LocalDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : type->LeftToken();
	}

	Token* LocalDeclaration::RightToken()
	{
		return semicolon_token_opt ? semicolon_token_opt
			       : (VariableDeclarator(NumVariableDeclarators() - 1) ->
				       RightToken());
	}

	void   LocalDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	unsigned i;
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		type->traverse(visitor, scope);
		unsigned num = NumVariableDeclarators();
		for (i = 0; i < num; i++)
			(*variable_declarators)[i]->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstVariableDeclarator*& LocalDeclaration::VariableDeclarator(unsigned i)
	{
		return (*variable_declarators)[i];
	}

	unsigned LocalDeclaration::NumVariableDeclarators()
	{
		return variable_declarators ? variable_declarators->Length() : 0;
	}

	void LocalDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (LocalVariableStatement):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0)
            << " #" << type -> id << '(';
    for (i = 0; i < NumVariableDeclarators(); i++)
        os << " #" << VariableDeclarator(i) -> id;
    os << ')' << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    type -> Print(os, lex_stream);
    for (i = 0; i < NumVariableDeclarators(); i++)
        VariableDeclarator(i) -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstLocalVariableStatement_def_INCLUDED
