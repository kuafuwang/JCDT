
#ifndef typedef_h_Jikes_INCLUDED
#define typedef_h_Jikes_INCLUDED
#include "config.h"
/*
undefine any symbols defined by the autotools
build process that can conflict with our symbols
*/

#undef PACKAGE
#undef PACKAGE_NAME
#undef VERSION

// A macro for generating <cname> vs. <name.h> as appropriate.
#ifdef HAVE_STD
# define STD_LIB_NAME(name) <c ## name>
#else
# define STD_LIB_NAME(name) <name.h>
#endif // ! HAVE_STD

/* Boilerplate autoconf checking */

#include <sys/types.h>
//FIXME: all stat stuff should be included in the platform.cpp file!
#include <sys/stat.h>
#ifdef STDC_HEADERS
# include STD_LIB_NAME(stdlib)
# include STD_LIB_NAME(stddef)
#else // ! STDC_HEADERS
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif // ! STDC_HEADERS

#ifdef HAVE_STRING_H



# ifndef STDC_HEADERS
#  ifdef HAVE_MEMORY_H
#   include <memory.h>
#  endif
# endif // ! STDC_HEADERS
# include STD_LIB_NAME(string)
#else // ! HAVE_STRING_H
# ifdef HAVE_STRINGS_H
#include <strings.h>
# endif
#endif // ! HAVE_STRING_H

#ifdef HAVE_SYS_CYGWIN_H
#include <sys/cygwin.h>
#endif





#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_WINDOWS_H
# include <windows.h>
#endif

#ifdef HAVE_DIRECT_H
# include <direct.h>
#endif

#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif

#ifdef HAVE_ERRNO_H
# include STD_LIB_NAME(errno)
#endif

#ifndef HAVE_WINT_T
/* On some systems the type wint_t is not defined in wchar.h */
typedef unsigned int wint_t;
#endif

#ifdef HAVE_WCHAR_H
# include STD_LIB_NAME(wchar)
#endif

#ifdef HAVE_CTYPE_H
# include STD_LIB_NAME(ctype)
#endif

#ifdef HAVE_ASSERT_H
# include STD_LIB_NAME(assert)
#endif

#ifdef HAVE_STDIO_H
# include STD_LIB_NAME(stdio)
#endif

#ifdef HAVE_LIMITS_H
# include STD_LIB_NAME(limits)
#endif

#ifdef HAVE_MATH_H
# include STD_LIB_NAME(math)
#endif

#ifdef HAVE_FLOAT_H
# include STD_LIB_NAME(float)
#endif

#ifdef HAVE_TIME_H
# include STD_LIB_NAME(time)
#endif

// C++ standard support

#ifdef HAVE_STD
# include <iostream>
# include <fstream>
#else
# include <iostream.h>
# include <fstream.h>
#endif // ! HAVE_STD

// VC++ pretends to support the C++ standard, but it does not.
// The set_new_handler method in <new> is not implemented so
// the _set_new_handler method in <new.h> must be used.

#ifdef HAVE_VCPP_SET_NEW_HANDLER
# include <new.h>
#else
# ifdef HAVE_STD
#  include <new>
# else
#  include <new.h>
# endif // ! HAVE_STD
#endif // ! HAVE_VCPP_SET_NEW_HANDLER

#ifdef HAVE_STD
# ifdef HAVE_NAMESPACES
using namespace std;
# endif
#endif


//
// In the compiler, we want EXACT control over signed and unsigned values
// of certain bit widths. Configure should have already found us what we need.
//
#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif // HAVE_INTTYPES_H


//
// Define a templatized function for dynamic_cast<> operator.
// This is slightly scary, but we need to do it so that we
// can continue to support older compilers that don't implement
// the dynamic_cast<> operator. We also do extra checking
// of the result when RTTI is supported. This does add some
// overhead, but if we catch a downcast bug as a result it
// is worth it. Downcast bugs were to blame for a number of
// core dumps in Jikes.
//
#ifdef HAVE_RTTI
# include <typeinfo>
#endif

typedef uint8_t u1;
typedef int8_t i1;
typedef uint16_t u2;
typedef int16_t i2;
typedef uint32_t u4;
typedef int32_t i4;
#ifdef HAVE_64BIT_TYPES
typedef uint64_t u8;
typedef int64_t i8;
#endif // HAVE_64BIT_TYPES
typedef u4 TokenIndex;
static const TokenIndex BAD_TOKEN = (TokenIndex)0;

// Rename for readability in JCDT_Lib/internal/impl/double.h.
#ifdef TYPE_INT32_T_IS_INT
#define TYPE_I4_IS_INT TYPE_INT32_T_IS_INT
#endif

//
// Some compilers do not correctly predefine the primitive type "bool"
// and its possible values: "false" and "true"
//
#ifndef HAVE_BOOL
// We define the type "bool" and the constants "false" and "true".
// The type bool as well as the constants false and true are expected
// to become standard C++. When that happens, these declarations should
// be removed.
typedef unsigned char bool;
enum { false = 0, true = 1 };
#endif

//typedef std::ostream Ostream;

namespace Jikes { // Open namespace Jikes block

	

} // Close namespace Jikes block


#endif // _INCLUDED

