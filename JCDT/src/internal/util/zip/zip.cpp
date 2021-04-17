// $Id: zip.cpp,v 1.22 2002/07/30 16:30:03 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include <JCDT_Lib/internal/impl/FileBase.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>

namespace Jikes { // Open namespace Jikes block


//************************************************************
//
// The ZipFile methods follow
//
//************************************************************
#ifdef UNIX_FILE_SYSTEM
int (*ZipFile::uncompress_file[10]) (FILE *, char *, long) =
{
    UncompressFile0,
    UncompressFile1,
    UncompressFile2,
    UncompressFile3,
    UncompressFile4,
    UncompressFile5,
    UncompressFile6,
    UncompressFile7,
    UncompressFile8,
    UncompressFile9
};

inline u1 ZipFile::GetU1()
{
    return getc(zipfile);
}

inline void ZipFile::Skip(u4 length)
{
    for (u4 i = 0; i < length; i++)
        getc(zipfile);
}

#elif defined(WIN32_FILE_SYSTEM) // ! UNIX_FILE_SYSTEM


inline u1 ZipFile::GetU1()
{
    return *file_buffer++;
}

inline void ZipFile::Skip(u4 length)
{
    file_buffer += length;
}
#endif // WIN32_FILE_SYSTEM


inline u2 ZipFile::GetU2()
{
    u4 val = GetU1();
    val |= (((u4) GetU1()) << 8);

    return val;
}


inline u4 ZipFile::GetU4()
{
    u4 val = GetU1();
    val |= (((u4) GetU1()) << 8);
    val |= (((u4) GetU1()) << 16);
    val |= (((u4) GetU1()) << 24);

    return val;
}


ZipFile::ZipFile(Zip* zip, u4 uncompressed_size, u4 offset) :buffer(nullptr), file_buffer(nullptr)
{
	assert(zip);
	if (!zip->IsValid())
		return;

#ifdef UNIX_FILE_SYSTEM
    zipfile = zip -> zipfile;
    int rc = fseek(zipfile, file_symbol -> offset, SEEK_SET);

    assert(rc == 0);

#elif defined(WIN32_FILE_SYSTEM)
    file_buffer = &zip -> zipbuffer[ offset];
#endif

    Skip(8); // u4 magic                     = GetU4();
             // u2 version_needed_to_extract = GetU2();
             // u2 general_purpose_bits      = GetU2();
    u2 compression_method                    = GetU2();
    Skip(16); // u2 time                     = GetU2();
              // u2 date                     = GetU2();
              // u4 crc32                    = GetU4();
              // u4 compressed_size          = GetU4();
              // u4 uncompressed_size        = GetU4();
    u2 filename_length                       = GetU2();
    u2 extra_field_length                    = GetU2();
    Skip(filename_length + extra_field_length);

#ifdef UNIX_FILE_SYSTEM
    this -> buffer = new char[file_symbol -> uncompressed_size];
    if (! uncompress_file[compression_method < 9 ? compression_method : 9](zipfile, this -> buffer, file_symbol -> uncompressed_size))
    {
        delete [] this -> buffer;
        this -> buffer = NULL;
    }
#elif defined(WIN32_FILE_SYSTEM)
    if (compression_method > 0)
    {
        this -> buffer = new char[uncompressed_size];
        if (! (this->*uncompress_file[compression_method < 9 ? compression_method : 9])(file_buffer,
                                                                               this -> buffer,
                                                                               uncompressed_size))
        {
            delete [] this -> buffer;
            this -> buffer = NULL;
            this -> file_buffer = NULL;
        }
    }
#endif
}


ZipFile::~ZipFile()
{
    delete [] buffer;
}


	

	//********************************************************************
//
// The Zip methods follow:
//
//********************************************************************



Zip::Zip():  zipbuffer(nullptr), buffer_ptr(NULL)
{

#ifdef UNIX_FILE_SYSTEM
	FILE *zipfile = NULL;
#elif defined(WIN32_FILE_SYSTEM)
	zipfile =  INVALID_HANDLE_VALUE;
	mapfile = INVALID_HANDLE_VALUE;
#endif
	magic = (0);
	zipbuffer = (NULL);
}
void Zip::ReadZipContent(const wchar_t *zipfile_name)
{
	char* target = new char[wcslen(zipfile_name) * 3 + 1];
	CharOperation::ConvertUnicodeToUtf8(zipfile_name, target);
	ReadZipContent(target);
	delete[] target;
}
void Zip::ReadZipContent(const char *zipfile_name)
{

	Clear();

	magic = (0);
	zipbuffer = (NULL);

#ifdef UNIX_FILE_SYSTEM
    zipfile = SystemFopen(zipfile_name, "rb");
    if (zipfile)
    {
        int rc = fseek(zipfile, -END_SIZE, SEEK_END);
        if (rc == 0)
        {
            zipbuffer = new char[END_SIZE];
            buffer_ptr = zipbuffer;
            SystemFread(buffer_ptr, sizeof(char), END_SIZE, zipfile);

            magic = GetU4();
        }
    }
#elif defined(WIN32_FILE_SYSTEM)
	
    zipfile = CreateFileW(CharOperation::ConvertUtf8ToUnicode(zipfile_name).c_str(),
                         GENERIC_READ,
                         FILE_SHARE_READ,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_READONLY,
                         NULL);
    if (zipfile != INVALID_HANDLE_VALUE)
    {
        mapfile = CreateFileMapping(zipfile, NULL, PAGE_READONLY, 0, 0, NULL);
        zipbuffer = (mapfile == INVALID_HANDLE_VALUE ?
                     NULL :
                     (char *) MapViewOfFile(mapfile,
                                            FILE_MAP_READ,
                                            0, 0, 0)
                     );
        if (zipbuffer)
        {
            buffer_ptr = &zipbuffer[GetFileSize(zipfile, NULL) - END_SIZE];
            magic = GetU4();
        }
    }
#endif

    // The following was posted to the dev list, but was just
    // too good to not put in here, the next person to have to
    // deal with this crap will appreciate it. -=Chris
    //
    // From: Mo DeJong <supermo@bayarea.net>
    //
    //   Ode to a zip file:
    //
    //   I can't read it forwards
    //   I can't read it backwards
    //   I must know where to begin
    //   so I need to look in the middle
    //   to find the middle, I must know the end
    //   but I don't know where that is, so I guess
    //
    // -------------------------------------------------


    // This may or may not be a valid zip file. The zip file might have
    // a file comment so we can't be sure where the END header is located.
    // We check for the LOC header at byte 0 to make sure this is a valid
    // zip file and then scan over the file backwards in search of the
    // END header.

    if (zipbuffer != NULL && ! IsValid()) {
        u4 sig = 0;

#ifdef UNIX_FILE_SYSTEM
        int res = fseek(zipfile, 0, SEEK_SET);
        assert(res == 0);

        char *tmpbuffer = new char[LOC_SIZE];
        buffer_ptr = tmpbuffer;
        SystemFread(buffer_ptr, sizeof(char), LOC_SIZE, zipfile);
        sig = GetU4();
        delete [] tmpbuffer;
        buffer_ptr = NULL;

        if (sig == LOC_SIG)
        {
            int block_size = 8192;
            tmpbuffer = new char[block_size];
            char *holdbuffer = new char[8];
            char *index_ptr;

            res = fseek(zipfile, 0, SEEK_END);
            assert(res == 0);

            long zip_file_size = ftell(zipfile);
            int num_loops = zip_file_size / block_size;
            magic = 0;

            for (; magic == 0 && num_loops >= 0 ; num_loops--) {

                if ((ftell(zipfile) - block_size) < 0)
                {
                    block_size = ftell(zipfile);
                    res = fseek(zipfile, 0L, SEEK_SET);
                }
                else
                {
                    res = fseek(zipfile, -block_size, SEEK_CUR);
                }

                assert(res == 0);
                SystemFread(tmpbuffer, sizeof(char), block_size, zipfile);
                res = fseek(zipfile, -block_size, SEEK_CUR); // undo fread
                assert(res == 0);

                for (index_ptr = tmpbuffer + block_size - 1;
                     index_ptr >= tmpbuffer;
                     index_ptr--)
                {
                    if (*index_ptr == 'P')
                    {
                        // Check for header signature that spans buffer
                        int span = (tmpbuffer + block_size) - index_ptr;

                        if (span < 4)
                        {
                            memmove(holdbuffer+span, holdbuffer, 3);
                            memmove(holdbuffer, index_ptr, span);
                            buffer_ptr = holdbuffer;
                        }
                        else
                        {
                            buffer_ptr = index_ptr;
                        }

                        sig = GetU4();

                        if (sig == END_SIG)
                        {
                            // Found the END header, put it in zipbuffer.
                            buffer_ptr = zipbuffer;
                            fseek(zipfile, block_size-span, SEEK_CUR);
                            SystemFread(buffer_ptr, sizeof(char),
                                END_SIZE, zipfile);

                            magic = GetU4();
                            break;
                        }
                    }
                }

                // Copy first 3 bytes into holdbuffer in case sig spans
                holdbuffer[0] = tmpbuffer[0];
                holdbuffer[1] = tmpbuffer[1];
                holdbuffer[2] = tmpbuffer[2];
            }

            delete [] tmpbuffer;
            delete [] holdbuffer;
        }
#elif defined(WIN32_FILE_SYSTEM)
        buffer_ptr = &zipbuffer[0];
        sig = GetU4();

        if (sig == LOC_SIG)
        {
            buffer_ptr = &zipbuffer[GetFileSize(zipfile, NULL) - END_SIZE];
            for ( ; buffer_ptr >= zipbuffer; buffer_ptr--)
            {
                if (*buffer_ptr == 'P')
                {
                    sig = GetU4();
                    if (sig == END_SIG)
                    {
                       magic = sig;
                       break;
                    }
                    else
                       buffer_ptr -= 4;
                }
            }
        }
#endif
    }

    ReadDirectory();
}


Zip::~Zip()
{
	Zip::Clear();
}
void Zip::Clear()
{
#ifdef UNIX_FILE_SYSTEM

	delete[] zipbuffer;
	zipbuffer = NULL;
	if (zipfile)
		fclose(zipfile);
	zipfile = NULL;

#elif defined(WIN32_FILE_SYSTEM)

	if (zipfile != INVALID_HANDLE_VALUE)
	{
		if (mapfile != INVALID_HANDLE_VALUE)
		{
			if (zipbuffer)
				UnmapViewOfFile(zipbuffer);
			CloseHandle(mapfile);
		}
		CloseHandle(zipfile);
	}
	zipfile = INVALID_HANDLE_VALUE;
#endif

	
}




} // Close namespace Jikes block


