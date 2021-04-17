// $Id: stream.cpp,v 1.85 2004/03/25 13:32:28 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/lex/LexStream.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include <JCDT_Lib/internal/problem/ErrorString.h>
#include <tchar.h>


namespace Jikes { // Open namespace Jikes block


// Class StreamError

	Problem::Severity StreamError::getSeverity()
{
    // Most Lexical errors are ERRORs.
    return kind >= StreamError::LAST_CHARACTER_NOT_NEWLINE
        ? Problem::JIKES_WARNING : Problem::JIKES_ERROR;
}

const char* StreamError::getFileName()
{
	return fileName.c_str();
}

wstring StreamError::getErrorMessage()
{
    switch (kind)
    {
    case BAD_TOKEN:
        return L"Illegal token ignored.";
    case EMPTY_CHARACTER_CONSTANT:
        return L"Empty character constant.";
    case UNTERMINATED_CHARACTER_CONSTANT:
        return L"Character constant not properly terminated.";
    case MULTI_CHARACTER_CONSTANT:
        return L"Character constant must be only one character.";
    case ESCAPE_EXPECTED:
        return L"Escape sequence required for this character constant.";
    case UNTERMINATED_COMMENT:
        return L"Comment not properly terminated.";
    case UNTERMINATED_STRING_CONSTANT:
        return L"String constant not properly terminated.";
    case INVALID_HEX_CONSTANT:
        return L"The hexadecimal prefix '0x' must be followed by at least one "
            L"hex digit.";
    case INVALID_FLOATING_HEX_EXPONENT:
        return L"A hexadecimal floating point literal must have an exponent "
            L"'p' designator.";
    case INVALID_FLOATING_HEX_MANTISSA:
        return L"A hexadecimal floating point literal must have at least one "
            L"hex digit between the prefix '0x' and exponent 'p'.";
    case INVALID_FLOATING_HEX_PREFIX:
        return L"A hexadecimal floating point literal must start with the "
            L"prefix '0x'.";
    case INVALID_OCTAL_CONSTANT:
        return L"The octal prefix '0' must not be followed by '8' or '9'.";
    case INVALID_FLOATING_EXPONENT:
        return L"A floating point exponent must have at least one digit.";
    case INVALID_UNICODE_ESCAPE:
        return L"Invalid unicode escape character.";
    case INVALID_ESCAPE_SEQUENCE:
        return L"Invalid escape sequence.";
    case LAST_CHARACTER_NOT_NEWLINE:
        return L"While not necessary, it is a good idea to end a file with a "
            L"line terminator.";
    case DEPRECATED_IDENTIFIER_ASSERT:
        return L"The use of \"assert\" as an identifier is deprecated, "
            L"as it is now a keyword. Use -source 1.4 if you intended "
            L"to make use of assertions.";
    case DEPRECATED_IDENTIFIER_ENUM:
        return L"The use of \"enum\" as an identifier is deprecated, "
            L"as it will be a keyword once -source 1.5 is implemented.";
    case DOLLAR_IN_IDENTIFIER:
        return L"The use of \"$\" in an identifier, while legal, is strongly "
            L"discouraged, since it can conflict with compiler-generated "
            L"names. If you are trying to access a nested type, use \".\" "
            L"instead of \"$\".";
    case FAVOR_CAPITAL_L_SUFFIX:
        return L"The L suffix is preferred over the l suffix because l "
            L"(lowercase L) is easily confused with 1 (the digit 1).";
    default:
        assert(false);
    }

    return L"Unknown Error";
}

bool StreamError::emacs_style_report = false;

 wstring StreamError::getErrorReport()
{
    //
    // We need to use this lazy initialization, because we can't to it in
    // Initialize() method. Reason is that Find* methods are unusable until
    // LexStream::CompressSpace is called, which does not happen until later
    // after scanning is done and all errors are reported.
    //
    if (! initialized){
		assert(false);
		return NULL;
    }

    return error_string;
}




void StreamError::getemacsErrorString()
{
	ErrorString s;
	s << getFileName()
		<< ':' << left_line_no << ':' << left_column_no
		<< ':' << right_line_no << ':' << right_column_no
		<< ": Lexical " << getSeverityString() << ": " << getErrorMessage().c_str();

	
	error_string = s.ArrayString();
	
}

void StreamError::getregularErrorString(BaseLexStream* lex_stream)
{
	if(!lex_stream){
		assert(lex_stream);
		return;
	}
	ErrorString output;
	ErrorString s;
	lex_stream->OutputSource(this, s);
	wstring fmt_string = getErrorMessage();
	int format_size = 80;
	int string_size = fmt_string.size();
	int t = 1;
	while (string_size > format_size)
	{
		fmt_string.insert(format_size*t, _T("\n"));
		string_size = string_size - format_size;
		t++;
	}

	output << "*** Lexical " << getSeverityString() << ":  "<< s.ArrayString().c_str()
		<< fmt_string.c_str();
	
	error_string = output.ArrayString();
	
}

void StreamError::Initialize(StreamErrorKind kind_, unsigned start,
                             unsigned end)
{
		kind = kind_;
		start_location = start;
		end_location = end;	
}

	void StreamError::CanculateDetail(BaseLexStream* lex_stream)
	{
		if (!initialized)
		{
			assert(lex_stream);
			left_line_no = lex_stream->FindLine(start_location);
			left_column_no = lex_stream->FindColumn(start_location - 1) + 1;
			right_line_no = lex_stream->FindLine(end_location);
			right_column_no = lex_stream->FindColumn(end_location);
			//fileName = lex_stream->FileName();
			emacs_style_report ? getemacsErrorString() : getregularErrorString(lex_stream);

			initialized = true;

		}
	}

	StreamError::StreamError() :JikesError(Problem::StreamError), start_location(0), end_location(0),
		kind(BAD_TOKEN),left_line_no(0), left_column_no(0), 
		right_line_no(0),right_column_no(0), initialized(false)
	{
		
	}

	




} // Close namespace Jikes block


