// $Id: jikesapi.cpp,v 1.46 2004/03/20 04:48:17 jikesadmin Exp $
//
// This software is subject to the terms of the IBM Jikes JikesApi
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/problem/JikesError.h"

#include "JCDT_Lib/internal/impl/JikesOption.h"
#include "JCDT_Lib/internal/impl/ArgumentExpander.h"
#include "JCDT_Lib/internal/impl/OptionError.h"
#include "JCDT_Lib/internal/util/FileUtil.h"
#include "JCDT_Lib/internal/impl/FileHelper.h"
#include "JCDT_Lib/compiler/Compiler.h"

#include "jikesapi.h"
#include "JavaArgumentPathUtil.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>

// Note: JikesApi classes only use the Jikes namespace, they
// are never defined in the Jikes namespace. The use of the Jikes
// namespace is a compile time option and jikesapi.h can not
// include build files like platform.h or config.h. Only
// the Default* classes in this file can live in the Jikes namespace.


namespace Jikes {






JikesApi::JikesApi() : option(NULL),
                       parsedOptions(NULL)
{
    SetNewHandler();
    FloatingPointCheck();
   
}


JikesApi::~JikesApi()
{
    cleanupOptions();
}

void JikesApi::cleanupOptions()
{
    delete option;

    if (parsedOptions)
    {
        for (char** parsed = parsedOptions; *parsed != NULL; parsed++)
        {
            delete [] *parsed;
        }
        delete [] parsedOptions;
        parsedOptions = NULL;
    }
}

char** JikesApi::parseOptions(int argc, char** argv)
{
    cleanupOptions();

    Tuple<OptionError *> bad_options;
    ArgumentExpander *args = new ArgumentExpander(argc, argv, bad_options);
    Option* opt = new Option(*args, bad_options);
    option = opt;

    if (bad_options.Length() > 0)
    {
        for (unsigned i = 0; i < bad_options.Length(); i++)
            Coutput << bad_options[i] -> GetErrorMessage().c_str() << endl;
        parsedOptions = NULL;
    }
    else
    {
        int n = args -> argc - opt -> first_file_index;
        parsedOptions = new char*[n + 1];
        for (int i = 0; i < n; i++)
        {
            const char *o = args -> argv[opt -> first_file_index + i];
            assert(o);
            parsedOptions[i] = new char[strlen(o) + 1];
            strcpy(parsedOptions[i], o);
        }
        parsedOptions[n] = NULL;
    }

    for (unsigned i = 0; i < bad_options.Length(); i++)
        delete bad_options[i];

    delete args;
    return parsedOptions;
}

/**
* Compile given list of files.
*/
int JikesApi::compile(char** filenames)
{
	// Cast the JikesOption to an Option instance.
	// Note that the reason we don't use an Option
	// member type in the declaration of Compiler
	// is so that the jikespai.h header does not
	// need to include option.h.

	Option*_opt = new Option((*(Option*)option));

	

	JavaArgumentPathUtil pathUtil((Option*)option);
	JikesClassPaths collector;
	pathUtil.GetClassPaths(collector);
	INameEnvironment* nameEnv = new INameEnvironment(_opt, collector);

	Compiler* compiler = new Compiler(nameEnv,_opt);


	set<wstring> add_file_set;
	set<wstring> remove_file_set;
	set<wstring> chang_file_set;
	FileHelper::ProcessNewInputFiles(add_file_set, filenames);
	int return_code =compiler->compile(add_file_set, chang_file_set,remove_file_set);
	add_file_set.clear();
	remove_file_set.clear();
	chang_file_set.clear();
	//
	// If the incremental flag is on, check to see if the user wants us
	// to recompile.
	//
	if (((Option*)option)->incremental) {

		for (;;) {
			Ostream out;
			out.StandardOutput();
			out << endl << "Incremental: Enter to continue or q + Enter to quit: "
				<< flush;

			char ch;
			bool b_q = false;
			// See if the user types Q or presses enter/escape or sends an EOF
			while (1) {
				cin.get(ch);
				if (cin.eof() || (ch == U_q) || (ch == U_Q)) {
					b_q = true;
				}
				if ((ch == U_ESCAPE) || (ch == U_LINE_FEED)) {
					break;
				}
			}
			if(b_q){
				break;
			}
			return_code = compiler->compile(add_file_set, chang_file_set,remove_file_set);
		}
	}

	delete compiler;
	return return_code;
}
JikesOption* JikesApi::getOptions()
{
    return option;
}



} // Close namespace Jikes block


