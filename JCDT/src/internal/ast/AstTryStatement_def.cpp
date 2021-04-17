#include <JCDT_Lib/internal/ast/AstBlock.h>
#include <JCDT_Lib/internal/ast/AstFinallyClause.h>
#include <JCDT_Lib/internal/ast/AstCatchClause.h>
#ifndef AstTryStatement_def_INCLUDED
#define AstTryStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTryStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void TryStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*TryStatement:#" << id << "*/";
    os << lex_stream -> NameString(try_token->index) << endl;
    block -> Unparse(os, lex_stream);
    for (unsigned i = 0; i < NumCatchClauses(); i++)
        CatchClause(i) -> Unparse(os, lex_stream);
    if (finally_clause_opt)
        finally_clause_opt -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:TryStatement#" << id << "*/";
}

Ast* TryStatement::Clone(AstStoragePool* ast_pool)
{
    TryStatement* clone = ast_pool -> GenTryStatement();
    clone -> try_token = try_token;
    clone -> block = (AstBlock*) block -> Clone(ast_pool);
    clone -> AllocateCatchClauses(NumCatchClauses());
    for (unsigned i = 0; i < NumCatchClauses(); i++)
        clone -> AddCatchClause((AstCatchClause*) CatchClause(i) ->
                                Clone(ast_pool));
    if (finally_clause_opt)
        clone -> finally_clause_opt =
            (AstFinallyClause*) finally_clause_opt -> Clone(ast_pool);
    return clone;
}
	Token* TryStatement::RightToken()
	{
		return finally_clause_opt ? finally_clause_opt->RightToken()
			       : CatchClause(NumCatchClauses() - 1)->RightToken();
	}

	void   TryStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		block->traverse(visitor, scope);
		unsigned num = NumCatchClauses();
		for (i = 0; i < num; i++)
			(*catch_clauses)[i]->traverse(visitor, scope);
		if (finally_clause_opt)
			finally_clause_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	TryStatement::TryStatement(AstStoragePool* p): 
		ReachComleteAbleStatement(TRY_KIND), pool(p),
		catch_clauses(nullptr), 
		try_token(nullptr), block(nullptr),
		finally_clause_opt(nullptr), 
		processing_try_block(false)
	{}

	void TryStatement::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (TryStatement):  "
            << lex_stream.NameString(try_token->index)
            << " #" << block -> id
            << " catch (";
    for (i = 0; i < NumCatchClauses(); i++)
        os << " #" << CatchClause(i) -> id;
    os << ") finally #"
            << (finally_clause_opt ? finally_clause_opt -> id : 0) << endl;

    block -> Print(os, lex_stream);
    for (i = 0; i < NumCatchClauses(); i++)
        CatchClause(i) -> Print(os, lex_stream);
    if (finally_clause_opt)
        finally_clause_opt -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstTryStatement_def_INCLUDED
