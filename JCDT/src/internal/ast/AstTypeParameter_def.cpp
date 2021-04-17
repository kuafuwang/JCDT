#include <JCDT_Lib/internal/ast/AstTypeName.h>
#ifndef AstTypeParameter_def_INCLUDED
#define AstTypeParameter_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTypeParameter.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block

    void AstTypeParameter::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstTypeParameter:#" << id << "*/";
        os << lex_stream -> NameString(identifier_token->index);
        if (NumBounds())
        {
            os << " extends ";
            Bound(0) -> Unparse(os, lex_stream);
        }
        for (unsigned i = 1; i < NumBounds(); i++)
        {
            os << " & ";
            Bound(i) -> Unparse(os, lex_stream);
        }
        if (debug_unparse)
            os << "/*:AstTypeParameter#" << id << "*/";
    }

	Ast* AstTypeParameter::Clone(AstStoragePool* ast_pool)
	{
		AstTypeParameter* clone = ast_pool->GenTypeParameter(identifier_token);
		clone->AllocateBounds(NumBounds());
		for (unsigned i = 0; i < NumBounds(); i++)
			clone->AddBound((AstTypeName*)Bound(i)->Clone(ast_pool));
		return clone;
	}



	

	  void AstTypeParameter::AllocateBounds(unsigned estimate)
	{
		assert(!bounds);
		bounds = new (pool) AstArray<AstTypeName*>(pool, estimate);
	}
	Token* AstTypeParameter::RightToken()
	{
		return NumBounds() ? Bound(NumBounds() - 1)->RightToken()
			       : identifier_token;
	}

	void   AstTypeParameter::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumBounds();
		for (i = 0; i < num; i++)
			(*bounds)[i]->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstTypeParameter::AstTypeParameter(AstStoragePool* p, Token* token)
		: Ast(TYPE_PARAM) , pool(p), bounds(nullptr), identifier_token(token),
		symbol(nullptr)
	{}

	void AstTypeParameter::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (TypeParameter):  "
            << lex_stream.NameString(identifier_token->index) << " (";
    for (i = 0; i < NumBounds(); i++)
        os << " #" << Bound(i) -> id;
    os << ')' << endl;
    for (i = 0; i < NumBounds(); i++)
        Bound(i) -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstTypeParameter_def_INCLUDED
