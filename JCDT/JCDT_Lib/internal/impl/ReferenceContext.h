// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ReferenceContext_INCLUDED
#define ReferenceContext_INCLUDED


namespace Jikes { // Open namespace Jikes block


	class CompilationResult;
	class ReferenceContext
	{
	public:
		virtual void abort(int abortLevel, int problem) = 0;
		virtual CompilationResult* compilationResult() = 0;
		virtual bool hasErrors() = 0;
		virtual void tagAsHavingErrors() = 0;
	};


} // Close namespace Jikes block


#endif // ReferenceContext_INCLUDED

