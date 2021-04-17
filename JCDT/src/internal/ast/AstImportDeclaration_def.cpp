#include <JCDT_Lib/internal/ast/AstName.h>
#ifndef AstImportDeclaration_def_INCLUDED
#define AstImportDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstImportDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

    void AstImportDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstImportDeclaration:#" << id << "*/";
        os << lex_stream -> NameString(import_token->index) << ' ';
        if (static_token_opt)
            os << lex_stream -> NameString(static_token_opt->index) << ' ';
        name -> Unparse(os, lex_stream);
        if (star_token_opt)
            os << '.' << lex_stream -> NameString(star_token_opt->index);
        os << ';' << endl;
        if (debug_unparse)
            os << "/*:AstImportDeclaration#" << id << "*/";
    }

	Ast* AstImportDeclaration::Clone(AstStoragePool* ast_pool)
	{
		AstImportDeclaration* clone = ast_pool->GenImportDeclaration();
		clone->import_token = import_token;
		clone->static_token_opt = static_token_opt;
		clone->name = (AstName*)name->Clone(ast_pool);
		clone->star_token_opt = star_token_opt;
		clone->semicolon_token = semicolon_token;
		return clone;
	}
	

	
void   AstImportDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		name->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	bool AstImportDeclaration::onDemand()
	{
		if(star_token_opt)
		{
			return true;
		}
		if (!name->IsQualifiedName())
			return true;
		return false;
	}

	wstring AstImportDeclaration::getFullName() const
	{
		auto _ret = getImportName();
		if (star_token_opt)
			_ret.append(L".*");
		return _ret;
	}

	void AstImportDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ImportDeclaration):  ";
    if (static_token_opt)
        os << lex_stream.NameString(static_token_opt->index) << ' ';
    os << lex_stream.NameString(import_token->index)
            << " #" << name -> id;
    if (star_token_opt)
        os << '.' << lex_stream.NameString(star_token_opt->index);
    os << endl;
    name -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstImportDeclaration_def_INCLUDED
