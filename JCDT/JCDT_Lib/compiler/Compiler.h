// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JCDT_Compiler_H_FLAG_
#define JCDT_Compiler_H_FLAG_
#include "JCDT_Lib/internal/impl/JikesOption.h"
#include "JCDT_Lib/internal/util/FileUtil.h"
#include <JCDT_Lib/internal/impl/ITypeRequestor.h>
#include <JCDT_Lib/internal/ast/ast.h>

namespace Jikes {

	// Open namespace Jikes block

/**
 * API to jikes compiler.
 */
class JikesError;
class AstStoragePool;
class Option;
class Scanner;
class Parser;
class Semantic;
class LexStream;
class AstPackageDeclaration;
class AstName;
class TypeDependenceChecker;
class TypeDeclaration;
class INameEnvironment;
class Compiler :public ITypeRequestor
{
public:

	void compiler(set<FileSymbol*>&);
	Compiler(INameEnvironment*, Option*);
	Compiler();
    virtual ~Compiler();

    /**
     * Returns instance of current compiler options.
     * Returned pointer can be used to modify current compiler options.
     */
    virtual JikesOption* getOptions();

	virtual bool accept(FileSymbol*) override;
	
	virtual AstPackageDeclaration* ParseForPackage(FileSymbol*, AstStoragePool*) override;
	virtual bool HeadParse(FileSymbol*) override;

	

    /**
     * Compile given list of files using current compiler options
     */
	virtual int compile(set<wstring>& ,
		set<wstring>& ,
		set<wstring>& );

    
    /**
     * This method will be called for each error reported.
     */
    static void reportError(JikesError* error);

   
	
   

	LookupEnvironment *lookupEnvironment;
    Option* option;
    char** parsedOptions;
	Parser* parser;
	Scanner* scanner;
};


} // Close namespace Jikes block


#endif // JCDT_Compiler_H_FLAG_
