// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include <JCDT_Lib/internal/problem/ErrorString.h>



namespace Jikes { // Open namespace Jikes block


Jikes::ErrorString::ErrorString()
		: ConvertibleArray<wchar_t>(1024),
		fill_char(' '),
		field_width(0)
	{
	}

	void ErrorString::DoFill(int n)
	{
		while (n < field_width)
		{
			Next() = (wchar_t)fill_char;
			n++;
		}
		field_width = 0;
	}

	ErrorString& ErrorString::operator<<(const wchar_t c)
	{
		DoFill(1);
		Next() = c;
		return *this;
	}

	ErrorString& ErrorString::operator<<(const char c)
	{
		DoFill(1);
		Next() = (wchar_t)c;
		return *this;
	}

	ErrorString& ErrorString::operator<<(const wchar_t* s)
	{
		if (s)
		{
			DoFill(
				wcslen(
#ifdef HAVE_ERROR_CALL_WCSLEN_CONST
					(wchar_t*)
#endif
					s)
				);
			while (*s)
				Next() = *(s++);
		}

		return *this;
	}

	ErrorString& ErrorString::operator<<(const char* s)
	{
		if (s)
		{
			DoFill(strlen(s));
			while (*s)
				Next() = (wchar_t)*(s++);
		}

		return *this;
	}

	ErrorString& ErrorString::operator<<(int n)
	{
		char buf[64];
		sprintf(buf, "%d", n);

		return (*this << buf);
	}

	wstring ErrorString::ArrayString()
	{
		// Not thread-safe, but jikes isn't threaded.
	
		Next() = U_NULL; // zero terminate string
		wstring _ret(top,0);
		wchar_t* result = const_cast<wchar_t*>(_ret.data());
		memcpy(result, ConvertibleArray<wchar_t>::Array(), top * sizeof(wchar_t));
		return _ret;
	}

	const wchar_t* ErrorString::CallerFreeArray()
	{
		// safe, but the caller will be resposible for freeing the memory
		Next() = U_NULL; // zero terminate string
		wchar_t* result_ret = new wchar_t[top];
		memcpy(result_ret, ConvertibleArray<wchar_t>::Array(), top * sizeof(wchar_t));
		return result_ret;
	}

	void ErrorString::width(int w)
	{
		field_width = w;
	}

	void ErrorString::fill(const char c)
	{
		fill_char = c;
	}



} // Close namespace Jikes block



