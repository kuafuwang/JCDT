#include <JCDT_Lib/internal/ast/AstMemberValue.h>
#ifndef AstMemberValuePair_def_INCLUDED
#define AstMemberValuePair_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstMemberValuePair.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

    void AstMemberValuePair::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstMemberValuePair:#" << id << "*/";
        if (identifier_token_opt)
            os << lex_stream -> NameString(identifier_token_opt->index) << " = ";
        member_value -> Unparse(os, lex_stream);
        if (debug_unparse)
            os << "/*:AstMemberValuePair#" << id << "*/";
    }
    
	Ast* AstMemberValuePair::Clone(AstStoragePool* ast_pool)
	{
		AstMemberValuePair* clone = ast_pool->GenMemberValuePair();
		clone->identifier_token_opt = identifier_token_opt;
		clone->member_value = (AstMemberValue*)member_value->Clone(ast_pool);
		return clone;
	}

	

	Token* AstMemberValuePair::LeftToken()
	{
		return identifier_token_opt ? identifier_token_opt
			       : member_value->LeftToken();
	}

	Token* AstMemberValuePair::RightToken()
	{ return member_value->RightToken(); }

	void   AstMemberValuePair::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		member_value->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void AstMemberValuePair::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (MemberValuePair):  "
            << (identifier_token_opt
                ? lex_stream.NameString(identifier_token_opt->index) : L"(value)")
            << "=#" << member_value -> id << endl;
    member_value -> Print(os, lex_stream);
}



} // Close namespace Jikes block


#endif // AstMemberValuePair_def_INCLUDED
