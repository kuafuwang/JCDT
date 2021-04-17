#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstVariableDeclarator.h>
#ifndef AstFieldDeclaration_def_INCLUDED
#define AstFieldDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFieldDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void FieldDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*FieldDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    type -> Unparse(os, lex_stream);
    os << ' ';
    VariableDeclarator(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumVariableDeclarators(); i++)
    {
        os << ", ";
        VariableDeclarator(i) -> Unparse(os, lex_stream);
    }
    os << ';' << endl;
    if (debug_unparse)
        os << "/*:FieldDeclaration#" << id << "*/";
}

	Ast* FieldDeclaration::Clone(AstStoragePool* ast_pool)
	{
		FieldDeclaration* clone = ast_pool->GenFieldDeclaration();
		clone->other_tag = other_tag;
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		clone->type = (AstType*)type->Clone(ast_pool);
		clone->AllocateVariableDeclarators(NumVariableDeclarators());
		for (unsigned i = 0; i < NumVariableDeclarators(); i++)
			clone->AddVariableDeclarator((AstVariableDeclarator*)
				VariableDeclarator(i) ->
				Clone(ast_pool));
		clone->semicolon_token = semicolon_token;
		return clone;
	}



	Token* FieldDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : type->LeftToken();
	}

	void   FieldDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
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

	FieldDeclaration::FieldDeclaration(AstStoragePool* p): AstDeclared(FIELD)
	                                                       , pool(p), variable_declarators(nullptr), type(nullptr), semicolon_token(nullptr)
	{}

	AstVariableDeclarator*& FieldDeclaration::VariableDeclarator(unsigned i)
	{
		return (*variable_declarators)[i];
	}

	unsigned FieldDeclaration::NumVariableDeclarators()
	{
		return variable_declarators ? variable_declarators->Length() : 0;
	}

	void FieldDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (FieldDeclaration):  #"
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


#endif // AstFieldDeclaration_def_INCLUDED
