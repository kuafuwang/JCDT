#include <JCDT_Lib/internal/ast/AstAnnotationDeclaration.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/ast/AstClassDeclaration.h>
#ifndef AstLocalClassStatement_def_INCLUDED
#define AstLocalClassStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstLocalClassStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstEnumDeclaration.h>
namespace Jikes { // Open namespace Jikes block



void AstLocalClassStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstLocalClassStatement:#" << id << "*/";
    declaration -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstLocalClassStatement#" << id << "*/";
}

Ast* AstLocalClassStatement::Clone(AstStoragePool* ast_pool)
{
    Ast* p = declaration -> Clone(ast_pool);
    if (ClassTypeDeclaration::ClassDeclarationCast(p))
        return ast_pool -> GenLocalClassStatement((ClassTypeDeclaration*) p);
    else return ast_pool -> GenLocalClassStatement((EnumTypeDeclaration*) p);
}



	Token* AstLocalClassStatement::LeftToken()
	{ return declaration->LeftToken(); }

	Token* AstLocalClassStatement::RightToken()
	{
		return declaration->class_body->right_brace_token;
	}

	void   AstLocalClassStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		declaration->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstLocalClassStatement::AstLocalClassStatement(ClassTypeDeclaration* decl): ReachComleteAbleStatement(LOCAL_CLASS, false, true)
	                                                                            , declaration(decl)
{
	if (decl)
		decl->MarkLocalLevel();
}

	AstLocalClassStatement::AstLocalClassStatement(EnumTypeDeclaration* decl)
		: ReachComleteAbleStatement(LOCAL_CLASS, false, true) , declaration(decl)
{
	if (decl)
		decl->MarkLocalLevel();
}

	void AstLocalClassStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (LocalClassStatement): #"
            << declaration -> id << endl;
    declaration -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstLocalClassStatement_def_INCLUDED
