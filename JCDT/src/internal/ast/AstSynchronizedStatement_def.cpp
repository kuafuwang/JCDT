#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstBlock.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstSynchronizedStatement_def_INCLUDED
#define AstSynchronizedStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSynchronizedStatement.h"
namespace Jikes { // Open namespace Jikes block




void SynchronizedStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*SynchronizedStatement:#" << id << "*/";
    os << lex_stream -> NameString(synchronized_token->index) << " (";
    expression -> Unparse(os, lex_stream);
    os << ')' << endl;
    block -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:SynchronizedStatement#" << id << "*/";
}
Ast* SynchronizedStatement::Clone(AstStoragePool* ast_pool)
{
    SynchronizedStatement* clone = ast_pool -> GenSynchronizedStatement();
    clone -> synchronized_token = synchronized_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> block = (AstBlock*) block -> Clone(ast_pool);
    return clone;
}

	Token* SynchronizedStatement::RightToken()
	{ return block->right_brace_token; }

	void   SynchronizedStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		expression->traverse(visitor, scope);
		block->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void SynchronizedStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (SynchronizedStatement):  "
            << lex_stream.NameString(synchronized_token->index)
            << " ( #" << expression -> id
            << " ) #" << block -> id << endl;
    expression -> Print(os, lex_stream);
    block -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstSynchronizedStatement_def_INCLUDED
