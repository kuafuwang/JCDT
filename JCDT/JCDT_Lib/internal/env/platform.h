

#ifndef platform_jiks_INCLUDED
#define platform_jiks_INCLUDED


#include "config.h"
#include "typedef.h"
#include "U_chars.h"


#include "StringConstant.h"
#include "IntToString.h"
#include "IntToWstring.h"
#include "LongToString.h"
#include "DoubleToString.h"
#include "FloatToString.h"
#include "Ostream.h"


namespace Jikes { // Open namespace Jikes block


	template <typename TO, typename FROM>
	inline TO DYNAMIC_CAST(FROM f)
	{
#ifndef HAVE_DYNAMIC_CAST
		return (TO)f;
#else
		// If NULL, return NULL to support dynamic_cast semantics
		if (!f)
			return (TO)NULL;
		TO ptr = dynamic_cast<TO> (f);

		if (!ptr)
		{
# ifdef HAVE_RTTI
			const type_info& t = typeid(f);
			const char* name = t.name();
			fprintf(stderr, "DYNAMIC_CAST argument type was \"%s\"\n", name);
# endif // HAVE_RTTI
			assert(ptr && "Failed dynamic_cast<> in DYNAMIC_CAST");
		}
		return ptr;
#endif // HAVE_DYNAMIC_CAST
	}

	//
	// The configure scripts check each of these to see if we need our own
	// implementation
	//

#ifndef HAVE_WCSLEN
	extern size_t wcslen(const wchar_t*);
#endif

#ifndef HAVE_WCSCPY
	extern wchar_t* wcscpy(wchar_t*, const wchar_t*);
#endif

#ifndef HAVE_WCSNCPY
	extern wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
#endif

#ifndef HAVE_WCSCAT
	extern wchar_t* wcscat(wchar_t*, const wchar_t*);
#endif

#ifndef HAVE_WCSCMP
	extern int wcscmp(const wchar_t*, const wchar_t*);
#endif

#ifndef HAVE_WCSNCMP
	extern int wcsncmp(const wchar_t*, const wchar_t*, size_t);
#endif


	//
	// If the system runs out of memory, this function is invoked.
	//
	void SetNewHandler();

#ifdef HAVE_VCPP_SET_NEW_HANDLER
	extern int OutOfMemory(size_t);
#else
	extern void OutOfMemory();
#endif

	//
	// When using the ICC compiler on Win95 or OS/2, we need to disable
	// testing for various floating point exceptions. Default behavior
	// was causing problems reading some standard class files.
	//
	extern void FloatingPointCheck();


	//
	// variants of system functions
	// are declared here and defined in code.cpp
	//

	extern FILE* SystemFopen(const char* name, const char* mode);
	extern size_t SystemFread(char* ptr, size_t element_size, size_t count,
		FILE* stream);
	extern int SystemIsDirectory(char* name);

	//
	// The symbol used in this environment for separating argument in a system
	// string. E.g., in a unix system directories specified in the CLASSPATH
	// are separated by a ':', whereas in win95 it is ';'.
	//
	extern char PathSeparator();
	extern int SystemMkdir(char*);
	extern int SystemMkdirhier(char*);
	extern int SystemMkdirhierForFile(char*);

	extern char* strcat3(const char*, const char*, const char*);
	extern char* wstring2string(wchar_t* in);


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// End of platform specific defines in this file, the rest of the code
	// in this file should work on any system
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	// The following comment was part of the original config.h, it explains why
	// some parts of the guts of the compiler are written the way they are. This
	// comment was removed when the ifdef'd EBCDIC code was initially removed.
	// I've added it back for its "why do they do it that way" value, hopefully
	// its presence will help prevent people from making assumptions and breaking
	// the code for non-ascii platforms. At some point we'll want to be able to
	// compile on EBCDIC systems again, using the API and iconv/ICU instead of
	// ifdefs scattered all over creation, if the core code hasn't been
	// compromised this should be easier. For those that have no idea what EBCDIC
	// is... it is like ASCII in that it is an encoding system, but it predates
	// ASCII by a number of years (think punchcards). It is still the default
	// encoding on some mainframes and minicomputers such as the IBM zSeries and
	// iSeries (aka the 390 and the 400). To see what the old EBCDIC code looked
	// like, pull a cvs tree with the tag v1-06.
	//
	//
	// Jikes can be compiled on systems using the EBCDIC character set, for which
	// character and string literals are translated by the compiler to EBCDIC and
	// not ASCII, and so cannot be used to represent ASCII/UNICODE values. Such
	// values are constructed using the U_ values defined below. Thus 'a' is
	// represented using U_a, and ".java" is represented by an explicit literal:
	//    {U_DOT, U_j, U_a, U_v, U_a, U_NULL}
	// Variables associated with such literals have names beginning with US_ if
	// the value are 16-bits or U8S_ for 8 bits. The initial underscore is
	// followed by the characters being represented, with letters and digits
	// representing themselves, and other values, all of which have a two
	// character code, surrounded by underscore. Thus the name used for the
	// literal above is US_DO_java.
	//
	// All string-related values are represented internally in ASCII/UNICODE
	// using the U_ values defined below. EBCDIC systems also require that
	// arguments to system functions representing file names be converted from
	// the internal form used by Jikes to EBCDIC, and such functions are referred
	// to using their usual name prefixed with "system_"; for example, a call to
	// "fopen(..." is written "system_fopen(..." The "system_" procedures are
	// define in the file code.cpp.
	//

	
	
	
	


} // Close namespace Jikes block


#endif // platform_INCLUDED

