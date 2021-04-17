#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>
#include <JCDT_Lib/internal/ast/AstEnumConstant.h>
#ifndef AstEnumDeclaration_def_INCLUDED
#define AstEnumDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstEnumDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void EnumTypeDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    unsigned i;
    if (debug_unparse)
        os << "/*EnumTypeDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(enum_token->index) << ' '
       << lex_stream -> NameString(class_body -> identifier_token->index) << ' ';
    if (NumInterfaces())
    {
        os << "extends ";
        Interface(0) -> Unparse(os, lex_stream);
        for (i = 1; i < NumInterfaces(); i++)
        {
            os << ", ";
            Interface(i) -> Unparse(os, lex_stream);
        }
    }
    os << '{' << endl;
    for (i = 0; i < NumEnumConstants(); i++)
    {
        EnumConstant(i) -> Unparse(os, lex_stream);
        os << ',' << endl;
    }
    class_body -> Unparse(os, lex_stream, true);
    if (debug_unparse)
        os << "/*:EnumTypeDeclaration#" << id << "*/";
}
	Ast* EnumTypeDeclaration::Clone(AstStoragePool* ast_pool)
	{
		unsigned i;
		EnumTypeDeclaration* clone = ast_pool->GenEnumDeclaration();
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		clone->AllocateInterfaces(NumInterfaces());
		for (i = 0; i < NumInterfaces(); i++)
			clone->AddInterface((AstTypeName*)Interface(i)->Clone(ast_pool));
		clone->AllocateEnumConstants(NumEnumConstants());
		for (i = 0; i < NumEnumConstants(); i++)
			clone->AddEnumConstant((AstEnumConstant*)EnumConstant(i) ->
				Clone(ast_pool));
		clone->class_body = (AstClassBody*)class_body->Clone(ast_pool);
		clone->class_body->owner = clone;
		clone->_level = _level;
		return clone;
	}

	

	Token* EnumTypeDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken() : enum_token;
	}
	 void EnumTypeDeclaration::AllocateEnumConstants(unsigned estimate)
	{
		assert(!enum_constants);
		enum_constants = new (pool) AstArray<AstEnumConstant*>(pool, estimate);
	}
	Token* EnumTypeDeclaration::RightToken()
	{ return class_body->right_brace_token; }

	void   EnumTypeDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	parent_scope = scope;
	unsigned i;
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		unsigned num = NumInterfaces();
		for (i = 0; i < num; i++)
			(*interfaces)[i]->traverse(visitor, scope);
		num = NumEnumConstants();

	

		for (i = 0; i < num; i++)
			(*enum_constants)[i]->traverse(visitor, this);
		class_body->traverse(visitor, this);
	}
	visitor->endVisit(this,scope);
}

	
	void EnumTypeDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (EnumDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << ' '
            << lex_stream.NameString(enum_token->index) << ' '
            << lex_stream.NameString(class_body -> identifier_token->index) << " (";
    for (i = 0; i < NumInterfaces(); i++)
        os << " #" << Interface(i) -> id;
    os << ") {";
    for (i = 0; i < NumEnumConstants(); i++)
        os << " #" << EnumConstant(i) -> id;
    os << "} #" << class_body -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    for (i = 0; i < NumInterfaces(); i++)
        Interface(i) -> Print(os, lex_stream);
    for (i = 0; i < NumEnumConstants(); i++)
        EnumConstant(i) -> Print(os, lex_stream);
    class_body -> Print(os, lex_stream);
}

	EnumTypeDeclaration::EnumTypeDeclaration(AstStoragePool* p): TypeDeclaration(ENUM_TYPE)
	                                                             , pool(p), interfaces(nullptr), enum_constants(nullptr), enum_token(nullptr)
	{}

	void EnumTypeDeclaration::AddEnumConstant(AstEnumConstant* constant)
	{
		assert(enum_constants);
		constant->ordinal = enum_constants->Length();
		enum_constants->Next() = constant;
	}
	 void EnumTypeDeclaration::AllocateInterfaces(unsigned estimate)
	{
		assert(!interfaces);
		interfaces = new (pool) AstArray<AstTypeName*>(pool, estimate);
	}



} // Close namespace Jikes block


#endif // AstEnumDeclaration_def_INCLUDED
