// $Id: option.cpp,v 1.87 2004/10/10 23:17:44 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/impl/JikesOption.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/parser/LPG/javasym.h"
#include "JCDT_Lib/internal/util/case.h"
#include "JCDT_Lib/internal/util/tab.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "JCDT_Lib/internal/problem/SemanticError.h"
#include "JCDT_Lib/internal/impl/OptionError.h"
#include "JCDT_Lib/internal/impl/ArgumentExpander.h"



namespace Jikes { // Open namespace Jikes block




#ifdef WIN32_FILE_SYSTEM
void Option::SaveCurrentDirectoryOnDisk(char c)
{
    if (! current_directory[c])
    {
        char *disk_directory = NULL,
             disk[3] = { c, U_COLON, U_NULL },
             tmp[1];

        if (SetCurrentDirectoryA(disk))
        {
            // first, get the right size
            DWORD directory_length = GetCurrentDirectoryA(0, tmp);
            // allocate the directory
            disk_directory = new char[directory_length + 1];
            DWORD length = GetCurrentDirectoryA(directory_length,
                                               disk_directory);
            if (length <= directory_length)
            {
                for (char *ptr = disk_directory; *ptr; ptr++)
                    // turn '\' to '/'.
                    *ptr = (*ptr != U_BACKSLASH ? *ptr : (char) U_SLASH);
            }
        }

        if (! disk_directory)
        {
            disk_directory = new char[2];
            disk_directory[0] = U_DOT;
            disk_directory[1] = U_NULL;
        }

        current_directory[Case::ToAsciiLower(c)] = disk_directory;
        current_directory[Case::ToAsciiUpper(c)] = disk_directory;
    }
}
#endif // WIN32_FILE_SYSTEM


//
// Skip leading whitespace and copy the rest to a new string
// so we don't have to worry about affecting the environment
// variable. If input value is NULL or consists of only whitespace
// characters, returns NULL.
//
static inline char* makeStrippedCopy(char* value)
{
    if (value == NULL)
        return NULL;

    while (isspace(*value))
        value++;

    char* result = new char[strlen(value) + 1];
    strcpy(result, value);

#ifdef HAVE_CYGWIN_WIN32_TO_POSIX_PATH_LIST
    if (*value)
    {
        //
        // Under Cygwin, we convert a Windows-style path into a UNIX-style
        // path.  A path like "C:\Cygwin\tmp;C:\Windows" is converted into
        // "/tmp:/cygdrive/c/Windows" (assuming C:\Cygwin is cygroot).  We
        // can then parse it using the UNIX path seperator char ':'.
        //
        if (! cygwin_posix_path_list_p(result))
        {
            char* temp =
                new char[cygwin_win32_to_posix_path_list_buf_size(result)];
            cygwin_win32_to_posix_path_list(result, temp);
            delete[] result;
            result = temp;
        }
    }
#endif // CYGWIN_WIN32_TO_POSIX_PATH_LIST
    return result;
}
Option::Option()
	: first_file_index(0),
#ifdef JIKES_DEBUG
	debug_trap_op(0),
	debug_dump_lex(false),
	debug_dump_ast(false),
	debug_unparse_ast(false),
	debug_unparse_ast_debug(false),
	debug_comments(false),
	debug_dump_class(false),
	debug_trace_stack_change(false),
#endif // JIKES_DEBUG
	nocleanup(false),
	incremental(false),
	makefile(false),
	dependence_report(false),
	bytecode(true),
	full_check(false),
	unzip(false),
	errors(true),
	pedantic(false),
	noassert(false),
	dependence_report_name(NULL),
	jcdk_version(JCDK_2_X)
{
#ifdef WIN32_FILE_SYSTEM
	for (int j = 0; j < 128; j++)
		current_directory[j] = NULL;

	char tmp[1];
	// first, get the right size
	DWORD directory_length = GetCurrentDirectoryA(0, tmp);
	// allocate the directory
	char *main_current_directory = new char[directory_length + 1];
	DWORD length = GetCurrentDirectoryA(directory_length,
		main_current_directory);
	if (length > directory_length)
	{
		main_current_directory[0] = U_DOT;
		main_current_directory[1] = U_NULL;
		main_disk = 0;
	}
	else
	{
		for (char *ptr = main_current_directory; *ptr; ptr++)
			// turn '\' to '/'.
			*ptr = (*ptr != U_BACKSLASH ? *ptr : (char)U_SLASH);
		main_disk = main_current_directory[0]; // the first character
		current_directory[Case::ToAsciiLower(main_disk)] =
			main_current_directory;
		current_directory[Case::ToAsciiUpper(main_disk)] =
			main_current_directory;
	}
	current_directory[0] = main_current_directory;
#endif // WIN32_FILE_SYSTEM


	// Specify defaults for -source and -target.
	if (source == UNKNOWN)
	{
		switch (target)
		{
		case SDK1_1:
		case SDK1_2:
		case SDK1_3:
			source = SDK1_3;
			break;
		case UNKNOWN:
			target = SDK1_4_2;
			// fallthrough
		case SDK1_4:
		case SDK1_4_2:
			source = SDK1_4;
			break;
		case SDK1_5:
			source = ENABLE_SOURCE_15 ? SDK1_5 : SDK1_4;
			break;
		default:
			assert(false && "Unexpected target level");
		}
	}
	else if (target == UNKNOWN)
		target = source;
#ifdef _JIkesConsole
	if (!bootclasspath)
		// Create a clean copy of the bootclasspath envvar so we can modify
		//   this copy and delete it later in ~JikesOption
		bootclasspath = makeStrippedCopy(getenv("BOOTCLASSPATH"));
	if (!bootclasspath)
		SetFallBackBootClassPath();
	if (!extdirs)
		// Create a clean copy of the extdirs envvar so we can modify
		//   this copy and delete it later in ~JikesOption
		extdirs = makeStrippedCopy(getenv("EXTDIRS"));
	if (!classpath)
	{
		// Create a clean copy of the jikespath envvar so we can modify
		//   this copy and delete it later in ~JikesOption
		classpath = makeStrippedCopy(getenv("JIKESPATH"));
		if (!classpath)
			// Create a clean copy of the classpath envvar so we can modify
			//   this copy and delete it later in ~JikesOption
			classpath = makeStrippedCopy(getenv("CLASSPATH"));
	}
	if (!sourcepath)
	{
		// Create a clean copy of the sourcepath envvar so we can modify
		//   this copy and delete it later in ~JikesOption
		sourcepath = makeStrippedCopy(getenv("SOURCEPATH"));

		if (!sourcepath)
		{
			sourcepath = new char[2];
			sourcepath[0] = '.';
			sourcepath[1] = U_NULL;
		}
	}
#endif

}

Option::Option(ArgumentExpander& arguments,Tuple<OptionError *>& bad_options)
    : first_file_index(arguments.argc),
#ifdef JIKES_DEBUG
      debug_trap_op(0),
      debug_dump_lex(false),
      debug_dump_ast(false),
      debug_unparse_ast(false),
      debug_unparse_ast_debug(false),
      debug_comments(false),
      debug_dump_class(false),
      debug_trace_stack_change(false),
#endif // JIKES_DEBUG
      nocleanup(false),
      incremental(false),
      makefile(false),
      dependence_report(false),
      bytecode(true),
      full_check(false),
      unzip(false),
      errors(true),
      pedantic(false),
      noassert(false),
      dependence_report_name(NULL),
	jcdk_version(JCDK_2_X)
{
#ifdef WIN32_FILE_SYSTEM
    for (int j = 0; j < 128; j++)
        current_directory[j] = NULL;

    char tmp[1];
    // first, get the right size
    DWORD directory_length = GetCurrentDirectoryA(0, tmp);
    // allocate the directory
    char *main_current_directory = new char[directory_length + 1];
    DWORD length = GetCurrentDirectoryA(directory_length,
                                       main_current_directory);
    if (length > directory_length)
    {
        main_current_directory[0] = U_DOT;
        main_current_directory[1] = U_NULL;
        main_disk = 0;
    }
    else
    {
        for (char *ptr = main_current_directory; *ptr; ptr++)
            // turn '\' to '/'.
            *ptr = (*ptr != U_BACKSLASH ? *ptr : (char) U_SLASH);
        main_disk = main_current_directory[0]; // the first character
        current_directory[Case::ToAsciiLower(main_disk)] =
            main_current_directory;
        current_directory[Case::ToAsciiUpper(main_disk)] =
            main_current_directory;
    }
    current_directory[0] = main_current_directory;
#endif // WIN32_FILE_SYSTEM

    Tuple<int> filename_index(2048);

    for (int i = 1; i < arguments.argc; i++)
    {
        bool consumed = false;
        if (arguments.argv[i][0] == '-')
        {
            consumed = true;
            if (strcmp(arguments.argv[i], "-bootclasspath") == 0 ||
                strcmp(arguments.argv[i], "--bootclasspath") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // Create a clean copy of the -bootclasspath argument so we
                // can modify this copy and delete it later in ~JikesOption
                delete [] bootclasspath;
                bootclasspath = makeStrippedCopy(arguments.argv[++i]);
            }
            else if (strcmp(arguments.argv[i], "-classpath") == 0 ||
                     strcmp(arguments.argv[i], "--classpath") == 0 ||
                     strcmp(arguments.argv[i], "-cp") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // Create a clean copy of the -classpath argument so we can
                // modify this copy and delete it later in ~JikesOption
                delete [] classpath;
                classpath = makeStrippedCopy(arguments.argv[++i]);
            }
            else if (strcmp(arguments.argv[i], "-d") == 0 ||
                     strcmp(arguments.argv[i], "--target-directory") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                ++i;
                delete [] directory;

                //
                // FIXME: Move this platform specific stuff to platform.cpp.
                //
#if defined(UNIX_FILE_SYSTEM)
                int length = strlen(arguments.argv[i]);
                directory = new char[length + 1];
                strcpy(directory, arguments.argv[i]);
#elif defined(WIN32_FILE_SYSTEM)
                char disk = (Case::IsAsciiAlpha(arguments.argv[i][0]) &&
                             arguments.argv[i][1] == U_COLON)
                    ? arguments.argv[i][0] : 0;
                SaveCurrentDirectoryOnDisk(disk);
                if (SetCurrentDirectoryA(arguments.argv[i]))
                {
                    char tmp[1];
                    // first, get the right size
                    DWORD directory_length = GetCurrentDirectoryA(0, tmp);
                    // allocate the directory
                    directory = new char[directory_length + 1];
                    DWORD length = GetCurrentDirectoryA(directory_length,
                                                       directory);
                    if (length > directory_length)
                    {
                        delete [] directory;
                        directory = NULL;
                    }
                }

                // reset the current directory on the disk
                ResetCurrentDirectoryOnDisk(disk);
                // reset the real current directory...
                SetMainCurrentDirectory();

                if (! directory)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_DIRECTORY,
                                        arguments.argv[i]);
                }
#endif // WIN32_FILE_SYSTEM
                if (directory)
                {
                    for (char *ptr = directory; *ptr; ptr++)
                        // turn '\' to '/'.
                        *ptr = (*ptr != U_BACKSLASH ? *ptr : (char) U_SLASH);
                }
            }
            else if (strcmp(arguments.argv[i], "-debug") == 0)
                ; // Ignore for compatibility.
            else if (strcmp(arguments.argv[i], "-depend") == 0 ||
                     strcmp(arguments.argv[i], "--depend") == 0 ||
                     strcmp(arguments.argv[i], "-Xdepend") == 0)
            {
                depend = true;
            }
            else if (strcmp(arguments.argv[i], "-deprecation") == 0 ||
                     strcmp(arguments.argv[i], "--deprecation") == 0)
            {
                deprecation = true;
            }
            else if (strcmp(arguments.argv[i], "-encoding") == 0 ||
                     strcmp(arguments.argv[i], "--encoding") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                i++;
#if defined(HAVE_ENCODING)
                delete [] encoding;
                encoding = new char[strlen(arguments.argv[i]) + 1];
                strcpy(encoding, arguments.argv[i]);
                if (! Stream::IsSupportedEncoding(encoding))
                {
                    bad_options.Next() =
                        new OptionError(OptionError::UNSUPPORTED_ENCODING,
                                        encoding);
                    encoding = NULL;
                }

#else // ! defined(HAVE_ENCODING)
                bad_options.Next() =
                    new OptionError(OptionError::UNSUPPORTED_OPTION,
                                    "-encoding");
#endif // ! defined(HAVE_ENCODING)
            }
            else if (strcmp(arguments.argv[i], "-extdirs") == 0 ||
                     strcmp(arguments.argv[i], "--extdirs") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // Create a clean copy of the -extdirs argument so we can
                // modify this copy and delete it later in ~JikesOption
                delete [] extdirs;
                extdirs = makeStrippedCopy(arguments.argv[++i]);
            }
            else if (arguments.argv[i][1] == 'g')
            {
                // This defaults to SOURCE | LINES if no -g option was
                // explicitly specified. "-g" is equivalent to
                // "-g:source,lines,vars".
                if (! strcmp(arguments.argv[i], "-g:none"))
                    g = NONE;
                else if (! strcmp(arguments.argv[i], "-g:source") )
                    g = SOURCE;
                else if (! strcmp(arguments.argv[i], "-g:lines"))
                    g = LINES;
                else if (! strcmp(arguments.argv[i], "-g:vars"))
                    g = VARS;
                else if (! strcmp(arguments.argv[i], "-g:source,lines") ||
                         ! strcmp(arguments.argv[i], "-g:lines,source"))
                    g = SOURCE | LINES;
                else if (! strcmp(arguments.argv[i], "-g:source,vars") ||
                         ! strcmp(arguments.argv[i], "-g:vars,source"))
                    g = SOURCE | VARS;
                else if (! strcmp(arguments.argv[i], "-g:lines,vars") ||
                         ! strcmp(arguments.argv[i], "-g:vars,lines"))
                    g = LINES | VARS;
                else if (! strcmp(arguments.argv[i], "-g") ||
                         ! strcmp(arguments.argv[i], "-g:source,lines,vars") ||
                         ! strcmp(arguments.argv[i], "-g:source,vars,lines") ||
                         ! strcmp(arguments.argv[i], "-g:lines,source,vars") ||
                         ! strcmp(arguments.argv[i], "-g:lines,vars,source") ||
                         ! strcmp(arguments.argv[i], "-g:vars,source,lines") ||
                         ! strcmp(arguments.argv[i], "-g:vars,lines,source"))
                    g = SOURCE | LINES | VARS;
                else
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_OPTION,
                                        arguments.argv[i]);
                }
            }
            else if (strcmp(arguments.argv[i], "-help") == 0 ||
                     strcmp(arguments.argv[i], "--help") == 0 ||
                     strcmp(arguments.argv[i], "-h") == 0 ||
                     strcmp(arguments.argv[i], "-?") == 0)
            {
                help = true;
            }
            else if (arguments.argv[i][1] == 'J')
                ; // Ignore for compatibility.
            else if (strcmp(arguments.argv[i], "-nowarn") == 0 ||
                     strcmp(arguments.argv[i], "--nowarn") == 0 ||
                     strcmp(arguments.argv[i], "-q") == 0)
            {
                tolerance = NO_WARNINGS;
            }
            else if (strcmp(arguments.argv[i], "-Werror") == 0)
            {
                tolerance = ToleranceLevel(WARNINGS_ARE_ERRORS |
                                           CAUTIONS_ARE_ERRORS);
            }
            else if (strcmp(arguments.argv[i], "-nowrite") == 0 ||
                     strcmp(arguments.argv[i], "--nowrite") == 0)
            {
                nowrite = true;
            }
            else if (strcmp(arguments.argv[i], "-O") == 0 ||
                     strcmp(arguments.argv[i], "--optimize") == 0)
            {
                optimize = true;
            }
            else if (strcmp(arguments.argv[i], "-source") == 0 ||
                     strcmp(arguments.argv[i], "--source") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // See below for setting the default.
                i++;
                if (! strcmp(arguments.argv[i], "1.3"))
                    source = SDK1_3;
                else if (! strcmp(arguments.argv[i], "1.4"))
                    source = SDK1_4;
                else if (ENABLE_SOURCE_15 &&
                         ! strcmp(arguments.argv[i], "1.5"))
                {
                    source = SDK1_5;
                }
                else
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_SOURCE_ARGUMENT,
                                        "-source");
                }
            }
            else if (strcmp(arguments.argv[i], "-sourcepath") == 0 ||
                     strcmp(arguments.argv[i], "--sourcepath") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // Create a clean copy of the -sourcepath argument so we can
                // modify this copy and delete it later in ~JikesOption
                delete [] sourcepath;
                sourcepath = makeStrippedCopy(arguments.argv[++i]);
            }
            else if (strcmp(arguments.argv[i], "-target") == 0 ||
                     strcmp(arguments.argv[i], "--target") == 0)
            {
                if (i + 1 == arguments.argc)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                        arguments.argv[i]);
                    continue;
                }
                // See below for setting the default.
                i++;
                if (! strcmp(arguments.argv[i], "1.1"))
                    target = SDK1_1;
                else if (! strcmp(arguments.argv[i], "1.2"))
                    target = SDK1_2;
                else if (! strcmp(arguments.argv[i], "1.3"))
                    target = SDK1_3;
                else if (! strcmp(arguments.argv[i], "1.4"))
                    target = SDK1_4;
                else if (! strcmp(arguments.argv[i], "1.4.2"))
                    target = SDK1_4_2;
                else if (! strcmp(arguments.argv[i], "1.5"))
                    target = SDK1_5;
                else
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_TARGET_ARGUMENT,
                                        "-target");
                }
            }
            else if (strcmp(arguments.argv[i], "-verbose") == 0 ||
                     strcmp(arguments.argv[i], "--verbose") == 0 ||
                     strcmp(arguments.argv[i], "-v") == 0)
            {
                verbose = true;
            }
            else if (strcmp(arguments.argv[i], "-version") == 0 ||
                     strcmp(arguments.argv[i], "--version") == 0 ||
                     strcmp(arguments.argv[i], "-V") == 0)
            {
                version = true;
            }
            else if (strcmp(arguments.argv[i], "-Xstdout") == 0)
                //
                // FIXME: Javac 1.3 takes an argument to -Xstdout, as the name
                // of the file where to direct output. It is not a unary
                // flag to direct output to stdout instead of stderr.
                //
                Coutput.StandardOutput();
            else if (strcmp(arguments.argv[i], "-Xswitchcheck") == 0)
            {
                const char* image = arguments.argv[i] + 2;
                bool success = SemanticError::ProcessWarningSwitch(image);
                assert(success);
            }
            else if (arguments.argv[i][1] == 'X')
            {
                // Note that we've already consumed -Xdepend, -Xstdout,
                // and -Xswitchcheck.
                bad_options.Next() =
                    new OptionError(OptionError::UNSUPPORTED_OPTION,
                                    arguments.argv[i]);
            }
            else if (arguments.argv[i][1] != '-')
            {
                bad_options.Next() =
                    new OptionError(OptionError::INVALID_OPTION,
                                    arguments.argv[i]);
            }
            else consumed = false;
        }
        if (! consumed &&
            (arguments.argv[i][0] == '+' ||
             (arguments.argv[i][0] == '-' && arguments.argv[i][1] == '-')))
        {
            consumed = true;
            if (strcmp(arguments.argv[i], "++") == 0 ||
                strcmp(arguments.argv[i], "--incremental") == 0)
            {
                incremental = true;
                full_check = true;
            }
            else if (strcmp(arguments.argv[i], "+a") == 0 ||
                     strcmp(arguments.argv[i], "--noassert") == 0)
            {
                noassert = true;
            }
            else if (strcmp(arguments.argv[i], "+B") == 0 ||
                     strcmp(arguments.argv[i], "--nobytecode") == 0)
            {
                bytecode = false;
            }
            else if (strcmp(arguments.argv[i], "+D") == 0 ||
                     strcmp(arguments.argv[i], "--dump-errors") == 0)
            {
        
                errors = false;
            }
            else if (strncmp(arguments.argv[i], "+DR", 3) == 0)
            {
                char *image = arguments.argv[i] + 3;
                if (*image == '=')
                    image++;
                else if (! *image)
                {
                    if (i + 1 == arguments.argc)
                    {
                        bad_options.Next() =
                            new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                            arguments.argv[i]);
                        continue;
                    }
                    image = arguments.argv[++i];
                }

                makefile = true;
                dependence_report = true;
                full_check = true;
                dependence_report_name = new char[strlen(image) + 1];
                strcpy(dependence_report_name, image);
            }
            else if (strcmp(arguments.argv[i], "+E") == 0 ||
                     strcmp(arguments.argv[i], "--emacs") == 0)
            {
                errors = false;
            }
            else if (strcmp(arguments.argv[i], "+F") == 0 ||
                     strcmp(arguments.argv[i], "--full-dependence") == 0)
            {
                full_check = true;
            }
            else if (arguments.argv[i][1] == 'K')
            {
                char *name = arguments.argv[i] + 2,
                     *image;
                for (image = name; *image && *image != '='; image++)
                    ;

                if (*image != '=')
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_K_OPTION,
                                        arguments.argv[i]);
                else
                {
                    int key = 0; // assume undefined
                    image++;

                    if (strcmp(image, "boolean") == 0)
                        key = TK_boolean;
                    else if (strcmp(image, "byte") == 0)
                        key = TK_byte;
                    else if (strcmp(image, "char") == 0)
                        key = TK_char;
                    else if (strcmp(image, "short") == 0)
                        key = TK_short;
                    else if (strcmp(image, "int") == 0)
                        key = TK_int;
                    else if (strcmp(image, "long") == 0)
                        key = TK_long;
                    else if (strcmp(image, "float") == 0)
                        key = TK_float;
                    else if (strcmp(image, "double") == 0)
                        key = TK_double;
                    else
                    {
                        bad_options.Next() =
                            new OptionError(OptionError::INVALID_K_TARGET,
                                            image);
                    }

                    if (key != 0)
                    {
                        int i = keyword_map.NextIndex();
                        keyword_map[i].key = key;
                        keyword_map[i].length = image - name - 1;
                        keyword_map[i].name =
                            new wchar_t[keyword_map[i].length];
                        for (int k = 0; k < keyword_map[i].length; k++)
                            keyword_map[i].name[k] = name[k];
                    }
                }
            }
            else if (strcmp(arguments.argv[i], "+M") == 0 ||
                     strcmp(arguments.argv[i], "--makefile") == 0)
            {
                makefile = true;
                full_check = true;
            }

            else if (strcmp(arguments.argv[i], "+P") == 0 ||
                     strcmp(arguments.argv[i], "--pedantic") == 0)
            {
                // Turn on ALL default pedantic warnings. Can be called
                // multiple times.
                pedantic = true;
                SemanticError::EnableDefaultWarnings();
            }
            else if (arguments.argv[i][1] == 'P')
            {
                const char* image = arguments.argv[i] + 2;
                if (! SemanticError::ProcessWarningSwitch(image))
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_P_ARGUMENT,
                                        image);
                }
            }
            else if (arguments.argv[i][1] == 'T' ||
                     strncmp(arguments.argv[i], "--tab", 5) == 0)
            {
                int tab_size = 0;
                char* image = arguments.argv[i] + 2;
                if (arguments.argv[i][1] == '-')
                    image += 3;
                if (*image == '=')
                    image++;
                else if (! *image)
                {
                    if (i + 1 == arguments.argc)
                    {
                        bad_options.Next() =
                            new OptionError(OptionError::MISSING_OPTION_ARGUMENT,
                                            arguments.argv[i]);
                        continue;
                    }
                    image = arguments.argv[++i];
                }

                char *p;
                for (p = image; *p && *p >= '0' && *p <= '9'; p++)
                {
                    int digit = *p - '0';
                    tab_size = tab_size * 10 + digit;
                }
                if (*p)
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_TAB_VALUE, image);
                }
                Tab::SetTabSize(tab_size == 0 ? Tab::DEFAULT_TAB_SIZE
                                : tab_size);
            }
            else if (strcmp(arguments.argv[i], "+U") == 0 ||
                     strcmp(arguments.argv[i], "--unzip-dependence") == 0)
            {
                unzip = true;
                full_check = true;
            }
            else if (strcmp(arguments.argv[i], "--zero-cautions") == 0)
            {
                tolerance = CAUTIONS_ARE_ERRORS;
            }
            else if (strncmp(arguments.argv[i], "+Z", 2) == 0)
            {
                // We accept +Z, +Z0, +Z1, and +Z2, where +Z means +Z1.
                char* start = arguments.argv[i] + 2;
                char* end = 0;
                int level = (*start == '\0') ? 1 : strtoul(start, &end, 10);
                if (end && *end != 0) level = 666;

                // Translate the numeric level into a ToleranceLevel.
                if (level == 0)
                {
                    tolerance = NO_WARNINGS;
                }
                else if (level == 1)
                {
                    tolerance = CAUTIONS_ARE_ERRORS;
                }
                else if (level == 2)
                {
                    tolerance = ToleranceLevel(WARNINGS_ARE_ERRORS |
                                               CAUTIONS_ARE_ERRORS);
                }
                else
                {
                    bad_options.Next() =
                        new OptionError(OptionError::INVALID_OPTION,
                                        arguments.argv[i]);
                }
            }
#ifdef JIKES_DEBUG
            else if (strcmp(arguments.argv[i], "+A") == 0)
                debug_dump_ast = true;
            else if (strcmp(arguments.argv[i], "+c") == 0)
                debug_comments = true;
            else if (strcmp(arguments.argv[i], "+C") == 0)
                debug_dump_class = true;
            else if (strcmp(arguments.argv[i], "+L") == 0)
                debug_dump_lex = true;
            else if (strcmp(arguments.argv[i], "+O") == 0)
                debug_trap_op = atoi(arguments.argv[++i]);
            else if (strcmp(arguments.argv[i], "+S") == 0)
                debug_trace_stack_change = true;
            else if (strcmp(arguments.argv[i], "+u") == 0)
                debug_unparse_ast = true;
            else if (strcmp(arguments.argv[i], "+ud") == 0)
            {
                debug_unparse_ast = true;
                debug_unparse_ast_debug = true;
            }
#endif // JIKES_DEBUG
            else
            {
                bad_options.Next() =
                    new OptionError(OptionError::INVALID_OPTION,
                                    arguments.argv[i]);
            }
        }
        if (! consumed)
            filename_index.Next() = i;
    }

    // Specify defaults for -source and -target.
    if (source == UNKNOWN)
    {
        switch (target)
        {
        case SDK1_1:
        case SDK1_2:
        case SDK1_3:
            source = SDK1_3;
            break;
        case UNKNOWN:
            target = SDK1_4_2;
            // fallthrough
        case SDK1_4:
        case SDK1_4_2:
            source = SDK1_4;
            break;
        case SDK1_5:
            source = ENABLE_SOURCE_15 ? SDK1_5 : SDK1_4;
            break;
        default:
            assert(false && "Unexpected target level");
        }
    }
    else if (target == UNKNOWN)
        target = source;

#ifdef _JIkesConsole
    if (! bootclasspath)
        // Create a clean copy of the bootclasspath envvar so we can modify
        //   this copy and delete it later in ~JikesOption
        bootclasspath = makeStrippedCopy(getenv("BOOTCLASSPATH"));
    if (! bootclasspath)
        SetFallBackBootClassPath();
    if (! extdirs)
        // Create a clean copy of the extdirs envvar so we can modify
        //   this copy and delete it later in ~JikesOption
        extdirs = makeStrippedCopy(getenv("EXTDIRS"));
    if (! classpath)
    {
        // Create a clean copy of the jikespath envvar so we can modify
        //   this copy and delete it later in ~JikesOption
        classpath = makeStrippedCopy(getenv("JIKESPATH"));
        if (! classpath)
        // Create a clean copy of the classpath envvar so we can modify
        //   this copy and delete it later in ~JikesOption
            classpath = makeStrippedCopy(getenv("CLASSPATH"));
    }
    if (! sourcepath)
    {
        // Create a clean copy of the sourcepath envvar so we can modify
        //   this copy and delete it later in ~JikesOption
        sourcepath = makeStrippedCopy(getenv("SOURCEPATH"));

        if (! sourcepath)
        {
            sourcepath = new char[2];
            sourcepath[0] = '.';
            sourcepath[1] = U_NULL;
        }
    }
#endif

    //
    // Initially, first_file_index is set to argc. Since the array
    // filename_index contains the indices of all the input files in
    // arguments in reverse order, we reverse the order here again to get
    // the original list...
    //
    for (int k = filename_index.Length() - 1; k >= 0; k--)
    {
        first_file_index--;

        int i = filename_index[k];
        char *temp = arguments.argv[i];
        arguments.argv[i] = arguments.argv[first_file_index];
        arguments.argv[first_file_index] = temp;
    }

    //
    // If the traditional batch output form for reporting errors is requested,
    // then wide character expansion is turned on.
    //
    if (errors)
        Coutput.SetExpandWchar();
}

Option::Option(const Option& rhs)
{
	this->Copy(rhs);
}
void Option::Copy(const Option& rhs){

	JikesOption::Copy(rhs);


#ifdef WIN32_FILE_SYSTEM
	main_disk = rhs.main_disk; 
	for (size_t i = 0; i < 128; ++i){
		char* dir = rhs.current_directory[i];
		if(dir){
			int len = strlen(dir) + 1;
			char* cpy = new char[len];
			memcpy(cpy, dir, len);
			current_directory[i] = cpy;
		}
		else{
			current_directory[i] = NULL;
		}	
	}
#endif

	keyword_map.Resize(rhs.keyword_map.Length());
	keyword_map = rhs.keyword_map;

	first_file_index = rhs.first_file_index;

#ifdef JIKES_DEBUG
	 debug_trap_op = rhs.debug_trap_op;

	 debug_dump_lex = rhs.debug_dump_lex;
	 debug_dump_ast = rhs.debug_dump_ast;
	 debug_unparse_ast = rhs.debug_unparse_ast;
	 debug_unparse_ast_debug = rhs.debug_unparse_ast_debug;
	 debug_comments = rhs.debug_comments;
	 debug_dump_class = rhs.debug_dump_class;
	 debug_trace_stack_change = rhs.debug_trace_stack_change;
#endif // JIKES_DEBUG

	nocleanup = rhs.nocleanup;
	incremental = rhs.incremental;
	makefile = rhs.makefile;
	dependence_report = rhs.dependence_report;
	bytecode = rhs.bytecode;
	full_check = rhs.full_check;
	unzip = rhs.unzip;
	
	errors = rhs.errors;
	pedantic = rhs.pedantic;
	noassert = rhs.noassert;
	if (rhs.dependence_report_name) {
		int len = strlen(rhs.dependence_report_name) + 1;
		dependence_report_name = new char[len];
		dependence_report_name = rhs.dependence_report_name;
		memcpy(dependence_report_name, rhs.dependence_report_name, len);
	}
	else
	{
		dependence_report_name = NULL;
	}
	jcdk_version = rhs.jcdk_version;
}
Option::~Option()
{
    delete [] dependence_report_name;

#ifdef WIN32_FILE_SYSTEM
    for (char c = 'a'; c <= 'z'; c++)
        delete [] current_directory[c];
#endif // WIN32_FILE_SYSTEM
}


//
// Until we can switch to using the STL (I think AIX support holds us back),
// here's a poor man's replacement for std::ostringstream.
//
class OStringStream : public ConvertibleArray<char>
{
public:
    OStringStream& operator<<(char ch)
    {
        Next() = ch;
        return *this;
    }

    OStringStream& operator<<(const char* s)
    {
        assert(s != 0);
        for (const char* p = s; *p; ++p)
        {
            Next() = *p;
        }
        return *this;
    }
};


//
// Tests whether 's' ends with 'suffix'. The comparison is case-insensitive.
//
static bool CStringEndsWith(const char* s, const char* suffix)
{
    size_t s_length = strlen(s);
    size_t suffix_length = strlen(suffix);
    if (s_length < suffix_length) 
        return false;
    return (strcasecmp(s + s_length - suffix_length, suffix) == 0);
}


//
// On Mac OS X, we know where Java keeps its .jar files; on
// other platforms, it's not unknown for developers to have
// the JAVA_HOME environment variable set, and from there we
// know we want "jre/lib/".
//
// Scan the appropriate directory for .jar files, and use
// that as our bootclasspath. (This is only used if the
// other methods for coming by a bootclasspath have failed.)
//
void Option::SetFallBackBootClassPath()
{
#if defined(UNIX_FILE_SYSTEM)
    assert(bootclasspath == NULL);

#ifdef __APPLE__
    const char* classes_directory =
        "/System/Library/Frameworks/JavaVM.framework/Classes/";
#else
    const char* java_home = getenv("JAVA_HOME");
    if (java_home == NULL)
        return;

    OStringStream os;
    os << java_home << "/jre/lib/" << '\0';
    const char* classes_directory = os.Array();
#endif

    if (DIR* dir = opendir(classes_directory))
    {
        // Join all the .jar filenames together into a path.
        OStringStream path;
        const char* sep = "";
        for (dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir))
        {
            if (entry -> d_type = DT_REG &&
                CStringEndsWith(entry -> d_name, ".jar"))
            {
                path << sep << classes_directory << '/' << entry -> d_name;
                sep = ":";
            }
        }
        path << '\0';
        bootclasspath = makeStrippedCopy(path.Array());
        closedir(dir);
    }
#endif
}


} // Close namespace Jikes block


