
#ifndef AstPackageDeclaration_def_INCLUDED
#define AstPackageDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstPackageDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstName.h>
namespace Jikes { // Open namespace Jikes block


    void AstPackageDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstPackageDeclaration:#" << id << "*/";
        if (lex_stream -> IsDeprecated(LeftToken()->index))
            os << "/**@deprecated*/ ";
        if (modifiers_opt)
            modifiers_opt -> Unparse(os, lex_stream);
        os << lex_stream -> NameString(package_token->index) << ' ';
        name -> Unparse(os, lex_stream);
        os << ';' << endl;
        if (debug_unparse)
            os << "/*:AstPackageDeclaration#" << id << "*/";
    }

	Ast* AstPackageDeclaration::Clone(AstStoragePool* ast_pool)
	{
		AstPackageDeclaration* clone = ast_pool->GenPackageDeclaration();
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		clone->package_token = package_token;
		clone->name = (AstName*)name->Clone(ast_pool);
		clone->semicolon_token = semicolon_token;
		return clone;
	}


	Token* AstPackageDeclaration::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken() : package_token;
	}

	bool AstPackageDeclaration::onDemand()
	{
		return true;
	}

	void   AstPackageDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		name->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	
	AstPackageDeclaration::AstPackageDeclaration()
		: ImportReference(Ast::PACKAGE), modifiers_opt(nullptr),
		package_token(nullptr)
	{}

	void AstPackageDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (PackageDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0) << ' '
            << lex_stream.NameString(package_token->index)
            << " #" << name -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    name -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstPackageDeclaration_def_INCLUDED
