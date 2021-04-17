#include <JCDT_Lib/internal/ast/AstFormalParameter.h>
#include <JCDT_Lib/internal/ast/AstBlock.h>
#ifndef AstCatchClause_def_INCLUDED
#define AstCatchClause_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstCatchClause.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstCatchClause::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstCatchClause:#" << id << "*/";
    os << lex_stream -> NameString(catch_token->index) << " (";
    formal_parameter -> Unparse(os, lex_stream);
    os << ')' << endl;
    block -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstCatchClause#" << id << "*/";
}

Ast* AstCatchClause::Clone(AstStoragePool* ast_pool)
{
    AstCatchClause* clone = ast_pool -> GenCatchClause();
    clone -> catch_token = catch_token;
    clone -> formal_parameter =
        (AstFormalParameter*) formal_parameter -> Clone(ast_pool);
    clone -> block = (AstBlock*) block -> Clone(ast_pool);
    return clone;
}

	AstCatchClause::AstCatchClause(): Ast(CATCH), parameter_symbol(nullptr), catch_token(nullptr), formal_parameter(nullptr), block(nullptr)
	{}

	void AstCatchClause::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (CatchClause):  "
            << lex_stream.NameString(catch_token->index)
            << " #" << formal_parameter -> id
            << " #" << block -> id << endl;
    formal_parameter -> Print(os, lex_stream);
    block -> Print(os, lex_stream);
}
void  AstCatchClause::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		formal_parameter->traverse(visitor,scope);
		block->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}



	Token* AstCatchClause::RightToken()
	{ return block->right_brace_token; }
} // Close namespace Jikes block


#endif // AstCatchClause_def_INCLUDED
