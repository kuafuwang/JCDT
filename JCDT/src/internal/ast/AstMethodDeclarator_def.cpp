#include <JCDT_Lib/internal/ast/AstFormalParameter.h>
#include <JCDT_Lib/internal/ast/AstBrackets.h>
#ifndef AstMethodDeclarator_def_INCLUDED
#define AstMethodDeclarator_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstMethodDeclarator.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstMethodDeclarator::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstMethodDeclarator:#" << id << "*/";
    os << lex_stream -> NameString(identifier_token->index) << '(';
    if (NumFormalParameters())
        FormalParameter(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumFormalParameters(); i++)
    {
        os << ", ";
        FormalParameter(i) -> Unparse(os, lex_stream);
    }
    os << ')';
    if (brackets_opt)
        brackets_opt -> Unparse(os, lex_stream);
    os << ' ';
    if (debug_unparse)
        os << "/*:AstMethodDeclarator#" << id << "*/";
}

	Ast* AstMethodDeclarator::Clone(AstStoragePool* ast_pool)
	{
		AstMethodDeclarator* clone = ast_pool->GenMethodDeclarator();
		clone->identifier_token = identifier_token;
		clone->left_parenthesis_token = left_parenthesis_token;
		clone->AllocateFormalParameters(NumFormalParameters());
		for (unsigned i = 0; i < NumFormalParameters(); i++)
			clone->AddFormalParameter((AstFormalParameter*)
				FormalParameter(i)->Clone(ast_pool));
		clone->right_parenthesis_token = right_parenthesis_token;
		if (brackets_opt)
			clone->brackets_opt = (AstBrackets*)brackets_opt->Clone(ast_pool);
		return clone;
	}

	

	Token* AstMethodDeclarator::RightToken()
	{
		return brackets_opt ? brackets_opt->right_bracket_token
			       : right_parenthesis_token;
	}

	AstMethodDeclarator::AstMethodDeclarator(AstStoragePool* p): Ast(METHOD_DECLARATOR)
	                                                             , pool(p), formal_parameters(nullptr), identifier_token(nullptr), left_parenthesis_token(nullptr), right_parenthesis_token(nullptr), brackets_opt(nullptr)
	{}

	unsigned AstMethodDeclarator::NumBrackets()
	{
		return brackets_opt ? brackets_opt->dims : 0;
	}
	void   AstMethodDeclarator::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumFormalParameters();
		for (i = 0; i < num; i++)
			FormalParameter(i)->traverse(visitor, scope);
		if (brackets_opt)
			brackets_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	

	void AstMethodDeclarator::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (MethodDeclarator):  "
            << lex_stream.NameString(identifier_token->index)
            << " (";
    for (i = 0; i < NumFormalParameters(); i++)
        os << " #" << FormalParameter(i) -> id;
    os << " ) #" << (brackets_opt ? brackets_opt -> id : 0) << endl;
    for (i = 0; i < NumFormalParameters(); i++)
        FormalParameter(i) -> Print(os, lex_stream);
    if (brackets_opt)
        brackets_opt -> Print(os, lex_stream);
}
} // Close namespace Jikes block


#endif // AstMethodDeclarator_def_INCLUDED
