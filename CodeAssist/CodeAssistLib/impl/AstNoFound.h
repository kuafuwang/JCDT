// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef AstNoFound_INCLUDED
#define AstNoFound_INCLUDED
#include <exception>

namespace Jikes { // Open namespace Jikes block

	namespace CodeAssist {

		class AstNoFound :public std::exception
		{

		};

	}//namespace CodeAssist

} // Close namespace Jikes block


#endif // _INCLUDED

