// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ErrorInfo_INCLUDED
#define ErrorInfo_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/problem/JikesError.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include "ErrorString.h"

namespace Jikes { // Open namespace Jikes block


	class LexStream;
	class SemanticError;

	class ErrorInfo : public JikesError
	{
		friend class SemanticError;

	public:
		virtual wstring getErrorMessage();
		virtual wstring getErrorReport();

		virtual Problem::Severity getSeverity();
		virtual const char* getFileName();

		virtual int getLeftLineNo();
		virtual int getLeftColumnNo();
		virtual int getRightLineNo();
		virtual int getRightColumnNo();

		const wchar_t* getInsert(unsigned which);

		 ErrorInfo();
		virtual ~ErrorInfo();

		TokenIndex LeftIndex() const;

		TokenIndex RightIndex() const;

	public:
		int left_line_no = INVALID_LEFT_COLUMN_NO;
		int left_column_no = INVALID_LEFT_COLUMN_NO;
		int right_line_no = INVALID_LEFT_LINE_NO;
		int right_column_no = INVALID_RIGHT_COLUMN_NO;

		Token* left_token = nullptr;
		Token* right_token = nullptr;

		enum { MAX_INSERTS = 9 };
		const wchar_t* insert[MAX_INSERTS];
		const wchar_t* msg;
		unsigned num;
		short msg_code;
		Problem::Severity severity;

		static bool emacs_style_report;
		
		wstring& regularErrorString();
		const wchar_t* emacsErrorString();

		void Initialize(LexStream*);
		bool initialized = false;
	protected:
		wstring error_string;
		string fileName;
	
		void getemacsErrorString();
		void getregularErrorString(LexStream*);
};

	inline TokenIndex ErrorInfo::LeftIndex() const
	{
		return left_token ? left_token->index : BAD_TOKEN;
	}

	inline TokenIndex ErrorInfo::RightIndex() const
	{
		return right_token ? right_token->index : BAD_TOKEN;
	}
} // Close namespace Jikes block


#endif // ErrorInfo_INCLUDED

