// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#ifndef StreamError_H_FLAG_
#define StreamError_H_FLAG_
#include "JikesError.h"
#include <JCDT_Lib/internal/lookup/PositionInterface.h>
#include "ErrorString.h"

namespace Jikes { // Open namespace Jikes block


class StreamError : public JikesError,PositionNode
{
    friend class LexStream;
	friend class BaseLexStream;

public:
	StreamError();
    virtual wstring getErrorMessage();
    virtual wstring getErrorReport();

    virtual Problem::Severity getSeverity();
    virtual const char* getFileName();

    virtual int getLeftLineNo() { return left_line_no; }
    virtual int getLeftColumnNo() { return left_column_no; }
    virtual int getRightLineNo() { return right_line_no; }
    virtual int getRightColumnNo() { return right_column_no; }
	virtual int Compare(unsigned int pos) override {
	//unsigned start_location;
		//unsigned end_location;
		if(start_location > pos)
		{
			return LEFT_HAND_LARGE;
		}
		else if(end_location  < pos)
		{
			return LEFT_HAND_LESS;
		}
		else
		{
			return EQUAL;
		}
	}
    enum StreamErrorKind
    {
        BAD_TOKEN,
        EMPTY_CHARACTER_CONSTANT,
        UNTERMINATED_CHARACTER_CONSTANT,
        MULTI_CHARACTER_CONSTANT,
        ESCAPE_EXPECTED,
        UNTERMINATED_COMMENT,
        UNTERMINATED_STRING_CONSTANT,
        INVALID_HEX_CONSTANT,
        INVALID_FLOATING_HEX_EXPONENT,
        INVALID_FLOATING_HEX_MANTISSA,
        INVALID_FLOATING_HEX_PREFIX,
        INVALID_OCTAL_CONSTANT,
        INVALID_FLOATING_EXPONENT,
        INVALID_UNICODE_ESCAPE,
        INVALID_ESCAPE_SEQUENCE,
        LAST_CHARACTER_NOT_NEWLINE, // pedantic only
        DEPRECATED_IDENTIFIER_ASSERT, // from here, these are warnings only
        DEPRECATED_IDENTIFIER_ENUM,
        DOLLAR_IN_IDENTIFIER,
        FAVOR_CAPITAL_L_SUFFIX
    };

private:

    unsigned start_location;
    unsigned end_location;
    StreamErrorKind kind;

    static bool emacs_style_report;
    
    int left_line_no;
    int left_column_no;
    int right_line_no;
    int right_column_no;

	wstring error_string;
	
	string fileName;
    bool initialized = false;
	
    void Initialize(StreamErrorKind, unsigned, unsigned);
	void CanculateDetail(BaseLexStream*);

private:
	void getemacsErrorString();
	void getregularErrorString(BaseLexStream* lex_stream);
};




} // Close namespace Jikes block


#endif // StreamError_H_FLAG_
