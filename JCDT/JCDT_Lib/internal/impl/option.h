// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef option_INCLUDED
#define option_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/impl/JikesOption.h"


namespace Jikes {// Open namespace Jikes block


class ArgumentExpander;
class OptionError;
class Ostream;




class KeywordMap
{
public:
    wchar_t *name;
    int length,
        key;
};

class Option: public JikesOption
{
public:

	enum JCDK_VERSION
	{
		JCDK_1_X,
		JCDK_2_X,
		JCDK_3_X,
	};


#ifdef WIN32_FILE_SYSTEM
	char main_disk;
	char* current_directory[128];
#endif

    Tuple<KeywordMap> keyword_map;

    int first_file_index;

#ifdef JIKES_DEBUG
    int debug_trap_op;

    bool debug_dump_lex,
         debug_dump_ast,
         debug_unparse_ast,
         debug_unparse_ast_debug,
         debug_comments,
         debug_dump_class,
         debug_trace_stack_change;
#endif // JIKES_DEBUG

    bool nocleanup,
         incremental,
         makefile,
         dependence_report,
         bytecode,
         full_check,
         unzip,
     
         errors,
         pedantic,
         noassert;
	JCDK_VERSION jcdk_version;
    char *dependence_report_name;

    Option(ArgumentExpander &, Tuple<OptionError *>&);
	Option();
	Option(const Option&);
	void Copy(const Option& rhs);
    virtual ~Option();


#ifdef WIN32_FILE_SYSTEM

public:
	bool BadMainDisk() { return main_disk == 0; }
	void ResetCurrentDirectoryOnDisk(char d)
	{
		if (d != 0)
		{
			assert(current_directory[d]);
			SetCurrentDirectoryA(current_directory[d]);
		}
	}
	void SetMainCurrentDirectory()
	{
		SetCurrentDirectoryA(current_directory[main_disk]);
	}

	char *GetMainCurrentDirectory()
	{
		return current_directory[main_disk];
	}

	void SaveCurrentDirectoryOnDisk(char c);

#endif // WIN32_FILE_SYSTEM

	void SetFallBackBootClassPath();

};


} // Close namespace Jikes block


#endif // option_INCLUDED

