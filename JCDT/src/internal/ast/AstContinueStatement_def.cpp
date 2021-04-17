#ifndef AstContinueStatement_def_INCLUDED
#define AstContinueStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstContinueStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void ContinueStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*ContinueStatement:#" << id << "*/";
    os << lex_stream -> NameString(continue_token->index);
    if (identifier_token_opt)
    {
        os << ' ' << lex_stream -> NameString(identifier_token_opt->index);
    }
    os << ';' << endl;
    if (debug_unparse)
        os << "/*:ContinueStatement#" << id << "*/";
}
Ast* ContinueStatement::Clone(AstStoragePool* ast_pool)
{
    ContinueStatement* clone = ast_pool -> GenContinueStatement();
    clone -> continue_token = continue_token;
    clone -> identifier_token_opt = identifier_token_opt;
    clone -> semicolon_token = semicolon_token;
    clone -> nesting_level = nesting_level;
    return clone;
}


void   ContinueStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {

	}
	visitor->endVisit(this,scope);
}

	ContinueStatement::ContinueStatement(): ReachComleteAbleStatement(CONTINUE), continue_token(nullptr), identifier_token_opt(nullptr), semicolon_token(nullptr), nesting_level(0)
	{}

	void ContinueStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (ContinueStatement):  "
            << lex_stream.NameString(continue_token->index) << ' '
            << (identifier_token_opt
                ? lex_stream.NameString(identifier_token_opt->index) : L"")
            << " at nesting_level " << nesting_level << endl;
}



} // Close namespace Jikes block


#endif // AstContinueStatement_def_INCLUDED
