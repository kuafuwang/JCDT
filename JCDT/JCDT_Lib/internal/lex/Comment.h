// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef _INCLUDED
#define _INCLUDED
#include "Token.h"
#include <JCDT_Lib/internal/lookup/PositionInterface.h>


namespace Jikes { // Open namespace Jikes block

	struct Comment :public PositionNode
	{
		TokenIndex previous_token;
		unsigned location;
		unsigned length;
		wchar_t* string;
		inline unsigned Location() { return location; }
		inline unsigned end() {
			return location + length;
		}

		virtual int Compare(unsigned int pos) override{
			if(location > pos )
			{
				return LEFT_HAND_LARGE;
			}
			else if(location + length < pos )
			{
				return LEFT_HAND_LESS;
			}
			else
			{
				return EQUAL;
			}
		}
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

