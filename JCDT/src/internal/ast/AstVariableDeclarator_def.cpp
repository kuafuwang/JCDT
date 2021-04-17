#ifndef Jikes_AstVariableDeclarator_def_INCLUDED
#define Jikes_AstVariableDeclarator_def_INCLUDED

#include <JCDT_Lib/internal/ast/AstVariableDeclaratorId.h>
#include "JCDT_Lib/internal/ast/AstVariableDeclarator.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block


void AstVariableDeclarator::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*AstVariableDeclarator:#" << id << "*/";
    variable_declarator_name -> Unparse(os, lex_stream);
    if (variable_initializer_opt)
    {
        os << " = ";
        variable_initializer_opt -> Unparse(os, lex_stream);
    }
    if (debug_unparse)
        os << "/*:AstVariableDeclarator#" << id << "*/";
}

	Ast* AstVariableDeclarator::Clone(AstStoragePool* ast_pool)
	{
		AstVariableDeclarator* clone = ast_pool->GenVariableDeclarator();
		clone->variable_declarator_name = (AstVariableDeclaratorId*)
			variable_declarator_name->Clone(ast_pool);
		if (variable_initializer_opt)
			clone->variable_initializer_opt =
			variable_initializer_opt->Clone(ast_pool);
		return clone;
	}

	
void   AstVariableDeclarator::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		variable_declarator_name->traverse(visitor,scope);
		if (variable_initializer_opt)
			variable_initializer_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	wstring AstVariableDeclarator::getName()
	{
		return variable_declarator_name->identifier_token->getName();
	}

	wstring AstVariableDeclarator::getTypeName() const
	{
		if (owner)
			return{};

		if (owner->kind == LOCAL_VARIABLE_DECLARATION) {
			auto type_names = reinterpret_cast<LocalDeclaration*>(owner)->type->getTypeName();
			
			return boost::join(type_names, L".");
		
		}
		else if(owner->kind == PARAMETER)
		{
			auto type_names = reinterpret_cast<AstFormalParameter*>(owner)->type->getTypeName();

			return boost::join(type_names, L".");
		}
		else
		{
			auto type_names = reinterpret_cast<FieldDeclaration*>(owner)->type->getTypeName();

			return boost::join(type_names, L".");

		}
	}

	AstType* AstVariableDeclarator::Type()
	{
		if (!owner)
			return nullptr;

		switch (owner->kind )
		{
		case Ast::FIELD:
			return reinterpret_cast<FieldDeclaration*>(owner)->type;
		case Ast::LOCAL_VARIABLE_DECLARATION:
			return reinterpret_cast<LocalDeclaration*>(owner)->type;
		default:
			return reinterpret_cast<AstFormalParameter*>(owner)->type;
		}
	}

	AstVariableDeclarator::AstVariableDeclarator():
		ReachComleteAbleStatement(VARIABLE_DECLARATOR, true, true),
		symbol(nullptr), pending(false), variable_declarator_name(nullptr), 
		variable_initializer_opt(nullptr), owner(nullptr)
	{}

	void AstVariableDeclarator::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (VariableDeclarator):  " << '#'
            << variable_declarator_name -> id << " #"
            << (variable_initializer_opt ? variable_initializer_opt -> id : 0)
            << endl;
    variable_declarator_name -> Print(os, lex_stream);
    if (variable_initializer_opt)
        variable_initializer_opt -> Print(os, lex_stream);

}
 Token* AstVariableDeclarator::LeftToken()
{
	return variable_declarator_name->LeftToken();
}
 Token* AstVariableDeclarator::RightToken()
{
	return variable_initializer_opt
		? variable_initializer_opt->RightToken()
		: variable_declarator_name->RightToken();
}

} // Close namespace Jikes block


#endif // AstVariableDeclarator_def_INCLUDED
