// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include <JCDT_Lib/internal/problem/ErrorInfo.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/problem/ErrorString.h>
#include <tchar.h>


namespace Jikes { // Open namespace Jikes block

	 const char* JikesError::getSeverityString()
	{
		using namespace Problem;
		auto _s = getSeverity();

		if (_s == JIKES_ERROR)
		{
			return "Error";
		}
		if (_s & JIKES_WARNING && _s & JIKES_CAUTION)
		{
			return "Warning and Caution";
		}

		if (_s & JIKES_CAUTION)
		{
			return "Caution";
		}
		if (_s & JIKES_WARNING)
		{
			return "Caution";
		}
		return "Unknown";

	}

void ErrorInfo::Initialize(LexStream* lex_stream)
	{
		if (!initialized)
		{
		
			assert(lex_stream);
			if (left_token) {
				left_line_no = lex_stream->LineFromPos(left_token->start_location);
				left_column_no = lex_stream->Column(left_token->index);
			}
			if (right_token){
				right_line_no = lex_stream->LineFromPos(right_token->start_location);
				right_column_no = lex_stream->RightColumn(right_token->index);
			}
			//fileName = lex_stream->FileName();
			emacs_style_report ? getemacsErrorString() : getregularErrorString(lex_stream);
			initialized = true;
		}
	}

	ErrorInfo::ErrorInfo()
		: JikesError(Problem::ErrorInfo), msg(NULL), num(0), msg_code(0),
		severity(Problem::JIKES_ERROR)
	{
		
	}

	ErrorInfo::~ErrorInfo()
	{
		delete msg;
	}


	Problem::Severity ErrorInfo::getSeverity()
{
	return severity;
}
	int ErrorInfo::getLeftLineNo() { return left_line_no; }
	int ErrorInfo::getLeftColumnNo() { return left_column_no; }
	int ErrorInfo::getRightLineNo() { return right_line_no; }
	int ErrorInfo::getRightColumnNo() { return right_column_no; }

	const char* ErrorInfo::getFileName()
	{
		return fileName.c_str();	
	}

	wstring ErrorInfo::getErrorMessage()
	{
		assert(msg);
		return msg;
	}

	bool ErrorInfo::emacs_style_report = false;

	wstring ErrorInfo::getErrorReport()
	{
		return error_string;
	}

	
		
	void ErrorInfo::getregularErrorString(LexStream* l)
	{
		ErrorString output;
		ErrorString s;
		l->OutputSource(this, s);
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
		
		output << "*** Semantic " << getSeverityString() << ":  " << s.ArrayString().c_str()
			<< fmt_string.c_str();
			
		error_string = output.ArrayString();
	}


	

	void ErrorInfo::getemacsErrorString()
	{
		ErrorString s;
		s << getFileName()
			<< ':' << left_line_no << ':' << left_column_no
			<< ':' << right_line_no << ':' << right_column_no
			<< ": Semantic " << getSeverityString() << ": "
			<< getErrorMessage().c_str();
		
		error_string = s.ArrayString();
		
	}

	// Returns the insert for the given index. Used to translate from the
	// numeric indices used in format strings to the appropriate member variable.
	const wchar_t* ErrorInfo::getInsert(unsigned index)
	{
		assert(index && index <= MAX_INSERTS);
		return insert[index - 1];
	}

	
} // Close namespace Jikes block



