
#include <JCDT_Lib/internal/ast/AstName.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstMemberValuePair.h>
#ifndef AstAnnotation_def_INCLUDED
#define AstAnnotation_def_INCLUDED
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include "JCDT_Lib/internal/ast/AstAnnotation.h"
namespace Jikes { // Open namespace Jikes block

    void AstAnnotation::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstAnnotation:#" << id << "*/";
        os << '@';
        name -> Unparse(os, lex_stream);
        if (right_paren_token_opt)
            os << '(';
        if (NumMemberValuePairs())
        {
            assert(right_paren_token_opt);
            MemberValuePair(0) -> Unparse(os, lex_stream);
        }
        for (unsigned i = 1; i < NumMemberValuePairs(); i++)
        {
            os << ", ";
            MemberValuePair(i) -> Unparse(os, lex_stream);
        }
        if (right_paren_token_opt)
            os << ')';
        os << endl;
        if (debug_unparse)
            os << "/*:AstAnnotation#" << id << "*/";
    }

	Ast* AstAnnotation::Clone(AstStoragePool* ast_pool)
	{
		AstAnnotation* clone = ast_pool->GenAnnotation();
		clone->at_token = at_token;
		clone->name = (AstName*)name->Clone(ast_pool);
		clone->AllocateMemberValuePairs(NumMemberValuePairs());
		for (unsigned i = 0; i < NumMemberValuePairs(); i++)
			clone->AddMemberValuePair((AstMemberValuePair*)
				MemberValuePair(i)->Clone(ast_pool));
		clone->right_paren_token_opt = right_paren_token_opt;
		return clone;
	}



void   AstAnnotation::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this, scope)) {

	}
	visitor->endVisit(this, scope);
}

	Token* AstAnnotation::LeftToken()
    {
	    return at_token;
    }

	Token* AstAnnotation::RightToken()
	{
		if (right_paren_token_opt)
			return right_paren_token_opt;

		 if(name)
			 return  name->identifier_token;

		return nullptr;
	}

	AstAnnotation::AstAnnotation(AstStoragePool* p):
		AstMemberValue(ANNOTATION)
	    , pool(p), member_value_pairs(nullptr), 
		at_token(nullptr), name(nullptr),
		right_paren_token_opt(nullptr)
	{}

	AstMemberValuePair*& AstAnnotation::MemberValuePair(unsigned i)
	{
		return (*member_value_pairs)[i];
	}

	unsigned AstAnnotation::NumMemberValuePairs()
	{
		return member_value_pairs ? member_value_pairs->Length() : 0;
	}

	void AstAnnotation::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (Annotation):  #" << name -> id << '(';
    for (i = 0; i < NumMemberValuePairs(); i++)
    {
        if (i % 10 == 0)
            os << endl << "       ";
        os << " #" << MemberValuePair(i) -> id;
    }
    os << ')' << endl;
    name -> Print(os, lex_stream);
    for (i = 0; i < NumMemberValuePairs(); i++)
        MemberValuePair(i) -> Print(os, lex_stream);
}

	void AstAnnotation::AllocateMemberValuePairs(unsigned estimate)
	{
		assert(!member_value_pairs);
		member_value_pairs =
			new (pool) AstArray<AstMemberValuePair*>(pool, estimate);
	}

	void AstAnnotation::AddMemberValuePair(AstMemberValuePair* pair)
	{
		assert(member_value_pairs);
		member_value_pairs->Next() = pair;
	}

} // Close namespace Jikes block


#endif // AstAnnotation_def_INCLUDED
