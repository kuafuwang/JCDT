// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef FileUtil_INCLUDED
#define FileUtil_INCLUDED


namespace Jikes { // Open namespace Jikes block

	/**
	* Define the virtual base class for all Readers.
	* A pointer to an object of this type is returned by Compiler::read()
	*/
	class FileReader
	{
	public:
		virtual ~FileReader() {}

		// If the file is unreadable an object should still be created but
		// GetBuffer() should return NULL.
		virtual const char* getBuffer() = 0;
		// If the file is unreadable GetBufferSize() is undefined.
		virtual size_t getBufferSize() = 0;
	};

	/**
	* Define the virtual base class for all WriteObjects.
	* A pointer to an object of this type is returned by Compiler::write()
	*/
	class FileWriter
	{
	public:
		FileWriter(size_t mS) : maxSize(mS) {}
		virtual ~FileWriter() {}

		size_t write(const unsigned char* data, size_t size);
		virtual int isValid() = 0;

	private:
		// Guaranteed not to be called with a combined total of more than
		// maxSize bytes during the lifespan of the object.
		virtual size_t doWrite(const unsigned char* data, size_t size) = 0;
		size_t maxSize;
	};

	//
	// A default implementation of ReadObject that read from the file sysytem.
	//
	class DefaultFileReader : public FileReader
	{
	public:

		DefaultFileReader(const char *fileName);
		virtual ~DefaultFileReader();

		virtual const char* getBuffer() { return buffer; }
		virtual size_t getBufferSize() { return size; }

	private:

		const char* buffer;
		size_t size;

		// FIXME : need to move into platform.h
#ifdef WIN32_FILE_SYSTEM
		HANDLE srcfile;
		HANDLE mapfile;
#endif // WIN32_FILE_SYSTEM
	};


	class VirtualFileReader : public FileReader
	{
	public:

		VirtualFileReader(){};
		virtual ~VirtualFileReader(){};
		char buff[1];
		virtual const char* getBuffer() { return buff; }
		virtual size_t getBufferSize() { return 1; }
	};

	//
	// A default implementation of WriteObject that writes to the file system.
	//
	class DefaultFileWriter : public FileWriter
	{
	public:
		DefaultFileWriter(const char *fileName, size_t maxSize);
		virtual ~DefaultFileWriter();

		virtual int isValid();

	private:

		virtual size_t doWrite(const unsigned char *data, size_t size);

		// Note that we don't use the bool type anywhere in jikesapi.h
		// since it is not supported by some compilers. We can't
		// depend on the typedef in platform.h because jikesapi.h
		// should never include build time files.
		int valid;

		// FIXME: need to clean this up, why is this not wrapped in a platform.h function?
#ifdef UNIX_FILE_SYSTEM
		FILE *file;
#elif defined(WIN32_FILE_SYSTEM)
		HANDLE file;
		HANDLE mapfile;
		u1 *string_buffer;
		size_t dataWritten;
#endif
	};

} // Close namespace Jikes block


#endif // _INCLUDED

