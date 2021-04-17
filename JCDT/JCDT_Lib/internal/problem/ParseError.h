// $Id: JCDT_Lib/compiler/jikesapi.h,v 1.19 2004/02/28 17:09:45 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#ifndef ParseError_H_FLAG_
#define ParseError_H_FLAG_

#include <JCDT_Lib/internal/parser/LPG/javaprs.h>
#include <JCDT_Lib/internal/env/typedef.h>
#include <vector>

using std::vector;
namespace Jikes 
{
	
class FileSymbol;
enum ParseErrorCode;
class LexStream;
class BaseLexStream;
class Option;
class JikesError;
class ParseErrorInfo;

class ParseError : public javaprs_table
{
public:

    void Report(BaseLexStream* lex_stream, int msg_level, ParseErrorCode, int name_index,
                TokenIndex left_token, TokenIndex right_token,
		int scope_name_index = 0 );

    void SortMessages();

    ParseError(Option* option_ );

    void PrintMessages(BaseLexStream* lex_stream);
	void InitAllMessages(BaseLexStream* lex_stream);
	void Clear();
	
	void TransferError(vector<JikesError*>& errer_owner, BaseLexStream* lex_stream);
	unsigned NumErrors() const
	{ return errors.size(); }
	~ParseError();
private:
	
	Option* option;
    vector<ParseErrorInfo*> errors;



    void PrintMessage(BaseLexStream* lex_stream,int k);
};



} // Close namespace Jikes block


#endif // ParseError_H_FLAG_
