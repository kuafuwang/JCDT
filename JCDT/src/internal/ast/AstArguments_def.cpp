#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#include <JCDT_Lib/internal/ast/AstArray.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstName.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#ifndef AstArguments_def_INCLUDED
#define AstArguments_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstArguments.h"
namespace Jikes { // Open namespace Jikes block


	void AstArguments::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstArguments:#" << id << "*/";
    os << lex_stream -> NameString(left_parenthesis_token->index);
    if (NumArguments())
        Argument(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumArguments(); i++)
    {
        os << ", ";
        Argument(i) -> Unparse(os, lex_stream);
    }
    os << lex_stream -> NameString(right_parenthesis_token->index);
    if (debug_unparse)
        os << "/*:AstArguments#" << id << "*/";
}

	Ast* AstArguments::Clone(AstStoragePool* ast_pool)
	{
		unsigned i;
		AstArguments* clone = ast_pool->GenArguments(left_parenthesis_token,
			right_parenthesis_token);
		clone->AllocateArguments(NumArguments());
		for (i = 0; i < NumArguments(); i++)
			clone->AddArgument((Expression*)Argument(i)->Clone(ast_pool));
		clone->AllocateLocalArguments(NumLocalArguments());
		for (i = 0; i < NumLocalArguments(); i++)
			clone->AddLocalArgument((AstName*)LocalArgument(i) ->
				Clone(ast_pool));
		clone->other_tag = other_tag;
		return clone;
	}


	AstArguments::AstArguments(AstStoragePool* p, Token* l, Token* r)
		: Ast(ARGUMENTS) , pool(p), arguments(nullptr), 
		shadow_arguments(nullptr)
	                                                                   , left_parenthesis_token(l)
	                                                                   , right_parenthesis_token(r)
	{}

	void AstArguments::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (Arguments):  (";
    for (i = 0; i < NumArguments(); i++)
        os << " #" << Argument(i) -> id;
    os << ')' << endl;
    for (i = 0; i < NumArguments(); i++)
        Argument(i) -> Print(os, lex_stream);
}

void  AstArguments::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned _args = NumArguments();
		for (i = 0; i <_args; i++)
			(*arguments)[i]->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}

 void AstArguments::AllocateLocalArguments(unsigned estimate)
{
	assert(!shadow_arguments);
	shadow_arguments = new (pool) AstArray<AstName*>(pool, estimate);
}

} // Close namespace Jikes block


#endif // AstArguments_def_INCLUDED
