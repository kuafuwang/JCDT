// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef AbstractMethodDeclaration_INCLUDED
#define AbstractMethodDeclaration_INCLUDED
#include <JCDT_Lib/internal/impl/ReferenceContext.h>
#include <JCDT_Lib/internal/ast/ASTNode.h>
#include "AstNodeScope.h"

namespace Jikes { // Open namespace Jikes block



class AbstractMethodDeclaration : public Ast,public ReferenceContext ,public AstNodeScope {
	/*
	*	We cause the compilation task to abort to a given extent.
	*/
public :
		MethodSymbol* method_symbol;
		AstTypeParameters* type_parameters_opt;

		bool body_parse;
		AstModifiers* modifiers_opt;
		AstMethodDeclarator* method_declarator;
		wstring selector();
		AbstractMethodDeclaration(AstKind k): Ast(k), method_symbol(nullptr), type_parameters_opt(nullptr),
		                                      body_parse(false), modifiers_opt(nullptr),method_declarator(nullptr)
		{
			
		}
	
	virtual ~AbstractMethodDeclaration()
	{

	}
	virtual vector<wstring> getReturnTypeName() { return{}; };

	std::pair<int, int> getNameRange();

	void getParameterTypes(vector<wstring>& types)
	{
		getParameterTypesAndNames(&types, nullptr);
	};

	void getParameterNames(vector<wstring>& names)
	{
		getParameterTypesAndNames(nullptr, &names);
	}

	void  getParameterTypesAndNames(vector<wstring>&types,vector<wstring>&names)
	{
		getParameterTypesAndNames(&types, &names);
	}

	virtual unsigned int  BodyStart() = 0;
	virtual unsigned int  BodyEnd() = 0;
	 void abort(int abortLevel, int problem) {
	}
	
	 virtual CompilationResult* compilationResult()
{
	return  nullptr;
}
	 virtual bool hasErrors()
{
	return false;
}
	virtual void tagAsHavingErrors()
{
	
}
protected:
	
	 void getParameterTypesAndNames(vector<wstring>*, vector<wstring>*);
public:
	int GetScopeKind() override {
		return METHOD_SCOPE;
	}

	Symbol* GetSymbol() override;
};

	
} // Close namespace Jikes block


#endif // AbstractMethodDeclaration_INCLUDED

