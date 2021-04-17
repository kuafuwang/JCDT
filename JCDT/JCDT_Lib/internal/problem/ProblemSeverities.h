// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ProblemSeverities_INCLUDED
#define ProblemSeverities_INCLUDED


namespace Jikes { // Open namespace Jikes block

	namespace  Problem
	{
		enum Type
		{
			StreamError,
			ParseErrorInfo,
			ErrorInfo

		};
		enum Severity
		{
			JIKES_ERROR,
			JIKES_CAUTION,
			JIKES_WARNING
		};
	};
	
	

} // Close namespace Jikes block


#endif // ProblemSeverities_INCLUDED

