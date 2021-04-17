// $Id: JCDT_Lib/internal/lex/stream.h,v 1.53 2004/03/25 13:32:28 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef LexStream_INCLUDED
#define LexStream_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/impl/FileHelper.h"
#include "JCDT_Lib/internal/lex/Token.h"
#include "JCDT_Lib/internal/problem/StreamError.h"
#include "stream.h"
#include <vector>
#include "Comment.h"

#include <JCDT_Lib/internal/impl/FileBase.h>
#include <boost/pool/object_pool.hpp>
#include <JCDT_Lib/internal/util/tab.h>
using std::vector;

namespace Jikes { // Open namespace Jikes block



class Input_info;
class Scanner;
class Symbol;
class FileSymbol;
class ZipFile;
class BaseLexStream;
class ErrorString;
class RecoveryScanner;
class LiteralSymbol;
class NameSymbol;
class Option;


class BaseLexStream : public Stream
{
    friend class StreamError;

public:
	virtual const char* FileName(){
		return  "";
	}
	virtual unsigned FileNameLength(){
		return 0;
	}
#if defined(HAVE_ENCODING)
	enum UnicodeLexerState
	{
		START,
		RAW,
		CR,
		QUOTE,
		UNICODE_ESCAPE,
		UNICODE_ESCAPE_DIGIT_0,
		UNICODE_ESCAPE_DIGIT_1,
		UNICODE_ESCAPE_DIGIT_2
	};
#endif // HAVE_ENCODING

	friend class Scanner;
	friend class RecoveryScanner;
	friend class LookupEnvironment;


	virtual void RereadInput()
	{
		
	};
	virtual void ReadInput(){
	}

    typedef unsigned CommentIndex;
    enum
    {
		LEX_START = 1,
	    LEX_INFINITY = INT_MAX
    }; // the largest value for TokenIndex

	inline void SetTokenNumAsEndTokenIndex(){
		end_token_index = token_stream.size() - 1;
	}
	inline void SetEndTokenIndex(TokenIndex _idx){ 
		if (_idx >= token_stream.size()) {
			end_token_index = token_stream.size() -1;
		}
		else{
			end_token_index = _idx;
		}
	}
	inline TokenIndex GetEndTokenIndex(){
		auto num = token_stream.size();
		if (end_token_index >= num) {
			end_token_index = num -1;
		}
		return end_token_index;
	}
    inline TokenIndex Next(TokenIndex i)
    {
		++i;
		if(i < end_token_index + 1){
			return i;
		}
		return token_stream.size() - 1;
    }
    inline TokenIndex Previous(TokenIndex i)
	{
		return i <= 0 ? 0 : i - 1;
	}
    inline TokenIndex Peek()
	{
		return Next(index);
	}
    inline void Reset(TokenIndex i = LEX_START)
	{
		index = Previous(i);
	}
	
	inline  Token* getTokenObjNoIndexNoNext(TokenIndex tokenIndex){
		return token_stream[tokenIndex];
    }

    inline TokenIndex Gettoken()
	{
		return index = Next(index);
	}
    /*inline TokenIndex Gettoken(TokenIndex end_token)
    {
        return index = (index < end_token ? Next(index)
                        : token_stream.size() - 1);
    }*/

    inline unsigned Kind(TokenIndex i)
    {
		auto _size = token_stream.size();
        return token_stream[i >= _size ? _size - 1 : i]->Kind();
    }
	
	
	inline unsigned start(TokenIndex i)
	{
		assert(i < NumTokens());
		return token_stream[i]->start_location;
	}
	inline unsigned end(TokenIndex i)
    {
		return token_stream[i]->EndLocation();
    }
    inline unsigned Location(TokenIndex i)
    {
        assert(i < NumTokens());
        return token_stream[i]->start_location;
    }
	inline unsigned LineFromPos(unsigned pos)
	{
		return FindLine(pos);
	}
    inline unsigned Line(TokenIndex i)
    {
        return FindLine(token_stream[i]->start_location);
    }
	inline bool isInsideStream(TokenIndex i) const {
		 return token_stream.size() - 1 > i;
	}
	inline unsigned ColumnFromPos(unsigned pos)
	{
		return input_buffer ? FindColumn(pos - 1) + 1 : 0;
		
	}
    inline unsigned Column(TokenIndex i){
        // FindColumn grabs the right edge of an expanded character.
        return input_buffer ? FindColumn(token_stream[i]->start_location - 1) + 1 : 0;
    }
	inline unsigned RightColumnFromPos(unsigned pos)
	{
		if (!input_buffer)
			return 0;
		return FindColumn(pos);
	};

	inline unsigned RightColumn(TokenIndex i);

    inline bool AfterEol(TokenIndex i)
    {
        return i < 1 ? true : Line(i - 1) < Line(i);
    }

    inline bool IsDeprecated(TokenIndex i) { return token_stream[i]->Deprecated(); }

	

	inline const wchar_t* NameString(TokenIndex i);
	inline unsigned NameStringLength(TokenIndex i);

	inline wstring  getNameString(TokenIndex i);

	inline  LiteralSymbol* GetLiteralSymbol(TokenIndex);
	inline  NameSymbol* GetNameSymbol(TokenIndex);

    

	inline  unsigned LineLength(unsigned line_no);
    inline unsigned LineStart(unsigned line_no) const
    {
        return locations[line_no];
    }
    inline unsigned LineEnd(unsigned line_no) const
    {
        return locations[line_no + 1] - 1;
    }

    unsigned LineSegmentLength(TokenIndex i);

    //
    // For a sequence of tokens in a given range find out how many large
    // characters they contain and compute the appropriate offset.
    //
    inline unsigned WcharOffset(TokenIndex start, TokenIndex end)
    {
        unsigned offset = 0;
        for (TokenIndex i = start; i <= end; i++)
        {
            for (const wchar_t* str = NameString(i); *str; str++)
            {
                if (*str > 0xff)
                    offset += 5;
            }
        }

        return offset;
    }

    //
    // When only an end token is supplied, the start token is assume to be the
    // first one on the same line.
    //
    inline unsigned WcharOffset(TokenIndex end)
    {
        TokenIndex start = end;
        unsigned the_line = Line(end);
        while (Line(--start) == the_line);
        return WcharOffset(start + 1, end);
    }

    //
    // Used for outputting sections of source code in error messages.
    //
	void GetText(unsigned, unsigned, wstring&);
    void OutputLine(unsigned, ErrorString&);
    void OutputSource(JikesError*, ErrorString&);

    CommentIndex FirstComment(TokenIndex);

 
    inline unsigned NumTokens() { return token_stream.size(); }
    inline unsigned NumComments() { return comment_stream.size(); }
    inline TokenIndex PrecedingToken(CommentIndex i)
    {
        return comment_stream[i]->previous_token;
    }
    inline unsigned CommentLocation(CommentIndex i)
    {
        return comment_stream[i]->location;
    }

    inline const wchar_t* CommentString(CommentIndex i)
    {
        return comment_stream[i]->string;
    }

    inline unsigned CommentStringLength(CommentIndex i)
    {
        return comment_stream[i]->length;
    }

    inline TokenIndex PackageToken()
    {
        return package;
    }

    inline unsigned NumErrorTokens()
    {
        unsigned count = 0;
        for (unsigned i = 0; i < bad_token_stream.size(); i++)
            if (bad_token_stream[i]->getSeverity() == Problem::JIKES_ERROR)
                count++;
        return count;
    }
    inline unsigned NumWarnTokens()
    {
        return bad_token_stream.size() - NumErrorTokens();
    }
	inline unsigned NumBadTokens()
	{
		return bad_token_stream.size();
	}

#ifdef JIKES_DEBUG
    bool file_read;
#endif

    //
    // Constructors and Destructor.
    //
    BaseLexStream(Option*);

    virtual ~BaseLexStream();

	void DestroyInput();
	void ReportMessage(StreamError::StreamErrorKind,
		unsigned start, unsigned end);
	void SortMessages();

  virtual	void SetUpComments();


    //
    // Return the total size of space allocated for the tokens.
    //
    size_t TokenSpaceAllocated(void)
    {
        return token_stream.size() * sizeof(Token);
    }

    //
    // Return the total size of space allocated for the comments.
    //
    size_t CommentSpaceAllocated(void)
    {
        return comment_stream.size() * sizeof(Comment);
    }
	virtual void ChangeTokenStream(vector<Token*>& _token_stream)
    {
		token_stream.swap(_token_stream);
		
		Reset();
    }
	void RccordeBraceBalance(bool _b);
	bool IsBraceBalance() const;
	Token* getNewAndUnInitToken(unsigned _location);
	boost::object_pool<Token>* pool = nullptr;
	void TransferError(vector<JikesError*>& errer_owner);
	void ProcessInput(const char*, long);
	vector<Token*> token_stream;

	Token* GetNextTokenAndReturnToken(unsigned location);

protected:
	TokenIndex GetNextToken(unsigned location = 0);

	static int hexvalue(wchar_t ch);

	bool brace_balance = true;
	
	
	vector<StreamError*> bad_token_stream;
	
	vector<Comment*> comment_stream;


    TokenIndex index;
	TokenIndex end_token_index = LEX_INFINITY;
	

	Comment* NewComent(){

		Comment* c = new Comment();
		comment_stream.push_back(c);
		return c;
	}
    unsigned* locations;
	unsigned  line_length = 0;
    ConvertibleArray<unsigned> line_location;
   
    TokenIndex package;

    void CompressSpace();

    bool initial_reading_of_input;

    wchar_t* comment_buffer;

    
	Option* option;

#if defined(HAVE_ENCODING)
    void ProcessInputUnicode(const char*, long);
#else
    void ProcessInputAscii(const char*, long);
#endif // defined(HAVE_ENCODING)

  

    unsigned FindLine(unsigned location);

    //
    // Finds the column of the right edge of a character.
    //
    unsigned FindColumn(unsigned loc);
	
public :
	// look up 
	unsigned FindPosition(unsigned line_no, unsigned column_no);
	int FindCommentIndex(unsigned int pos) ;
	int FindBadTokenIndex(unsigned int pos) ;
	int FindGoodTokenIndex(unsigned int pos);
};



inline int BaseLexStream::FindCommentIndex(unsigned int pos) {
	auto _size = comment_stream.size();
	if (_size)
	{
		return BinarySearch<Comment>(comment_stream.data(), _size, pos);
	}
		
	return PositionNode::NO_FOUND;
}

inline int BaseLexStream::FindBadTokenIndex(unsigned int pos) {
	auto _size = bad_token_stream.size();
	if (_size)
	{
		return BinarySearch<StreamError>(bad_token_stream.data(), _size, pos);
	}
	
	return PositionNode::NO_FOUND;
}

inline int BaseLexStream::FindGoodTokenIndex(unsigned int pos)
{
	auto _size = token_stream.size();
	if (_size > 1)
	{
		return BinarySearch<Token>(token_stream.data(), _size, pos);
	}
	
	return PositionNode::NO_FOUND;
}

inline bool BaseLexStream::IsBraceBalance() const
{
	return brace_balance;
}
inline void BaseLexStream::RccordeBraceBalance(bool _b)
{
	brace_balance = _b;
}

inline Token* BaseLexStream::getNewAndUnInitToken(unsigned _location)
{
	return pool->construct(_location);
	
}


inline void BaseLexStream::ProcessInput(const char* buffer, long filesize)
{
#if defined(HAVE_ENCODING)

	BaseLexStream::ProcessInputUnicode(buffer, filesize);

#else // defined(HAVE_ENCODING)

	BaseLexStream::ProcessInputAscii(buffer, filesize);
#endif // ! defined(HAVE_ENCODING)

}


inline  unsigned BaseLexStream::RightColumn(TokenIndex i)
{
	if (!input_buffer)
		return 0;
	unsigned location = token_stream[i]->EndLocation();
	return FindColumn(location);
}

inline const wchar_t* BaseLexStream::NameString(TokenIndex i)
{
	return token_stream[i]->NameString();
}
inline wstring BaseLexStream::getNameString(TokenIndex i) {
	return token_stream[i]->getName();
}

inline unsigned BaseLexStream::NameStringLength(TokenIndex i)
{
	return token_stream[i]->NameStringLength();
}

inline unsigned BaseLexStream::LineLength(unsigned line_no)
{
	assert(input_buffer && locations);
	return Tab::Wcslen(input_buffer, locations[line_no],
		locations[line_no + 1] - 2); // ignore the \n
}

inline unsigned BaseLexStream::LineSegmentLength(TokenIndex i)
{
	return Tab::Wcslen(input_buffer, token_stream[i]->start_location,
		LineEnd(Line(i)));
}

//
// If the token represents a literal, this returns the literal symbol
// associated with it.
//
inline LiteralSymbol* BaseLexStream::GetLiteralSymbol(TokenIndex i)
{
	assert(i < (unsigned)token_stream.size());
	return token_stream[i]->GetLiteralSymbol();
}


//
// If the token represents a literal, this returns the name symbol
// associated with it.
//
inline NameSymbol* BaseLexStream::GetNameSymbol(TokenIndex i)
{
	assert(i < (unsigned)token_stream.size());
	return token_stream[i]->GetNameSymbol();
}



inline void BaseLexStream::CompressSpace()
{
	SortMessages();
	locations = line_location.Array();
	line_length = line_location.Length();

	for (size_t i = 0, size = bad_token_stream.size(); i < size; ++i) {
		bad_token_stream[i]->CanculateDetail(this);
	}
}

inline unsigned BaseLexStream::FindColumn(unsigned loc)
{
	assert(locations);
	return input_buffer[loc] == U_LINE_FEED ? 0
		: Tab::Wcslen(input_buffer, locations[FindLine(loc)], loc);
}
inline unsigned BaseLexStream::FindPosition(unsigned line_no, unsigned column_no)
{

	if (line_length > line_no)
	{
		int start = locations[line_no];
		int end = locations[line_no + 1] - 1;
		return Tab::GetPos(input_buffer, start, end, column_no);
	}
	else
	{
		assert(false);
	}
	return 0;
}

class MemorySourceLexStream: public BaseLexStream
{
public:
	MemorySourceLexStream(Option* option)
		: BaseLexStream(option)
	{
	}
	void SetSource(const char* source, long size);

	
};

inline void MemorySourceLexStream::SetSource(const char* source, long size)
{
	ProcessInput(source, size);
}
class LexStream : public BaseLexStream
{
public:
#ifdef JIKES_DEBUG
	void Dump(); // temporary function used to dump token stream.
#endif

	LexStream(Option* option_, FileBase* file_symbol_);
	const char* FileName() override;
	unsigned FileNameLength()override;
	 void ReadInput() override;
	 void RereadInput() override;


	 void PrintMessages();


	FileBase* file_symbol;

};


//
//
} // Close namespace Jikes block


#endif // stream_INCLUDED

