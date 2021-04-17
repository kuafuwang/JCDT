// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JIKES_LOOKUP_Hash_INCLUDED
#define JIKES_LOOKUP_Hash_INCLUDED
#include <JCDT_Lib/internal/impl/double.h>


namespace Jikes { // Open namespace Jikes block


	class Hash
	{
	public:
		//
		// HASH takes as argument a pointer to a character string
		// and its length which it hashes it into a location in the name
		// hash table.
		//
		inline static unsigned Function(const wchar_t* head, int len)
		{
			unsigned hash_value = 0;
			while (--len >= 0)
				hash_value = (hash_value << 5) - hash_value + *head++;
			return hash_value;
		}

		//
		// Same as above function for a regular "char" string.
		//
		inline static unsigned Function(const char* head, int len)
		{
			unsigned hash_value = 0;
			while (--len >= 0)
				hash_value = (hash_value << 5) - hash_value + *head++;
			return hash_value;
		}

		inline static unsigned Function(LongInt value)
		{
			return value.hashCode();
		}

		inline static unsigned Function(IEEEfloat value)
		{
			return value.hashCode();
		}

		inline static unsigned Function(IEEEdouble value)
		{
			return value.hashCode();
		}
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

