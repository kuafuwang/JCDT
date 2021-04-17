// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ErrorString_INCLUDED
#define ErrorString_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"

namespace Jikes { // Open namespace Jikes block


	//
	// Since basic_ostringstream<wchar_t> is not supported correctly by all
	// compilers, we now use this workaround class.
	// WARNING: It is not thread-safe - calling Array() in multiple threads may
	// cause a race condition, getting the wrong string as a result.
	//
	class ErrorString : public ConvertibleArray<wchar_t>
	{
	public:
		ErrorString();
		~ErrorString()
		{
		//	delete[] result;
		}

		ErrorString& operator<<(const wchar_t* s);
		ErrorString& operator<<(const wchar_t c);
		ErrorString& operator<<(const char* s);
		ErrorString& operator<<(const char c);
		ErrorString& operator<<(int n);
		ErrorString& operator<<(ostream&(*f)(ostream&))
		{
			assert(f == (ostream&(*)(ostream&)) endl);
			return *this << '\n';
		}

		void width(int w);
		void fill(const char c);

		//
		// The returned value is , and is only guaranteed valid
		// until the next call to Array().
		//
		wstring ArrayString();

		//
		// The returned value is safe, but the caller is responsible for
		// freeing the returned memory.
		//
		const wchar_t* CallerFreeArray();

	private:
		void DoFill(int n);
		char fill_char;
		int field_width;
		// wchar_t* result;

	};


} // Close namespace Jikes block


#endif // ErrorString_INCLUDED

