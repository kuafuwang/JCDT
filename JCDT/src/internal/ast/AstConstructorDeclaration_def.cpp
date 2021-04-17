#ifndef JIkes_AstConstructorDeclaration_def_INCLUDED
#define JIkes_AstConstructorDeclaration_def_INCLUDED

#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstTypeParameters.h>
#include <JCDT_Lib/internal/ast/AstMethodDeclarator.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>
#include <JCDT_Lib/internal/ast/AstMethodBody.h>

#include "JCDT_Lib/internal/ast/AstConstructorDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include  <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block



void ConstructorDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ConstructorDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    if (type_parameters_opt)
        type_parameters_opt -> Unparse(os, lex_stream);
    method_declarator -> Unparse(os, lex_stream);
    if (NumThrows())
    {
        os << " throws ";
        Throw(0) -> Unparse(os, lex_stream);
        for (unsigned i = 1; i < NumThrows(); i++)
        {
            os << ", ";
            Throw(i) -> Unparse(os, lex_stream);
        }
    }
    constructor_body -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:ConstructorDeclaration#" << id << "*/";
}

	Ast* ConstructorDeclaration::Clone(AstStoragePool* ast_pool)
	{
		ConstructorDeclaration* clone = ast_pool->GenConstructorDeclaration();
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		if (type_parameters_opt)
			clone->type_parameters_opt =
			(AstTypeParameters*)type_parameters_opt->Clone(ast_pool);
		clone->method_declarator =
			(AstMethodDeclarator*)method_declarator->Clone(ast_pool);
		clone->AllocateThrows(NumThrows());
		for (unsigned i = 0; i < NumThrows(); i++)
			clone->AddThrow((AstTypeName*)Throw(i)->Clone(ast_pool));
		clone->constructor_body =
			(AstMethodBody*)constructor_body->Clone(ast_pool);
		return clone;
	}


void   ConstructorDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	this->parent_scope = scope;
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		if (type_parameters_opt)
			type_parameters_opt->traverse(visitor, scope);
		method_declarator->traverse(visitor, scope);
		unsigned num = NumThrows();
		for (unsigned i = 0; i < num; i++)
			(*throws)[i]->traverse(visitor, scope);
		constructor_body->traverse(visitor, this);
	}
	visitor->endVisit(this,scope);
}



	Token* ConstructorDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : type_parameters_opt ? type_parameters_opt->left_angle_token
			       : method_declarator->identifier_token;
	}

	Token* ConstructorDeclaration::RightToken()
	{
		return constructor_body->right_brace_token;
	}

	
	


	
	void ConstructorDeclaration::AllocateThrows(unsigned estimate)
	{
		assert(!throws);
		throws = new (pool) AstArray<AstTypeName*>(pool, estimate);
	}

	unsigned ConstructorDeclaration::BodyStart()
	{
		return constructor_body->LeftToken()->start_location;
		
	}

	unsigned ConstructorDeclaration::BodyEnd()
	{
		return constructor_body->RightToken()->EndLocation();

	}

	ConstructorDeclaration::ConstructorDeclaration(AstStoragePool* p):
		AbstractMethodDeclaration(CONSTRUCTOR)
	     , pool(p), throws(nullptr), index(ConstructorCycleChecker::OMEGA), 
		 constructor_body(nullptr)
	{}

	void ConstructorDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (ConstructorDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << " <#"
            << (type_parameters_opt ? type_parameters_opt -> id : 0)
            << " #" << method_declarator -> id << " throws: (";
    for (i = 0; i < NumThrows(); i++)
        os << " #" << Throw(i) -> id;
    os << ") #" << constructor_body -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    if (type_parameters_opt)
        type_parameters_opt -> Print(os, lex_stream);
    method_declarator -> Print(os, lex_stream);
    for (i = 0; i < NumThrows(); i++)
        Throw(i) -> Print(os, lex_stream);
    constructor_body -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstConstructorDeclaration_def_INCLUDED
