#include <JCDT_Lib/internal/ast/AstTypeArguments.h>
#include <JCDT_Lib/internal/ast/AstName.h>
#ifndef AstTypeName_def_INCLUDED
#define AstTypeName_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstTypeName.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block
    void AstTypeName::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
            os << "/*AstTypeName:#" << id << "*/";
        if (base_opt)
        {
            base_opt -> Unparse(os, lex_stream);
            os << '.';
        }
        name -> Unparse(os, lex_stream);
        if (type_arguments_opt)
            type_arguments_opt -> Unparse(os, lex_stream);
        if (debug_unparse)
            os << "/*:AstTypeName#" << id << "*/";
    }
	 vector<wstring> AstTypeName::getTypeName()
	 {
		 
		 vector<wstring> ret;
		 if(base_opt)
		 {
			 ret = base_opt->getTypeName();
		 }
		 auto name_strs = name->getTokenNames();
		 for (size_t i = 0; i < name_strs.size(); ++i)
		 {
			 auto tokon = name_strs[i];
			 if(tokon){
				 ret.push_back(tokon);
			 }
			
		 }
		 return ret;
	 }
	 vector<wstring> AstTypeName::getParameterizedTypeName()
	 {
		
		
		 vector<wstring> ret;
		 if (base_opt)
		 {
			 ret = base_opt->getParameterizedTypeName();
		 }
		 auto name_strs = name->getTokenNames();
		 for (size_t i = 0; i < name_strs.size(); ++i)
		 {
			 auto tokon = name_strs[i];
			 if (tokon) {
				 ret.push_back(tokon);
			 }

		 }
		 if(type_arguments_opt)
		 {
			 ret.push_back(type_arguments_opt->getTypeArgumentNames());
		 }
		 return ret;
	 }

	Ast* AstTypeName::Clone(AstStoragePool* ast_pool)
	{
		AstTypeName* clone =
			ast_pool->GenTypeName((AstName*)name->Clone(ast_pool));
		if (base_opt)
			clone->base_opt = (AstTypeName*)base_opt->Clone(ast_pool);
		if (type_arguments_opt)
			clone->type_arguments_opt =
			(AstTypeArguments*)type_arguments_opt->Clone(ast_pool);
		return clone;
	}

	

	Token* AstTypeName::LeftToken()
	{
		return base_opt ? base_opt->LeftToken() : name->LeftToken();
	}

	Token* AstTypeName::RightToken()
	{
		return type_arguments_opt ? type_arguments_opt->right_angle_token
			       : name->identifier_token;
	}

	Token* AstTypeName::IdentifierToken()
	{
		return name->identifier_token;
	}

	void   AstTypeName::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		
		if (base_opt)
			base_opt->traverse(visitor, scope);
		name->traverse(visitor, scope);
		if (type_arguments_opt)
			type_arguments_opt->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

	AstTypeName::AstTypeName(AstName* n): AstType(TYPE), base_opt(nullptr)
	                                      , name(n), type_arguments_opt(nullptr)
	{}

	AstType*& AstTypeName::TypeArgument(unsigned i)
	{
		return type_arguments_opt->TypeArgument(i);
	}

	unsigned AstTypeName::NumTypeArguments()
	{
		return type_arguments_opt
			       ? type_arguments_opt->NumTypeArguments() : 0;
	}

	void AstTypeName::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (TypeName):  #"
            << (base_opt ? base_opt -> id : 0) << ".#" << name -> id << "<#"
            << (type_arguments_opt ? type_arguments_opt -> id : 0) << '>'
            << endl;
    if (base_opt)
        base_opt -> Print(os, lex_stream);
    name -> Print(os, lex_stream);
    if (type_arguments_opt)
        type_arguments_opt -> Print(os, lex_stream);
}


} // Close namespace Jikes block


#endif // AstTypeName_def_INCLUDED
