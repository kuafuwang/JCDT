#include <JCDT_Lib/internal/ast/AstBlock.h>
#ifndef AstFinallyClause_def_INCLUDED
#define AstFinallyClause_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstFinallyClause.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void AstFinallyClause::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstFinallyClause:#" << id << "*/";
    os << lex_stream -> NameString(finally_token->index) << endl;
    block -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstFinallyClause#" << id << "*/";
}



	Token* AstFinallyClause::RightToken()
	{ return block->right_brace_token; }

	void   AstFinallyClause::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		block->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstFinallyClause::AstFinallyClause(): Ast(FINALLY), finally_token(nullptr), block(nullptr)
	{}

	void AstFinallyClause::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (FinallyClause):  "
            << lex_stream.NameString(finally_token->index)
            << " #" << block -> id << endl;
    block -> Print(os, lex_stream);
}



Ast* AstFinallyClause::Clone(AstStoragePool* ast_pool)
{
    AstFinallyClause* clone = ast_pool -> GenFinallyClause();
    clone -> finally_token = finally_token;
    clone -> block = (AstBlock*) block -> Clone(ast_pool);
	clone->block->owner_ast = clone;
    return clone;
}


} // Close namespace Jikes block


#endif // AstFinallyClause_def_INCLUDED
