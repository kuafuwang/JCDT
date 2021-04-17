#ifndef AstBreakStatement_def_INCLUDED
#define AstBreakStatement_def_INCLUDED
#define AstBrackets_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstBreakStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block



void BreakStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*BreakStatement:#" << id << "*/";
    os << lex_stream -> NameString(break_token->index);
    if (identifier_token_opt)
    {
        os << ' ' << lex_stream -> NameString(identifier_token_opt->index);
    }
    os << ';' << endl;
    if (debug_unparse)
        os << "/*:BreakStatement#" << id << "*/";
}

Ast* BreakStatement::Clone(AstStoragePool* ast_pool)
{
    BreakStatement* clone = ast_pool -> GenBreakStatement();
    clone -> break_token = break_token;
    clone -> identifier_token_opt = identifier_token_opt;
    clone -> semicolon_token = semicolon_token;
    clone -> nesting_level = nesting_level;
    return clone;
}

	BreakStatement::BreakStatement()
		: ReachComleteAbleStatement(BREAK), break_token(nullptr), 
		identifier_token_opt(nullptr), semicolon_token(nullptr), 
		nesting_level(0)
	{}

	void BreakStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (BreakStatement):  "
            << lex_stream.NameString(break_token->index) << ' '
            << (identifier_token_opt
                ? lex_stream.NameString(identifier_token_opt->index) : L"")
            << " at nesting_level " << nesting_level << endl;
}
void  BreakStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}


} // Close namespace Jikes block


#endif // AstBreakStatement_def_INCLUDED
