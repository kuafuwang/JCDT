// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/impl/ArgumentExpander.h"

#include <boost/filesystem.hpp>
#include <JCDT_Lib/internal/util/CharOperation.h>
namespace Jikes { // Open namespace Jikes block


				  //
				  // Look for file names in an @file and add them to the passed
				  // in tuple.
				  //
	void ArgumentExpander::ExpandAtFileArgument(Tuple<char *>& arguments,
		char *file_name,
		Tuple<OptionError *>& bad_options)
	{
		

		FILE *afile = SystemFopen(file_name, "r");
		
		boost::filesystem::path _path(CharOperation::ConvertUtf8ToUnicode(file_name));
		if (afile && boost::filesystem::exists(_path ) && boost::filesystem::is_regular_file(_path))
		{
			char *buffer;
			int file_size;
			char *start = NULL;
			char *end = NULL;
			char *eol = NULL;
			
			auto st_size = boost::filesystem::file_size(_path);
			buffer = new char[st_size + 2];
			file_size = SystemFread(buffer, 1, st_size, afile);

			// Fails under Cygwin (fopen "b" flag)
			//assert(status.st_size == file_size);

			buffer[file_size] = U_LINE_FEED;
			buffer[file_size + 1] = U_NULL;

			for (char *ptr = buffer; *ptr; )
			{
				// Skip spaces, tabs, and EOL until we find some text

				while (start == NULL && *ptr) {
					switch (*ptr) {
					case U_SPACE:
					case U_HORIZONTAL_TAB:
					case U_LINE_FEED:
					case U_CARRIAGE_RETURN:
						break; // Out of the switch not the while
					default:
						start = ptr;
						end = ptr;
					}
					ptr++;
				}

				// If at end of the buffer, no arguments are left
				if (!*ptr)
					break;

				// Find the end of this line, save last non space
				// or tab position in the end ptr

				while (eol == NULL)
				{
					switch (*ptr)
					{
					case U_LINE_FEED:
					case U_CARRIAGE_RETURN:
						eol = ptr;
						break;
					case U_SPACE:
					case U_HORIZONTAL_TAB:
						break; // ignore tabs and spaces
					default:
						end = ptr;
					}
					ptr++;

				}

				// Ignore single/double quotes at start and end of line.

				if (start < end &&
					(*start == U_DOUBLE_QUOTE &&
						*end == U_DOUBLE_QUOTE) ||
					(*start == U_SINGLE_QUOTE &&
						*end == U_SINGLE_QUOTE)) {
					start++;
					end--;
				}

				// Another @file name in an @file is not allowed
				if (start < end && (*start == U_AT)) {
					bad_options.Next() =
						new OptionError(OptionError::NESTED_AT_FILE, file_name);
					break;
				}

				// Copy arg into new string and add to tuple
				*(end + 1) = '\0';
				char *arg = new char[strlen(start) + 1];
				strcpy(arg, start);
				arguments.Next() = arg;

				// Reinit the pointers
				start = NULL;
				end = NULL;
				eol = NULL;
			}

			delete[] buffer;
		}
		else
		{
			bad_options.Next() = new OptionError(OptionError::INVALID_AT_FILE,
				file_name);
		}

		if (afile != NULL)
			fclose(afile);
	}


	ArgumentExpander::ArgumentExpander(int argc_, char *argv_[],
		Tuple<OptionError *>& bad_options)
	{
		Tuple<char *> arguments(8192);
		for (int i = 0; i < argc_; i++)
		{
			char *argument = argv_[i];
			if (argument[0] == '@')
			{
				ExpandAtFileArgument(arguments, argument + 1, bad_options);
			}
			else
			{
				char *str = new char[strlen(argument) + 1];
				strcpy(str, argument);
				arguments.Next() = str;
			}
		}

		argc = arguments.Length();
		argv = new char*[argc];

		for (int k = 0; k < argc; k++)
			argv[k] = arguments[k];
	}



} // Close namespace Jikes block



