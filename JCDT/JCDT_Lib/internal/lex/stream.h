// $Id: JCDT_Lib/internal/lex/LexStream.h,v 1.53 2004/03/25 13:32:28 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef stream_INCLUDED
#define stream_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"

struct UConverter;
namespace Jikes { // Open namespace Jikes block

//
// The stream class encapsulates details related to reading
// a stream of possibly encoded data from the file system.
//
class Stream
{
public:

    Stream();
    ~Stream();

    void DestroyInput()
    {
        delete [] input_buffer;
        input_buffer = NULL;
    }

    inline const wchar_t* InputBuffer() { return input_buffer; }
    inline unsigned InputBufferLength() { return input_buffer_length; }

    inline wchar_t* AllocateInputBuffer(unsigned size)
    {
        // +3 for leading \n, trailing \r\0
        return input_buffer = new wchar_t[size + 3];
    }

#if defined(HAVE_ENCODING)
    static bool IsSupportedEncoding(const char* encoding);
    bool SetEncoding(const char* encoding);
#endif

protected:

    wchar_t* input_buffer;
    unsigned input_buffer_length;

    const char* source_ptr;    // Start of data buffer to decoded
    const char* source_tail;   // End of data buffer to be decoded
    const char* data_buffer;   // The data to be decoded

    bool error_decode_next_character;

//private: // FIXME : Make vars private once extracted from LexStream!

#ifdef HAVE_ENCODING


    UConverter* _decoder;

    void DestroyEncoding();

    // Read the next wchar_t from the stream.
    // If an error occurs the ErrorDecodeNextCharacter
    // method will return true on the next call.

    wchar_t DecodeNextCharacter();

    inline bool ErrorDecodeNextCharacter()
    {
        bool result = error_decode_next_character;
        if (result)
            error_decode_next_character = false;
        return result;
    }

    // Returns true if an encoding has been set

    inline bool HaveDecoder(){
        return _decoder != NULL;
    }

#endif // HAVE_ENCODING

    inline void InitializeDataBuffer(const char* buffer, long size)
    {
        data_buffer = buffer;
        source_ptr = data_buffer;
        source_tail = data_buffer + size - 1;
    }

    inline bool HasMoreData()
    {
        return source_ptr <= source_tail;
    }
};



} // Close namespace Jikes block


#endif // stream_INCLUDED

