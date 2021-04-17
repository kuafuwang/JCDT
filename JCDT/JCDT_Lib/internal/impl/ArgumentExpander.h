// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ArgumentExpander_INCLUDED
#define ArgumentExpander_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "OptionError.h"

namespace Jikes { // Open namespace Jikes block


	class ArgumentExpander
	{
	public:

		int argc;
		char** argv;

		ArgumentExpander(int, char **, Tuple<OptionError *>& bad_options);

		~ArgumentExpander()
		{
			for (int i = 0; i < argc; i++)
				delete[] argv[i];
			delete[] argv;
		}

		void ExpandAtFileArgument(Tuple<char *>& arguments, char* file_name,
			Tuple<OptionError *>& bad_options);
};

} // Close namespace Jikes block


#endif // ArgumentExpander_INCLUDED

