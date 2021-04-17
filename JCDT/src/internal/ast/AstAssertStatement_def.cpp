#include <JCDT_Lib/internal/ast/AstExpression.h>

#ifndef AstAssertStatement_def_INCLUDED
#define AstAssertStatement_def_INCLUDED
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include "JCDT_Lib/internal/ast/AstAssertStatement.h"
namespace Jikes { // Open namespace Jikes block



void AssertStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AssertStatement:#" << id << "*/";
    os << lex_stream -> NameString(assert_token->index) << ' ';
    condition -> Unparse(os, lex_stream);
    if (message_opt)
    {
        os << " : ";
        message_opt -> Unparse(os, lex_stream);
    }
    os << ';' << endl;
    if (debug_unparse)
        os << "/*:AssertStatement#" << id << "*/";
}
Ast* AssertStatement::Clone(AstStoragePool* ast_pool)
{
    AssertStatement* clone = ast_pool -> GenAssertStatement();
    clone -> assert_token = assert_token;
    clone -> condition = (Expression*) condition -> Clone(ast_pool);
    if (message_opt)
        clone -> message_opt = (Expression*) message_opt -> Clone(ast_pool);
    clone -> semicolon_token = semicolon_token;
    return clone;
}

	AssertStatement::AssertStatement()
		: ReachComleteAbleStatement(ASSERT_KIND), assert_token(nullptr), 
		semicolon_token(nullptr), condition(nullptr),
		message_opt(nullptr), assert_variable(nullptr)
	{}

	void AssertStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (AssertStatement):  "
            << lex_stream.NameString(assert_token->index)
            << " ( #" << condition -> id;
    if (message_opt)
        os << " : " << message_opt -> id;
    else os << " #0";
    os << " ;" << endl;
    condition -> Print(os, lex_stream);
    if (message_opt)
        message_opt -> Print(os, lex_stream);
}
void  AssertStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		condition->traverse(visitor,scope);
		if (message_opt)
			message_opt->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}


} // Close namespace Jikes block


#endif // AstAssertStatement_def_INCLUDED
