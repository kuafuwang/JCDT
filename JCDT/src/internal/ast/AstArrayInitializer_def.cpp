#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#ifndef AstArrayInitializer_def_INCLUDED
#define AstArrayInitializer_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstArrayInitializer.h"
namespace Jikes { // Open namespace Jikes block


void AstArrayInitializer::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstArrayInitializer:#" << id << "*/";
    os << endl << "{ ";
    if (NumVariableInitializers())
        VariableInitializer(0) -> Unparse(os, lex_stream);
    for (unsigned i = 1; i < NumVariableInitializers(); i++)
    {
        os << ", ";
        VariableInitializer(i) -> Unparse(os, lex_stream);
    }
    os << " }";
    if (debug_unparse)
        os << "/*:AstArrayInitializer#" << id << "*/";
}
	Ast* AstArrayInitializer::Clone(AstStoragePool* ast_pool)
	{
		AstArrayInitializer* clone = ast_pool->GenArrayInitializer();
		clone->left_brace_token = left_brace_token;
		clone->AllocateVariableInitializers(NumVariableInitializers());
		for (unsigned i = 0; i < NumVariableInitializers(); i++)
			clone->AddVariableInitializer((AstMemberValue*)
				VariableInitializer(i) ->
				Clone(ast_pool));
		clone->right_brace_token = right_brace_token;
		return clone;
	}


	AstArrayInitializer::AstArrayInitializer(AstStoragePool* p)
		: AstMemberValue(ARRAY_INITIALIZER) , pool(p), variable_initializers(nullptr), left_brace_token(nullptr), right_brace_token(nullptr)
	{}

	void AstArrayInitializer::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (ArrayInitializer):  "
            << endl << "    {";
    for (i = 0; i < NumVariableInitializers(); i++)
    {
        if (i % 10 == 0)
            os << endl << "       ";
        os << " #" << VariableInitializer(i) -> id;
    }
    os << endl << "    }" << endl;

    for (i = 0; i < NumVariableInitializers(); i++)
        VariableInitializer(i) -> Print(os, lex_stream);
}
void  AstArrayInitializer::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned num = NumVariableInitializers();
		for (unsigned i = 0; i <num ; i++)
			(*variable_initializers)[i]->traverse(visitor,scope);
	}
	visitor->endVisit(this,scope);
}

} // Close namespace Jikes block


#endif // AstArrayInitializer_def_INCLUDED
