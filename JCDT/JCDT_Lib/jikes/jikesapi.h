// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes JikesApi
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JIKES_API_H_FLAG_
#define JIKES_API_H_FLAG_


#include "JCDT_Lib/internal/util/FileUtil.h"

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
class JikesOption;
class JikesApi
{
public:

	JikesApi();
	virtual ~JikesApi();

	/**
	* Returns instance of current compiler options.
	* Returned pointer can be used to modify current compiler options.
	*/
	virtual JikesOption* getOptions();

	/**
	* Creates instance of compiler options,
	* corresponding to given command line parameters.
	*
	* @return list of java file names found on command line
	* Caller should not attempt to manage the memory returned
	* by this method as it can be freed during another call
	* to parseOptions() or when this object is destroyed.
	*/
	virtual char** parseOptions(int argc, char** argv);

	/**
	* Compile given list of files using current compiler options.
	*/
	virtual int compile(char** filenames);

	
	

private:
	void cleanupOptions(); // Helper to delete option and parsedOptions

	JikesOption* option;
	char** parsedOptions;
};


} // Close namespace Jikes block


#endif // JIKES_API_H_FLAG_
