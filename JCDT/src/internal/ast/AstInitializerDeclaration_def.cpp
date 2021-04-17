#include <JCDT_Lib/internal/ast/AstModifiers.h>
#include <JCDT_Lib/internal/ast/AstMethodBody.h>
#ifndef AstInitializerDeclaration_def_INCLUDED
#define AstInitializerDeclaration_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstInitializerDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


	void Initializer::Unparse(Ostream& os, LexStream* lex_stream)
	{
		if (debug_unparse)
			os << "/*Initializer:#" << id << "*/";
		if (modifiers_opt)
			modifiers_opt->Unparse(os, lex_stream);
		block->Unparse(os, lex_stream);
		if (debug_unparse)
			os << "/*:Initializer#" << id << "*/";
	}

	Ast* Initializer::Clone(AstStoragePool* ast_pool)
	{
		Initializer* clone = ast_pool->GenInitializerDeclaration();
		clone->other_tag = other_tag;
		if (modifiers_opt)
			clone->modifiers_opt =
			(AstModifiers*)modifiers_opt->Clone(ast_pool);
		clone->block = (AstMethodBody*)block->Clone(ast_pool);

		return clone;
	}

	
	Token* Initializer::LeftToken()
	{
		return modifiers_opt ? modifiers_opt->LeftToken()
			       : block->left_brace_token;
	}

	Token* Initializer::RightToken()
	{ return block->right_brace_token; }

	void   Initializer::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		if (modifiers_opt)
			modifiers_opt->traverse(visitor, scope);
		block->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void Initializer::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (InitializerDeclaration):  #"
            << (modifiers_opt ? modifiers_opt -> id : 0)
            << " #" << block -> id << endl;
    if (modifiers_opt)
        modifiers_opt -> Print(os, lex_stream);
    block -> Print(os, lex_stream);
}

} // Close namespace Jikes block


#endif // AstInitializerDeclaration_def_INCLUDED
