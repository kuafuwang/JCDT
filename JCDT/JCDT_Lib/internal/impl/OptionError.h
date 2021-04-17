// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef OptionError_INCLUDED
#define OptionError_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"


namespace Jikes { // Open namespace Jikes block



	class OptionError
	{
	public:
		enum OptionErrorKind
		{
			INVALID_OPTION,
			MISSING_OPTION_ARGUMENT,
			INVALID_SOURCE_ARGUMENT,
			INVALID_TARGET_ARGUMENT,
			INVALID_K_OPTION,
			INVALID_K_TARGET,
			INVALID_TAB_VALUE,
			INVALID_P_ARGUMENT,
			INVALID_DIRECTORY,
			INVALID_AT_FILE,
			NESTED_AT_FILE,
			UNSUPPORTED_ENCODING,
			UNSUPPORTED_OPTION,
			DISABLED_OPTION
		};

		OptionError(OptionErrorKind kind_, const char *str) : kind(kind_)
		{
			name = new char[strlen(str) + 1];
			strcpy(name, str);
			return;
		}

		~OptionError() { delete[] name; }

		wstring GetErrorMessage();

	private:
		OptionErrorKind kind;
		char *name;
};

	

} // Close namespace Jikes block


#endif // OptionError_INCLUDED

