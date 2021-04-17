#include <JCDT_Lib/internal/lookup/BlockSymbol.h>
#ifndef AstBlock_def_INCLUDED
#define AstBlock_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstBlock.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


	void AstBlock::Reset()
	{
		if(block_statements)
			block_statements = new (pool) AstArray<ReachComleteAbleStatement*>(pool, 1);
		defined_variables = nullptr;
		block_symbol = nullptr;
	}

	AstBlock::AstBlock(AstStoragePool* p, AstKind k, bool reachable)
		: ReachComleteAbleStatement(k, reachable), pool(p),
		block_statements(nullptr),
		defined_variables(nullptr), block_symbol(nullptr), 
		nesting_level(0), label_opt(nullptr),
		left_brace_token(nullptr), right_brace_token(nullptr), 
		no_braces(false)
	{}

	void AstBlock::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (";
    if (label_opt)
        os << lex_stream.NameString(label_opt->index) << ": ";
    os << "Block at level " << nesting_level;
    if (block_symbol)
        os << ", max_variable_index "
                << block_symbol -> max_variable_index
                << ", helper_variable_index "
                << block_symbol -> helper_variable_index;
    else os << ", BLOCK_SYMBOL NOT SET";
    os << ')';

    if (NumStatements() > 0)
    {
        os << "    {";
        for (i = 0; i < NumStatements(); i++)
        {
            if (i % 10 == 0)
                os << endl << "        ";
            os << " #" << GetStatement(i) -> id;
        }
        os << "    }" << endl;
        for (i = 0; i < NumStatements(); i++)
            GetStatement(i) -> Print(os, lex_stream);
    }
    else os << endl;
}
void  AstBlock::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	this->parent_scope = scope;
	if (visitor->visit(this,scope)) {

			unsigned i;
			unsigned num = NumStatements();
			for (i = 0; i < num; i++)
				(*block_statements)[i]->traverse(visitor,this);
		
	}
	visitor->endVisit(this,scope);
}

	Ast* AstBlock::Clone(AstStoragePool* ast_pool)
	{
		AstBlock* clone = ast_pool->GenBlock();
		if(!ast_pool->IsCloneForStruct())
			clone->CloneBlock(ast_pool, this);
		return clone;
	}

	void AstBlock::CloneBlock(AstStoragePool* ast_pool, AstBlock* orig)
	{
		other_tag = orig->other_tag;
		label_opt = orig->label_opt;
		nesting_level = orig->nesting_level;
		left_brace_token = orig->left_brace_token;
		owner_ast = orig->owner_ast;
		unsigned count = orig->NumStatements();
		AllocateStatements(count);
		for (unsigned i = 0; i < count; i++)
			AddStatement((ReachComleteAbleStatement*)orig->GetStatement(i)->Clone(ast_pool));
		right_brace_token = orig->right_brace_token;
		no_braces = orig->no_braces;
	}

	Symbol* AstBlock::GetSymbol()
	{
		return block_symbol;
	}


	void AstBlock::Unparse(Ostream& os, LexStream* lex_stream)
	{
		if (debug_unparse)
			os << "/*AstBlock:#" << id << "*/"
			<< "/*no_braces:" << no_braces << "*/";
		if (label_opt)
			os << lex_stream->NameString(label_opt->index) << ": ";
		if (!no_braces)
			os << '{' << endl;
		for (unsigned i = 0; i < NumStatements(); i++)
			GetStatement(i)->Unparse(os, lex_stream);
		if (!no_braces)
			os << '}' << endl;
		if (debug_unparse)
			os << "/*:AstBlock#" << id << "*/";
	}

	

} // Close namespace Jikes block


#endif // AstBlock_def_INCLUDED
