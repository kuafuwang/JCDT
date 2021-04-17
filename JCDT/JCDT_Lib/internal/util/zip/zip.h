// $Id: zip.h,v 1.12 2001/12/13 00:40:15 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef zip_INCLUDED
#define zip_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"

#include "JCDT_Lib/internal/util/tuple.h"
#include <JCDT_Lib/internal/util/zip/unzip.h>


namespace Jikes {
	class INameEnvironment;
	// Open namespace Jikes block


class Zip;



class ZipFile : public Unzip
{
public:

   
    ~ZipFile();
	ZipFile(Zip *zip, u4 uncompressed_size, u4 offset);
	
private:
    char *buffer;
    u1 GetU1();
    u2 GetU2();
    u4 GetU4();
    void Skip(u4 length);

#ifdef UNIX_FILE_SYSTEM
        FILE *zipfile;
        static int (*uncompress_file[10]) (FILE *, char *, long);
    public:
        inline char *Buffer() { return buffer; }
#elif defined(WIN32_FILE_SYSTEM)
        char *file_buffer;
        
    public:
        inline char *Buffer() { return (buffer ? buffer : file_buffer); }
#endif
};

class Zip
{
public:
  
	Zip();
	virtual ~Zip();

    bool IsValid() const{ return magic == END_SIG; }
	void ReadZipContent(const wchar_t *);
	void ReadZipContent(const char *);
   

protected:
    friend class ZipFile;

	
    enum
    {
        LOC_SIG = 0x04034b50,
        LOC_SIZE = 30,
        DATA_SIG = 0x08074b50,
        DATA_SIZE = 16,
        CEN_SIG = 0x02014b50,
        CEN_SIZE = 46,
        END_SIG = 0x06054b50,
        END_SIZE = 22
    };

    u4 magic;

 

    char *zipbuffer,
         *buffer_ptr;

	inline u1 GetU1();
    inline u2 GetU2();
	inline u4 GetU4();
	inline void Skip(u4 length);

	virtual  void ReadDirectory() = 0;


	virtual void Clear();

#ifdef UNIX_FILE_SYSTEM
    FILE *zipfile;
#elif defined(WIN32_FILE_SYSTEM)
    HANDLE zipfile, mapfile;
#endif


};

//********************************************************************
//
// The Zip methods follow:
//
//********************************************************************
inline u1 Zip::GetU1()
{
	return *buffer_ptr++;
}


inline u2 Zip::GetU2()
{
	u4 val = GetU1();
	val |= (((u4)GetU1()) << 8);

	return val;
}


inline u4 Zip::GetU4()
{
	u4 val = GetU1();
	val |= (((u4)GetU1()) << 8);
	val |= (((u4)GetU1()) << 16);
	val |= (((u4)GetU1()) << 24);

	return val;
}


inline void Zip::Skip(u4 length)
{
	buffer_ptr += length;
}

} // Close namespace Jikes block


#endif // zip_INCLUDED

