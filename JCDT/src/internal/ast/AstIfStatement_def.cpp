#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstBlock.h>
#ifndef AstIfStatement_def_INCLUDED
#define AstIfStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstIfStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block




void IfStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*IfStatement:#" << id << "*/";
    os << lex_stream -> NameString(if_token->index) << '(';
    expression -> Unparse(os, lex_stream);
    os << ')' << endl;
    true_statement -> Unparse(os, lex_stream);
    if (false_statement_opt)
    {
        os << "else" << endl;
        false_statement_opt -> Unparse(os, lex_stream);
    }
    os << endl;
    if (debug_unparse)
        os << "/*:IfStatement#" << id << "*/";
}
Ast* IfStatement::Clone(AstStoragePool* ast_pool)
{
    IfStatement* clone = ast_pool -> GenIfStatement();
    clone -> if_token = if_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> true_statement = (AstBlock*) true_statement -> Clone(ast_pool);
    if (false_statement_opt)
        clone -> false_statement_opt =
            (AstBlock*) false_statement_opt -> Clone(ast_pool);
    return clone;
}



	Token* IfStatement::RightToken()
	{
		return false_statement_opt ? false_statement_opt->RightToken()
			       : true_statement->RightToken();
	}

	void   IfStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		expression->traverse(visitor, scope);
		true_statement->traverse(visitor, scope);
		if (false_statement_opt)
			false_statement_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void IfStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (IfStatement):  "
            << lex_stream.NameString(if_token->index)
            << " ( #" << expression -> id << " ) #" << true_statement -> id;
    if (false_statement_opt)
        os << " else #" << false_statement_opt -> id;
    else os << " #0";
    os << endl;

    expression -> Print(os, lex_stream);
    true_statement -> Print(os, lex_stream);
    if (false_statement_opt)
        false_statement_opt -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstIfStatement_def_INCLUDED
