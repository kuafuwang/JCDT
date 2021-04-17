// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JikesError_H_FLAG_
#define JikesError_H_FLAG_
#include "ProblemSeverities.h"
#include <string>
using std::wstring;

namespace Jikes { // Open namespace Jikes block



class JikesError
{
public:
	virtual ~JikesError()
	{

	}

	JikesError(Problem::Type _t):_type(_t)
	{
		
	}

	static const int INVALID_LEFT_LINE_NO = 0;
	static const int INVALID_LEFT_COLUMN_NO = 0;
	static const int INVALID_RIGHT_LINE_NO = 0;
	static const int INVALID_RIGHT_COLUMN_NO = 0;


    virtual Problem::Severity getSeverity() = 0;

    /**
     * The filename where the error occurred. The caller should not delete
     * the return value.
     */
    virtual const char* getFileName() = 0;

    virtual int getLeftLineNo() = 0;
    virtual int getLeftColumnNo() = 0;
    virtual int getRightLineNo() = 0;
    virtual int getRightColumnNo() = 0;

    /**
     * Returns message describing error. The caller should not delete the
     * return value.
     */
    virtual  wstring getErrorMessage() = 0;

    /**
     * Returns formatted error report. The caller should not delete the
     * return value.
     */
    virtual  wstring getErrorReport() = 0;

	Problem::Type _type;

	const char* getSeverityString();
};

	
} // Close namespace Jikes block


#endif // JikesError_H_FLAG_
