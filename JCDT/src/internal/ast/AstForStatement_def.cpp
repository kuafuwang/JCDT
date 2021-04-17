#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstExpressionStatement.h>
#include <JCDT_Lib/internal/ast/AstBlock.h>
#ifndef AstForStatement_def_INCLUDED
#define AstForStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstForStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block




void ForStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    unsigned i;
    if (debug_unparse)
        os << "/*ForStatement:#" << id << "*/";
    os << lex_stream -> NameString(for_token->index) << " (";
    if (NumForInitStatements())
        ForInitStatement(0) -> Unparse(os, lex_stream);
    else os << ' ';
    for (i = 1; i < NumForInitStatements(); i++)
    {
        os << ", ";
        ForInitStatement(i) -> Unparse(os, lex_stream);
    }
    os << "; ";
    if (end_expression_opt)
        end_expression_opt -> Unparse(os, lex_stream);
    os << "; ";
    if (NumForUpdateStatements())
        ForUpdateStatement(0) -> Unparse(os, lex_stream);
    for (i = 1; i < NumForUpdateStatements(); i++)
    {
        os << ", ";
        ForUpdateStatement(i) -> Unparse(os, lex_stream);
    }
    os << ')' << endl;
    statement -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:ForStatement#" << id << "*/";
}


	Token* ForStatement::RightToken()
	{ return statement->right_brace_token; }

	void   ForStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumForInitStatements();
		for (i = 0; i < num; i++)
			(*for_init_statements)[i]->traverse(visitor, scope);
		if (end_expression_opt)
			end_expression_opt->traverse(visitor, scope);
		num = NumForUpdateStatements();
		for (i = 0; i < num; i++)
			(*for_update_statements)[i]->traverse(visitor, scope);
		statement->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	ForStatement::ForStatement(AstStoragePool* p): ReachComleteAbleStatement(FOR)
 , pool(p), for_init_statements(nullptr), for_update_statements(nullptr), 
		for_token(nullptr), end_expression_opt(nullptr), statement(nullptr)
	{}

	ReachComleteAbleStatement*& ForStatement::ForInitStatement(unsigned i)
	{
		return (*for_init_statements)[i];
	}

	unsigned ForStatement::NumForInitStatements()
	{
		return for_init_statements ? for_init_statements->Length() : 0;
	}

	AstExpressionStatement*& ForStatement::ForUpdateStatement(unsigned i)
	{
		return (*for_update_statements)[i];
	}

	unsigned ForStatement::NumForUpdateStatements()
	{
		return for_update_statements ? for_update_statements->Length() : 0;
	}

	void ForStatement::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (ForStatement):  ("
            << lex_stream.NameString(for_token->index);
    for (i = 0; i < NumForInitStatements(); i++)
        os << " #" << ForInitStatement(i) -> id;
    os << "; #" << (end_expression_opt ? end_expression_opt -> id : 0)
            << ';';
    for (i = 0; i < NumForUpdateStatements(); i++)
        os << " #" << ForUpdateStatement(i) -> id;
    os << ") #" << statement -> id << endl;

    for (i = 0; i < NumForInitStatements(); i++)
        ForInitStatement(i) -> Print(os, lex_stream);
    if (end_expression_opt)
        end_expression_opt -> Print(os, lex_stream);
    for (i = 0; i < NumForUpdateStatements(); i++)
        ForUpdateStatement(i) -> Print(os, lex_stream);
    statement -> Print(os, lex_stream);
}



Ast* ForStatement::Clone(AstStoragePool* ast_pool)
{
    unsigned i;
    ForStatement* clone = ast_pool -> GenForStatement();
    clone -> for_token = for_token;
    clone -> AllocateForInitStatements(NumForInitStatements());
    for (i = 0; i < NumForInitStatements(); i++)
        clone -> AddForInitStatement((ReachComleteAbleStatement*)
                                     ForInitStatement(i) -> Clone(ast_pool));
    if (end_expression_opt)
        clone -> end_expression_opt =
            (Expression*) end_expression_opt -> Clone(ast_pool);
    clone -> AllocateForUpdateStatements(NumForUpdateStatements());
    for (i = 0; i < NumForUpdateStatements(); i++)
        clone -> AddForUpdateStatement((AstExpressionStatement*)
                                       ForUpdateStatement(i) ->
                                       Clone(ast_pool));
    clone -> statement = (AstBlock*) statement -> Clone(ast_pool);
    return clone;
}

} // Close namespace Jikes block


#endif // AstForStatement_def_INCLUDED
