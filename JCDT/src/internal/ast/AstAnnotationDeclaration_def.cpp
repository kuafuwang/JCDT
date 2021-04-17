#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstAnnotationDeclaration_def_INCLUDED
#define AstAnnotationDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstAnnotationDeclaration.h"
namespace Jikes { // Open namespace Jikes block


void AnnotationTypeDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AnnotationTypeDeclaration:#" << id << "*/";
    if (lex_stream -> IsDeprecated(LeftToken()->index))
        os << "/**@deprecated*/ ";
    if (modifiers_opt)
        modifiers_opt -> Unparse(os, lex_stream);
    os << lex_stream -> NameString(interface_token->index - 1)
       << lex_stream -> NameString(interface_token->index) << ' '
       << lex_stream -> NameString(class_body -> identifier_token->index) << ' ';
    class_body -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AnnotationTypeDeclaration#" << id << "*/";
}

Ast* AnnotationTypeDeclaration::Clone(AstStoragePool* ast_pool)
{
    AnnotationTypeDeclaration* clone =
        ast_pool -> GenAnnotationDeclaration(interface_token);
    if (modifiers_opt)
        clone -> modifiers_opt =
            (AstModifiers*) modifiers_opt -> Clone(ast_pool);
    clone -> class_body = (AstClassBody*) class_body -> Clone(ast_pool);
    clone -> class_body -> owner = clone;
	clone->_level = _level;
    return clone;
}

	AnnotationTypeDeclaration::AnnotationTypeDeclaration(Token* t)
		: TypeDeclaration(ANNOTATION_TYPE) , interface_token(t)
	{}

	void AnnotationTypeDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (AnnotationDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << " @"
            << lex_stream.NameString(interface_token->index) << ' '
            << lex_stream.NameString(class_body -> identifier_token->index) << " #"
            << class_body -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    class_body -> Print(os, lex_stream);
}
void  AnnotationTypeDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	this->parent_scope = scope;
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor,scope);
	
		class_body->traverse(visitor, this);
	}
	visitor->endVisit(this,scope);
}


	Token* AnnotationTypeDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : interface_token ;
	}

	Token* AnnotationTypeDeclaration::RightToken()
	{ return class_body->right_brace_token; }

	
} // Close namespace Jikes block


#endif // AstAnnotationDeclaration_def_INCLUDED
