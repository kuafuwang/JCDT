#ifndef AstWildcard_def_INCLUDED
#define AstWildcard_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstWildcard.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block

	 const wchar_t  AstWildcard::WILDCARD_NAME[] = { '?' };
	 const wchar_t AstWildcard::WILDCARD_SUPER[] = L" super "; //$NON-NLS-1$
	 const wchar_t AstWildcard::WILDCARD_EXTENDS[] = L" extends "; //$NON-NLS-1$

void AstWildcard::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstWildcard:#" << id << "*/";
    os << lex_stream -> NameString(question_token->index);
    if (extends_token_opt)
    {
        os << ' ' << lex_stream -> NameString(extends_token_opt->index) << ' ';
        bounds_opt -> Unparse(os, lex_stream);
    }
    else if (super_token_opt)
    {
        os << ' ' << lex_stream -> NameString(super_token_opt->index) << ' ';
        bounds_opt -> Unparse(os, lex_stream);
    }
    if (debug_unparse)
        os << "/*:AstWildcard#" << id << "*/";
}


	
void   AstWildcard::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		if (bounds_opt)
			bounds_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstWildcard::AstWildcard(Token* t)
		: AstType(WILDCARD)
	     , question_token(t), 
		extends_token_opt(nullptr),
		super_token_opt(nullptr), 
		bounds_opt(nullptr)
	{}

	void AstWildcard::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (Wildcard):  "
            << lex_stream.NameString(question_token->index);
    if (extends_token_opt)
        os << ' ' << lex_stream.NameString(extends_token_opt->index) << " #"
                << bounds_opt -> id;
    else if (super_token_opt)
        os << ' ' << lex_stream.NameString(super_token_opt->index) << " #"
                << bounds_opt -> id;
    os << endl;
    if (bounds_opt)
        bounds_opt -> Print(os, lex_stream);
}
 vector<wstring> AstWildcard::getTypeName()
{

	 vector<wstring> ret(1, WILDCARD_NAME);

	if(super_token_opt)
	{
		ret.push_back(WILDCARD_SUPER);
		auto boundTypeName = bounds_opt->getTypeName();
		ret.insert(ret.end(), boundTypeName.begin(), boundTypeName.end());
	}
	else if(extends_token_opt)
	{
		auto boundTypeName = bounds_opt->getTypeName();
		ret.insert(ret.end(), boundTypeName.begin(), boundTypeName.end());
		ret.push_back(WILDCARD_EXTENDS);
	}

		
	return ret;
	
}
 vector<wstring> AstWildcard::getParameterizedTypeName()
 {
	 vector<wstring> ret(1, WILDCARD_NAME);

	 if (super_token_opt)
	 {
		 ret.push_back(WILDCARD_SUPER);
		 auto boundTypeName = bounds_opt->getParameterizedTypeName();
		 ret.insert(ret.end(), boundTypeName.begin(), boundTypeName.end());
	 }
	 else if (extends_token_opt)
	 {
		 auto boundTypeName = bounds_opt->getParameterizedTypeName();
		 ret.insert(ret.end(), boundTypeName.begin(), boundTypeName.end());
		 ret.push_back(WILDCARD_EXTENDS);
	 }


	 return ret;
 }
	Ast* AstWildcard::Clone(AstStoragePool* ast_pool)
	{
		AstWildcard* clone = ast_pool->GenWildcard(question_token);
		clone->extends_token_opt = extends_token_opt;
		clone->super_token_opt = super_token_opt;
		if (bounds_opt)
			clone->bounds_opt = (AstType*)bounds_opt->Clone(ast_pool);
		return clone;
	}


} // Close namespace Jikes block


#endif // AstWildcard_def_INCLUDED
