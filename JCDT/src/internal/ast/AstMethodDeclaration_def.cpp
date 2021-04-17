#ifndef AstMethodDeclaration_def_INCLUDED
#define AstMethodDeclaration_def_INCLUDED
#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstTypeParameters.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstMemberValue.h>
#include <JCDT_Lib/internal/ast/AstMethodBody.h>
#include <JCDT_Lib/internal/ast/AstMethodDeclarator.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>

#include "JCDT_Lib/internal/ast/AstMethodDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include  <boost/algorithm/string.hpp>

namespace Jikes { // Open namespace Jikes block



void MethodDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*MethodDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    if (type_parameters_opt)
        type_parameters_opt -> Unparse(os, lex_stream);
    type -> Unparse(os, lex_stream);
    os << ' ';
    method_declarator -> Unparse(os, lex_stream);
    if (NumThrows())
    {
        os << "throws ";
        Throw(0) -> Unparse(os, lex_stream);
        for (unsigned i = 1; i < NumThrows(); i++)
        {
            os << ", ";
            Throw(i) -> Unparse(os, lex_stream);
        }
    }
    if (default_value_opt)
        default_value_opt -> Unparse(os, lex_stream);
    if (method_body_opt)
        method_body_opt -> Unparse(os, lex_stream);
    else os << ';';
    if (debug_unparse)
        os << "/*:MethodDeclaration#" << id << "*/";
}
	Ast* MethodDeclaration::Clone(AstStoragePool* ast_pool)
	{
		MethodDeclaration* clone = ast_pool->GenMethodDeclaration();
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		if (type_parameters_opt)
			clone->type_parameters_opt =
			(AstTypeParameters*)type_parameters_opt->Clone(ast_pool);
		clone->type = (AstType*)type->Clone(ast_pool);
		clone->method_declarator =
			(AstMethodDeclarator*)method_declarator->Clone(ast_pool);
		clone->AllocateThrows(NumThrows());
		for (unsigned i = 0; i < NumThrows(); i++)
			clone->AddThrow((AstTypeName*)Throw(i)->Clone(ast_pool));
		if (default_value_opt)
			clone->default_value_opt =
			(AstMemberValue*)default_value_opt->Clone(ast_pool);
		if (method_body_opt)
			clone->method_body_opt =
			(AstMethodBody*)method_body_opt->Clone(ast_pool);
		clone->semicolon_token_opt = semicolon_token_opt;
		return clone;
	}


	
	Token* MethodDeclaration::LeftToken()
	{
		if (modifiers_opt)
			return  modifiers_opt->LeftToken();
		else if (type_parameters_opt)
			return type_parameters_opt->left_angle_token;
		else if (type)
			return type->LeftToken();
		else
			return nullptr;
	}

	Token* MethodDeclaration::RightToken()
	{
		if (method_body_opt)
			return  method_body_opt->right_brace_token;
		else if (semicolon_token_opt)
			return  semicolon_token_opt;
		else
			return method_declarator->RightToken();
	}

	

	
	

	vector<wstring> MethodDeclaration::getReturnTypeName()
	{
		return type->getTypeName();
	}

	void   MethodDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	this->parent_scope = scope;
	if (visitor->visit(this,scope)) {
		unsigned i;
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		if (type_parameters_opt)
			type_parameters_opt->traverse(visitor, scope);
		type->traverse(visitor, scope);
		method_declarator->traverse(visitor, scope);
		unsigned num = NumThrows();
		for (i = 0; i < num; i++)
			(*throws)[i]->traverse(visitor, scope);
		if (default_value_opt)
			default_value_opt->traverse(visitor, scope);
		if (method_body_opt)
			method_body_opt->traverse(visitor, this);
	}
	visitor->endVisit(this,scope);
}

	unsigned MethodDeclaration::BodyStart()
	{
		if (method_body_opt)
			return 	method_body_opt->LeftToken()->start_location;
		return 0;
	}

	unsigned MethodDeclaration::BodyEnd()
	{
		if (method_body_opt)
			return 	method_body_opt->RightToken()->EndLocation();
		return 0;
	}

	AstTypeName*& MethodDeclaration::Throw(unsigned i) const
	{ return (*throws)[i]; }

	unsigned MethodDeclaration::NumThrows()
	{ return throws ? throws->Length() : 0; }

	void MethodDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (MethodDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << " <#"
            << (type_parameters_opt ? type_parameters_opt -> id : 0)
            << "> #" << type -> id << " #" << method_declarator -> id
            << " throws: (";
    for (i = 0; i < NumThrows(); i++)
        os << " #" << Throw(i) -> id;
    os << ") default #"
            << (default_value_opt ? default_value_opt -> id : 0) << ' '
            << (method_body_opt ? method_body_opt -> id : 0) << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    if (type_parameters_opt)
        type_parameters_opt -> Print(os, lex_stream);
    type -> Print(os, lex_stream);
    method_declarator -> Print(os, lex_stream);
    for (i = 0; i < NumThrows(); i++)
        Throw(i) -> Print(os, lex_stream);
    if (default_value_opt)
        default_value_opt -> Print(os, lex_stream);
    if (method_body_opt)
        method_body_opt -> Print(os, lex_stream);
}

	 void MethodDeclaration::AllocateThrows(unsigned estimate)
	{
		assert(!throws);
		throws = new (pool) AstArray<AstTypeName*>(pool, estimate);
	}

	 void MethodDeclaration::AddThrow(AstTypeName* exception)
	{
		assert(throws);
		throws->Next() = exception;
	}

} // Close namespace Jikes block


#endif // AstMethodDeclaration_def_INCLUDED
