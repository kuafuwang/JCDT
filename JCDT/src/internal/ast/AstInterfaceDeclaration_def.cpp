#include <JCDT_Lib/internal/ast/AstTypeParameters.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>
#include <JCDT_Lib/internal/ast/AstModifiers.h>
#ifndef AstInterfaceDeclaration_def_INCLUDED
#define AstInterfaceDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstInterfaceDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void InterfaceTypeDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*InterfaceTypeDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(interface_token->index) << ' '
       << lex_stream -> NameString(class_body -> identifier_token->index) << ' ';
    if (type_parameters_opt)
        type_parameters_opt -> Unparse(os, lex_stream);
    if (NumInterfaces())
    {
        os << "extends ";
        Interface(0) -> Unparse(os, lex_stream);
        for (unsigned i = 1; i < NumInterfaces(); i++)
        {
            os << ", ";
            Interface(i) -> Unparse(os, lex_stream);
        }
    }
    class_body -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:InterfaceTypeDeclaration#" << id << "*/";
}

Ast* InterfaceTypeDeclaration::Clone(AstStoragePool* ast_pool)
{
    InterfaceTypeDeclaration* clone = ast_pool -> GenInterfaceDeclaration();
    if (modifiers_opt)
        clone -> modifiers_opt =
            (AstModifiers*) modifiers_opt -> Clone(ast_pool);
    clone -> interface_token = interface_token;
    if (type_parameters_opt)
        clone -> type_parameters_opt =
            (AstTypeParameters*) type_parameters_opt -> Clone(ast_pool);
    clone -> AllocateInterfaces(NumInterfaces());
    for (unsigned i = 0; i < NumInterfaces(); i++)
        clone -> AddInterface((AstTypeName*) Interface(i) -> Clone(ast_pool));
    clone -> class_body = (AstClassBody*) class_body -> Clone(ast_pool);
    clone -> class_body -> owner = clone;
	clone->_level = _level;
    return clone;
}

	Token* InterfaceTypeDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken() : interface_token;
	}
	  void InterfaceTypeDeclaration::AllocateInterfaces(unsigned estimate)
	{
		assert(!interfaces);
		interfaces = new (pool) AstArray<AstTypeName*>(pool, estimate);
	}

	Token* InterfaceTypeDeclaration::RightToken()
	{ return class_body->right_brace_token; }

	void   InterfaceTypeDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	unsigned i;
	parent_scope = scope;
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		if (type_parameters_opt)
			type_parameters_opt->traverse(visitor, scope);
		unsigned num = NumInterfaces();
		for (i = 0; i < num; i++)
			(*interfaces)[i]->traverse(visitor, scope);
		class_body->traverse(visitor, this);
	}
	visitor->endVisit(this,scope);
}
void InterfaceTypeDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (InterfaceDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << ' '
            << lex_stream.NameString(interface_token->index) << ' '
            << lex_stream.NameString(class_body -> identifier_token->index) << " #"
            << (type_parameters_opt ? type_parameters_opt -> id : 0) << " (";
    for (i = 0; i < NumInterfaces(); i++)
        os << " #" << Interface(i) -> id;
    os << ") #" << class_body -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    if (type_parameters_opt)
        type_parameters_opt -> Print(os, lex_stream);
    for (i = 0; i < NumInterfaces(); i++)
        Interface(i) -> Print(os, lex_stream);
    class_body -> Print(os, lex_stream);
}




} // Close namespace Jikes block


#endif // AstInterfaceDeclaration_def_INCLUDED
