#ifndef AstEmptyDeclaration_def_INCLUDED
#define AstEmptyDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstEmptyDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


    void EmptyTypeDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
        os << "/*EmptyTypeDeclaration:#" << id << "*/";
        os << lex_stream -> NameString(semicolon_token->index) << endl;
        if (debug_unparse)
        os << "/*:EmptyTypeDeclaration#" << id << "*/";
    }

	Ast* EmptyTypeDeclaration::Clone(AstStoragePool* ast_pool)
	{
		return ast_pool->GenEmptyDeclaration(semicolon_token);
	}


    void   EmptyTypeDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
    {
        if (visitor->visit(this,scope)) {

        }
        visitor->endVisit(this,scope);
    }
    void EmptyTypeDeclaration::Print(Ostream& os,LexStream& lex_stream)
    {
        os << '#' << id << " (EmptyDeclaration):  "
                << lex_stream.NameString(semicolon_token->index) << endl;
    }


} // Close namespace Jikes block


#endif // AstEmptyDeclaration_def_INCLUDED
