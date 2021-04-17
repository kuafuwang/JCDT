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
#include <unicode/ucnv_err.h>
#include <unicode/ucnv.h>

namespace Jikes { // Open namespace Jikes block


// Class Stream

Stream::Stream()
	: input_buffer(NULL),
	  input_buffer_length(0)
	  , source_ptr(nullptr), source_tail(nullptr), data_buffer(nullptr), error_decode_next_character(false),
	  _decoder(NULL)

{
}

Stream::~Stream()
{
    DestroyInput();
#ifdef HAVE_ENCODING
    DestroyEncoding();
#endif // HAVE_ENCODING
}

#ifdef HAVE_ENCODING

// This method will return true is the given encoding
// can be supported, it is static because we need to
// be able to query encodings without an instance.

bool Stream::IsSupportedEncoding(const char* encoding)
{
    // Create a tmp object instead of duplicating
    // the code in SetEncoding and DestroyEncoding
    Stream* tmp = new Stream();
    bool supported = tmp -> SetEncoding(encoding);
    delete tmp;
    return supported;
}

bool Stream::SetEncoding(const char* encoding)
{
    assert(encoding);
    DestroyEncoding();


    UErrorCode err = U_ZERO_ERROR;
    _decoder = ucnv_open(encoding, &err);


    return HaveDecoder();
}

void Stream::DestroyEncoding()
{
    if (HaveDecoder()){
        ucnv_close(_decoder);
        _decoder = NULL;
    }
}

wchar_t Stream::DecodeNextCharacter()
{
    const char* before = source_ptr;
    wchar_t next;
    error_decode_next_character = false;


    if (!HaveDecoder())
        return (wchar_t) *source_ptr++;

    UErrorCode err = U_ZERO_ERROR;
    next = ucnv_getNextUChar(_decoder, &source_ptr, source_tail + 1, &err);

    if (U_FAILURE(err)){
        fprintf(stderr,"Conversion error: %s at byte %d\n",
            u_errorName(err),
            int(before - data_buffer)
        );
        error_decode_next_character = true;
        return 0;
    }

    if (before == source_ptr){
        //End of conversion
        error_decode_next_character = true;
        return 0;
    }

    return next;
}

#endif // HAVE_ENCODING



} // Close namespace Jikes block


