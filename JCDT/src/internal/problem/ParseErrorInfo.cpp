// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/problem/ParseErrorInfo.h"
#include <JCDT_Lib/internal/parser/LPG/lpginput.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/problem/ErrorString.h>
#include <tchar.h>
#include <JCDT_Lib/internal/problem/ParseErrorCode.h>
#include <JCDT_Lib/internal/problem/ParseError.h>

namespace Jikes { // Open namespace Jikes block


	void ParseErrorInfo::Initialize(BaseLexStream* lex_stream)
	{
		if (!initialized){
			assert(lex_stream);
			//fileName = lex_stream->FileName();
			left_line_no = lex_stream->Line(left_token);
			left_column_no = lex_stream->Column(left_token);
			right_line_no = lex_stream->Line(right_token);
			right_column_no = lex_stream->RightColumn(right_token);

			emacs_style_report ? getemacsErrorString() : getregularErrorString(lex_stream);
			initialized = true;
			
		}

	}

	int ParseErrorInfo::getLeftLineNo() { return left_line_no; }
	int ParseErrorInfo::getLeftColumnNo() { return left_column_no; }
	int ParseErrorInfo::getRightLineNo() { return right_line_no; }
	int ParseErrorInfo::getRightColumnNo() { return right_column_no; }

	Problem::Severity ParseErrorInfo::getSeverity()
	{
		return Problem::JIKES_ERROR;
	}

	const char* ParseErrorInfo::getFileName()
	{
		return fileName.c_str();
	}

	ParseErrorInfo::ParseErrorInfo():JikesError(Problem::ParseErrorInfo),
		left_line_no(0), left_column_no(0),
		right_line_no(0), right_column_no(0), left_token(0), 
		right_token(0), name_index(0), num(0), msg_level(0),
		msg_code(ERROR_CODE), scope_name_index(0), initialized(false)
	{
	}

	wstring ParseErrorInfo::getErrorMessage()
	{
		ErrorString s;
		const wchar_t* name = nullptr;
		int i, len = 0;

#if defined(FULL_DIAGNOSIS)
		if (name_index >= 0)
		{
			len = ParseError::name_length[name_index];
			name = &ParseError::string_buffer[ParseError::name_start[name_index]];
		}
#endif

		switch (msg_code)
		{
		case ERROR_CODE:
			s << "Parsing terminated at this token";
			break;

		case BEFORE_CODE:
			for (i = 0; i < len; i++)
				s << name[i];
			s << " inserted before this token";
			break;

		case INSERTION_CODE:
			for (i = 0; i < len; i++)
				s << name[i];
			s << " expected after this token";
			break;

		case DELETION_CODE:
			if (left_token == right_token)
				s << "Unexpected symbol ignored";
			else
				s << "Unexpected symbols ignored";
			break;

		case INVALID_CODE:
			if (len == 0)
				s << "Unexpected input discarded";
			else
			{
				s << "Invalid ";
				for (i = 0; i < len; i++)
					s << name[i];
			}
			break;

		case SUBSTITUTION_CODE:
			for (i = 0; i < len; i++)
				s << name[i];
			s << " expected instead of this token";
			break;

#if defined(SCOPE_REPAIR)
		case SCOPE_CODE:
			s << '\"';
			for (i = ParseError::scope_suffix[-(int)name_index];
			ParseError::scope_rhs[i] != 0; i++)
			{
				len = ParseError::name_length[ParseError::scope_rhs[i]];
				name = &ParseError::string_buffer[ParseError::name_start[ParseError::scope_rhs[i]]];
				string namedddd(name, name + len);
				for (int j = 0; j < len; j++)
					s << name[j];
				if (ParseError::scope_rhs[i + 1]) // any more symbols to print?
					s << ' ';
			}
			s << '\"';
			s << " inserted to complete ";
			//
			// TODO: This should not be an option
			//
			if (scope_name_index)
			{
				len = ParseError::name_length[scope_name_index];
				name = &ParseError::string_buffer[ParseError::name_start[scope_name_index]];
				for (int j = 0; j < len; j++) // any more symbols to print?
					s << name[j];
			}
			else
				s << "scope";
			break;
#endif

		case MANUAL_CODE:
			s << '\"';
			for (i = 0; i < len; i++)
				s << name[i];
			s << "\" inserted to complete structure";
			break;

		case MERGE_CODE:
			s << "symbols merged to form ";
			for (i = 0; i < len; i++)
				s << name[i];
			break;

		case EOF_CODE:
			for (i = 0; i < len; i++)
				s << name[i];
			s << " reached after this token";
			break;

		case MISPLACED_CODE:
			s << "misplaced construct(s)";
			break;

		default:
			if (len == 0)
			{
				s << "unexpected input discarded";
			}
			else
			{
				for (i = 0; i < len; i++)
					s << name[i];
				s << " expected instead";
			}
			break;
		}

	
		return s.ArrayString();
	}

	bool ParseErrorInfo::emacs_style_report = false;

	 wstring ParseErrorInfo::getErrorReport()
	{
		return error_string;
	}



	void ParseErrorInfo::getregularErrorString(BaseLexStream* lex_stream)
	{
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
		
		output << "*** Syntax " << getSeverityString() << ":  " << s.ArrayString().c_str()
			<< fmt_string.c_str();

		error_string = output.ArrayString();
	}

	
		void ParseErrorInfo::getemacsErrorString()
	{
		ErrorString s;
		s << getFileName()
			<< ':' << left_line_no << ':' << left_column_no
			<< ':' << right_line_no << ':' << right_column_no
			<< ": Syntax " << getSeverityString() << ": "
			<< getErrorMessage().c_str();
		error_string = s.ArrayString();
	}



	

} // Close namespace Jikes block



