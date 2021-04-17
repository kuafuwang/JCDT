#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/impl/long.h"
#include <boost/filesystem/operations.hpp>
#include <JCDT_Lib/internal/util/CharOperation.h>


namespace Jikes { // Open namespace Jikes block


// Define the PathSeparator() function with the proper
// impl for each platform.

#ifdef HAVE_PATHNAME_STYLE_DOS
char PathSeparator() { return U_SEMICOLON; } // ";"
#else
char PathSeparator() { return U_COLON; } // ":"
#endif // ! HAVE_PATHNAME_STYLE_DOS


// Define the SystemMkdir() function with the proper
// impl for each platform.

int SystemMkdir(char* dirname)
{
	if (!dirname)
		return 0;
	auto _w_dir =  CharOperation::ConvertUtf8ToUnicode(dirname);
	boost::system::error_code ec;
	return boost::filesystem::create_directories(_w_dir, ec);
}

//
// The configure script checks each of these to see if we need our own
// implementation.
//
#ifndef HAVE_WCSLEN
size_t wcslen(const wchar_t* cs)
{
    int n = 0;
    while (*cs++)
        n++;
    return n;
}
#endif // HAVE_WCSLEN

#ifndef HAVE_WCSCPY
wchar_t* wcscpy(wchar_t* s, const wchar_t* ct)
{
    wchar_t* ptr;
    for (ptr = s; *ct; ptr++, ct++)
        *ptr = *ct;
    *ptr = U_NULL;
    return s;
}
#endif // HAVE_WCSCPY

#ifndef HAVE_WCSNCPY
wchar_t* wcsncpy(wchar_t* s, const wchar_t* ct, size_t n)
{
    wchar_t* ptr;
    for (ptr = s; *ct && n-- > 0; ptr++, ct++)
        *ptr = *ct;
    while (n-- > 0)
        *ptr++ = U_NULL;
    return s;
}
#endif // HAVE_WCSNCPY

#ifndef HAVE_WCSCAT
wchar_t* wcscat(wchar_t* s, const wchar_t* ct)
{
    wchar_t* ptr = s;
    while (*ptr)
        ptr++;
    wcscpy(ptr, ct);
    return s;
}
#endif // HAVE_WCSCAT

#ifndef HAVE_WCSCMP
int wcscmp(const wchar_t* cs, const wchar_t* ct)
{
    while (*cs == *ct && *cs && *ct)
    {
        cs++;
        ct++;
    }
    return (*cs == *ct ? 0 : (*cs < *ct ? -1 : 1));
}
#endif // HAVE_WCSCMP

#ifndef HAVE_WCSNCMP
int wcsncmp(const wchar_t* cs, const wchar_t* ct, size_t n)
{
    while (*cs == *ct && *cs && *ct && n-- > 0)
    {
        cs++;
        ct++;
    }
    return (n <= 0 || *cs == *ct ? 0 : (*cs < *ct ? -1 : 1));
}
#endif // HAVE_WCSNCMP


//
// If the system runs out of memory, this function is invoked
// This is tricky because VC++ on windows uses a non standard
// implementation of the set_new_handler function.
//
#ifdef HAVE_VCPP_SET_NEW_HANDLER
int OutOfMemory(size_t)
#else
void OutOfMemory()
#endif // ! HAVE_VCPP_SET_NEW_HANDLER
{
    fprintf(stderr, "***System Failure: Out of memory\n");
    exit(1);
#ifdef HAVE_VCPP_SET_NEW_HANDLER
    return 0;
#endif // HAVE_VCPP_SET_NEW_HANDLER
}

void SetNewHandler()
{
#ifdef HAVE_VCPP_SET_NEW_HANDLER
    _set_new_handler(OutOfMemory);
#else
    set_new_handler(OutOfMemory);
#endif // ! HAVE_VCPP_SET_NEW_HANDLER
}


//
// When using the ICC compiler on Win95 or OS/2, we need to disable
// testing for various floating point exceptions. Default behavior
// was causing problems reading some standard class files.
//
// We obviously don't need this on AIX (non x86), which uses xlC/ICC
//
void FloatingPointCheck()
{
#ifdef HAVE_ICC_FP_BUGS
    _control87(EM_UNDERFLOW, EM_UNDERFLOW);
    _control87(EM_ZERODIVIDE, EM_ZERODIVIDE);
    _control87(EM_OVERFLOW, EM_OVERFLOW);
    _control87(EM_INVALID, EM_INVALID);
#endif // HAVE_ICC_FP_BUGS
}


//
// This next set of functions may need some porting to work on various systems
//


FILE* SystemFopen(const char* name, const char* mode)
{
    return fopen(name, mode);
}

size_t SystemFread(char* ptr, size_t element_size, size_t count, FILE* stream)
{
    return fread(ptr, element_size, count, stream);
}

int SystemIsDirectory(char* name)
{
   /* struct stat status;
    return ((SystemStat(name, &status) == 0 &&
             (status.st_mode & JIKES_STAT_S_IFDIR)) ? 1 : 0);*/
	if (!name)
		return 0;
	auto _w_dir = CharOperation::ConvertUtf8ToUnicode(name);
	boost::system::error_code ec;
	return boost::filesystem::is_directory(_w_dir, ec);
}

int SystemMkdirhier(char* dirname)
{
    if (SystemIsDirectory(dirname))
        return 0;

    for (char* ptr = dirname; *ptr; ptr++)
    {
        char delimiter = *ptr;
        if (delimiter == U_SLASH)
        {
            *ptr = U_NULL;
            if (! SystemIsDirectory(dirname))
                SystemMkdir(dirname);
            *ptr = delimiter;
        }
    }
    SystemMkdir(dirname);
    return (! SystemIsDirectory(dirname));
}


// Create the specified directory and also any missing parent directories.
int SystemMkdirhierForFile(char* filename)
{
    for (int i = strlen(filename); i >= 0; i--)
    {
        if (filename[i] == U_SLASH)
        {
            int result = 0;
            filename[i] = U_NULL;
            if (! SystemIsDirectory(filename))
            {
                Ostream() << "making directory " << filename << "\n";
                result = SystemMkdirhier(filename);
            }
            filename[i] = U_SLASH;
            return result;
        }
    }
    return 0;
}


// FIXME: These next two should definitely be inlined; but when I
// add the "inline" keyword , I get linker problems.


// Given three strings, return a newly-allocated string which is their
// concatenation.
char* strcat3(const char* prefix, const char* middle, const char* suffix)
{
    int prefix_len = strlen(prefix);
    int prefix_middle_len = prefix_len + strlen(middle);

    char* result = new char[prefix_middle_len + strlen(suffix) + 1];
    strcpy(result, prefix);
    // The below is more efficient than this commented-out code.
    // strcat(result, middle);
    // strcat(result, suffix);
    strcpy(result + prefix_len, middle);
    strcpy(result + prefix_middle_len, suffix);
    return result;
}

// It's inconceivable that this is the right way to go about this.
// One alternative is to use ConvertUnicodeToUtf8.
char* wstring2string(wchar_t* in)
{
    char* result = new char[wcslen(in) + 1];
    result[wcslen(in)] = 0;
    for (size_t i=0; i<wcslen(in); i++) {
        wchar_t ch = in[i];
        result[i] = (ch >> 8 == 0 ? (char)ch : '?');
    }
    return result;
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// End of platform specific defines in this file, the rest of the code
// in this file should work on any system
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




} // Close namespace Jikes block

