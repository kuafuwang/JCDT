#ifndef AstName_def_INCLUDED
#define AstName_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstName.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


void AstName::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstName:#" << id << "*/";
    if (base_opt)
    {
        base_opt -> Unparse(os, lex_stream);
        os << '.';
    }
    os << lex_stream -> NameString(identifier_token->index);
    if (debug_unparse)
        os << "/*:AstName#" << id << "*/";
}

	Ast* AstName::Clone(AstStoragePool* ast_pool)
	{
		AstName* clone = ast_pool->GenName(identifier_token);
		if (base_opt)
			clone->base_opt = (AstName*)base_opt->Clone(ast_pool);
		if (resolution_opt)
			clone->resolution_opt =
			(Expression*)resolution_opt->Clone(ast_pool);
		return clone;
	}




void   AstName::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		if (base_opt)
			base_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void AstName::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (Name):  #"
            << (base_opt ? base_opt -> id : 0) << '.'
            << lex_stream.NameString(identifier_token->index) << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
}
vector<const wchar_t*> AstName::getTokenNames()
{
	vector<const wchar_t*> ret;
	if (base_opt)
	{
		 ret = base_opt->getTokenNames();
	}
	ret.push_back(identifier_token->NameString());
	return ret;
}

void AstName::getCompoundNames(wstring& _name)
{
	;
	if (base_opt){
		base_opt->getCompoundNames(_name);
	}
	
	if(!_name.empty()){
		_name.append(L".").append(identifier_token->NameString());
	}
	else
	{
		_name = identifier_token->NameString();
	}
}

	bool AstName::isTypeReference()
	{
		return  symbol && symbol->_kind == Symbol::TYPE;
	}

	bool AstName::isTypeAccess()
	{
		return symbol == nullptr || symbol->_kind == Symbol::TYPE;
	}
} // Close namespace Jikes block


#endif // AstName_def_INCLUDED
