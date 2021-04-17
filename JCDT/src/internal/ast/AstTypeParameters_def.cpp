#include <JCDT_Lib/internal/ast/AstTypeParameter.h>
#ifndef AstTypeParameters_def_INCLUDED
#define AstTypeParameters_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTypeParameters.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


    void AstTypeParameters::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstTypeParameters:#" << id << "*/";
        os << '<';
        TypeParameter(0) -> Unparse(os, lex_stream);
        for (unsigned i = 1; i < NumTypeParameters(); i++)
        {
            os << ", ";
            TypeParameter(i) -> Unparse(os, lex_stream);
        }
        os << '>';
        if (debug_unparse)
            os << "/*:AstTypeParameters#" << id << "*/";
    }
	Ast* AstTypeParameters::Clone(AstStoragePool* ast_pool)
	{
		AstTypeParameters* clone = ast_pool->GenTypeParameters();
		clone->left_angle_token = left_angle_token;
		clone->AllocateTypeParameters(NumTypeParameters());
		for (unsigned i = 0; i < NumTypeParameters(); i++)
			clone->AddTypeParameter((AstTypeParameter*)TypeParameter(i) ->
				Clone(ast_pool));
		clone->right_angle_token = right_angle_token;
		return clone;
	}




void   AstTypeParameters::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumTypeParameters();
		for (i = 0; i < num; i++)
			(*parameters)[i]->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstTypeParameters::AstTypeParameters(AstStoragePool* p)
		: Ast(PARAM_LIST), pool(p), parameters(nullptr),
		left_angle_token(nullptr), right_angle_token(nullptr)
	{}

	void AstTypeParameters::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (TypeParameters): <";
    for (i = 0; i < NumTypeParameters(); i++)
        os << " #" << TypeParameter(i) -> id;
    os << '>' << endl;
    for (i = 0; i < NumTypeParameters(); i++)
        TypeParameter(i) -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstTypeParameters_def_INCLUDED
