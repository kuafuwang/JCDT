// $Id: scanner.h,v 1.17 2004/03/25 13:32:28 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef scanner_INCLUDED
#define scanner_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include <stack>
using std::stack;

namespace Jikes { // Open namespace Jikes block


class LookupEnvironment;
class FileSymbol;
class Option;
class INameEnvironment;

template <typename T>
class BraceStack
{
public:
	void push(T brace) { table.Next() = brace; }
	void pop()
	{
		if (table.Length() > 0)
			table.Reset(table.Length() - 1);
	}
	int size() { return table.Length(); }
	T top()
	{
		return table.Length() > 0 ? table[table.Length() - 1] : T();
	}
	void clear()
	{
		table.Reset();
	}

	Tuple<T > table;
};
//
// The Scanner object
//
class Scanner
{
public:

    Scanner(Option*, INameEnvironment*);
    ~Scanner() { }

    void SetUp(FileBase*);
    void Scan(FileBase*);
	void Scan(BaseLexStream*);

private:
  
	INameEnvironment* nameEnv;
	Option* option;
    BaseLexStream* lex;
    const wchar_t* cursor;
    const wchar_t* input_buffer_tail;
    bool dollar_warning_given;
    bool deprecated; // true if the next token should be marked deprecated

    Token* current_token;
    TokenIndex current_token_index;

    void Initialize();
    void Scan();

    static int (*scan_keyword[13]) (const wchar_t* p1);
    static int ScanKeyword0(const wchar_t* p1);
    static int ScanKeyword2(const wchar_t* p1);
    static int ScanKeyword3(const wchar_t* p1);
    static int ScanKeyword4(const wchar_t* p1);
    static int ScanKeyword5(const wchar_t* p1);
    static int ScanKeyword6(const wchar_t* p1);
    static int ScanKeyword7(const wchar_t* p1);
    static int ScanKeyword8(const wchar_t* p1);
    static int ScanKeyword9(const wchar_t* p1);
    static int ScanKeyword10(const wchar_t* p1);
    static int ScanKeyword12(const wchar_t* p1);

    inline void SkipSpaces();
    void ScanSlashComment();
    void ScanStarComment();

	BraceStack< TokenIndex > lbrace_stack;

//	std::stack<TokenIndex> lbrace_stack;
	int lbrace_count = 0;
	int rbrace_count = 0;
    void (Scanner::*classify_token[128 + 1])();

    void ClassifyCharLiteral();
    void ClassifyStringLiteral();
    void ClassifyIdOrKeyword();
    void ClassifyId();
    void ClassifyNumericLiteral();
    void ClassifyColon();
    void ClassifyPlus();
    void ClassifyMinus();
    void ClassifyStar();
    void ClassifySlash();
    void ClassifyLess();
    void ClassifyGreater();
    void ClassifyAnd();
    void ClassifyOr();
    void ClassifyXor();
    void ClassifyNot();
    void ClassifyEqual();
    void ClassifyMod();
    void ClassifyPeriod();
    void ClassifySemicolon();
    void ClassifyComma();
    void ClassifyLbrace();
    void ClassifyRbrace();
    void ClassifyLparen();
    void ClassifyRparen();
    void ClassifyLbracket();
    void ClassifyRbracket();
    void ClassifyComplement();
    void ClassifyAt();
    void ClassifyBadToken();
    void ClassifyQuestion();
    void ClassifyEof();

    void ClassifyNonAsciiUnicode();
};


} // Close namespace Jikes block


#endif // scanner_INCLUDED

