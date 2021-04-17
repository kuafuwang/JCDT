#include <JCDT_Lib/internal/ast/AstBrackets.h>
#ifndef AstVariableDeclaratorId_def_INCLUDED
#define AstVariableDeclaratorId_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstVariableDeclaratorId.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


void AstVariableDeclaratorId::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstVariableDeclaratorId:#" << id << "*/";
    os << lex_stream -> NameString(identifier_token->index);
    if (brackets_opt)
        brackets_opt -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:AstVariableDeclaratorId#" << id << "*/";
}

	Ast* AstVariableDeclaratorId::Clone(AstStoragePool* ast_pool)
	{
		AstVariableDeclaratorId* clone = ast_pool->GenVariableDeclaratorId();
		clone->identifier_token = identifier_token;
		if (brackets_opt)
			clone->brackets_opt = (AstBrackets*)brackets_opt->Clone(ast_pool);
		return clone;
	}



	Token* AstVariableDeclaratorId::RightToken()
	{
		return brackets_opt ? brackets_opt->right_bracket_token
			       : identifier_token;
	}

	void   AstVariableDeclaratorId::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (brackets_opt)
			brackets_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	unsigned AstVariableDeclaratorId::NumBrackets()
	{
		return brackets_opt ? brackets_opt->dims : 0;
	}

	void AstVariableDeclaratorId::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (VariableDeclaratorId):  "
            << lex_stream.NameString(identifier_token->index) << " #"
            << (brackets_opt ? brackets_opt -> id : 0) << endl;
    if (brackets_opt)
        brackets_opt -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstVariableDeclaratorId_def_INCLUDED
