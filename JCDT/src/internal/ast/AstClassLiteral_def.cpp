#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstTypeName.h>
#ifndef AstClassLiteral_def_INCLUDED
#define AstClassLiteral_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstClassLiteral.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void ClassLiteralAccess::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ClassLiteralAccess:#" << id << "*/";
    type -> Unparse(os, lex_stream);
    os << '.' << lex_stream -> NameString(class_token->index);
    if (debug_unparse)
        os << "/*:ClassLiteralAccess#" << id << "*/";
}

	ClassLiteralAccess::ClassLiteralAccess(Token* token): Expression(CLASS_LITERAL), type(nullptr)
	                                                      , class_token(token), resolution_opt(nullptr)
	{}

	void ClassLiteralAccess::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ClassLiteral):  #" << type -> id << ". "
            << lex_stream.NameString(class_token->index) << endl;
    type -> Print(os, lex_stream);
}
void  ClassLiteralAccess::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		type->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	Token* ClassLiteralAccess::LeftToken()
	{ return type->LeftToken(); }

	Ast* ClassLiteralAccess::Clone(AstStoragePool* ast_pool)
{
    ClassLiteralAccess* clone = ast_pool -> GenClassLiteral(class_token);
    clone -> type = (AstTypeName*) type -> Clone(ast_pool);
    if (resolution_opt)
        clone -> resolution_opt =
            (Expression*) resolution_opt -> Clone(ast_pool);
    return clone;
}




} // Close namespace Jikes block


#endif // AstClassLiteral_def_INCLUDED
