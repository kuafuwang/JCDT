// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/env/platform.h"
#include  "JCDT_Lib/internal/impl/JikesOption.h"



namespace Jikes { // Open namespace Jikes block



	JikesOption::~JikesOption(){
		Clear();
	}

	void JikesOption::Clear()
	{
		delete[] bootclasspath;
		bootclasspath = NULL;

		delete[] classpath;
		classpath = NULL;

		delete[] directory;
		directory = NULL;

		delete[] encoding;
		encoding = NULL;

		delete[] extdirs;
		extdirs = NULL;

		delete[] sourcepath;
		sourcepath = NULL;

	}
	JikesOption::JikesOption()
		: bootclasspath(NULL),
		extdirs(NULL),
		classpath(NULL),
		sourcepath(NULL),
		directory(NULL),
		encoding(NULL),
		nowrite(false),
		deprecation(false),
		optimize(false),
		verbose(false),
		depend(false),
		old_classpath_search_order(false),
		help(false),
		version(false),
		g(SOURCE | LINES),
		source(SDK1_5),
		target(SDK1_5),
		tolerance(DEFAULT)
	{
	}
	void JikesOption::Copy(const JikesOption& rhs)
	{
		Clear();
		int max_path_name_length;
		if (rhs.bootclasspath) {
			max_path_name_length = strlen(rhs.bootclasspath) + 1;
			bootclasspath = new char[max_path_name_length + 1];
			memcpy(bootclasspath, rhs.bootclasspath, max_path_name_length);
		}
		if (rhs.extdirs) {
			max_path_name_length = strlen(rhs.extdirs) + 1;
			extdirs = new char[max_path_name_length + 1];
			memcpy(extdirs, rhs.extdirs, max_path_name_length);
		}
		if (rhs.classpath) {
			max_path_name_length = strlen(rhs.classpath) + 1;
			classpath = new char[max_path_name_length + 1];
			memcpy(classpath, rhs.classpath, max_path_name_length);
		}
		if (rhs.sourcepath) {
			max_path_name_length = strlen(rhs.sourcepath) + 1;
			sourcepath = new char[max_path_name_length + 1];
			memcpy(sourcepath, rhs.sourcepath, max_path_name_length);
		}
		if (rhs.directory) {
			max_path_name_length = strlen(rhs.directory) + 1;
			directory = new char[max_path_name_length + 1];
			memcpy(directory, rhs.directory, max_path_name_length);
		}
		if (rhs.encoding) {
			max_path_name_length = strlen(rhs.encoding) + 1;
			encoding = new char[max_path_name_length + 1];
			memcpy(encoding, rhs.encoding, max_path_name_length);
		}
	

		nowrite = rhs.nowrite;
		deprecation = rhs.deprecation;
		optimize = rhs.optimize;
		verbose = rhs.verbose;
		depend = rhs.depend;
		old_classpath_search_order = rhs.old_classpath_search_order;
		help = rhs.help;
		version = rhs.version;
		g = rhs.g;
		source = rhs.source;
		target = rhs.target;
		tolerance = rhs.tolerance;
	}

} // Close namespace Jikes block



