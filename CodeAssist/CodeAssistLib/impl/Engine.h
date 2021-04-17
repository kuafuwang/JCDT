// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Engine
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JCDT_Engine__CodeAssist_H_FLAG_
#define JCDT_Engine__CodeAssist_H_FLAG_

#include <set>

#include <JCDT_Lib/compiler/Compiler.h>

using std::set;
using std::wstring;

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
class INameEnvironment;
class LookupEnvironment;
namespace CodeAssist {
	void ExcludeUselessMethod(TypeDeclaration* type, unsigned pos);
	class AssistOptions {
	public:

		 bool checkVisibility = false;
		 bool checkDeprecation = false;
		 bool checkForbiddenReference = false;
		 bool checkDiscouragedReference = false;
		 bool forceImplicitQualification = false;
		 bool camelCaseMatch = true;

	};
	class Engine:public  Compiler
	{
	public:
		FileSymbol* getUnit(const wstring& fileName) ;
		FileSymbol* cunit;
		Engine(INameEnvironment*, Option*);
		virtual ~Engine();
		void clear();
		AssistOptions options;
	protected:
		virtual bool checkSelection(LexStream*, unsigned int);


	
	};
}//namespace CodeAssist

} // Close namespace Jikes block


#endif // JCDT_Compiler_H_FLAG_
