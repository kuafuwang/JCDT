#include "JCDT_Lib/internal/env/platform.h"
#include  "JCDT_Lib/internal/util/FileUtil.h"
#include <JCDT_Lib/internal/util/CharOperation.h>

namespace Jikes { // Open namespace Jikes block



	/**
	* The Write() method on all WriteObject(s) makes sure that we do not
	* send too much data to the virtual function.
	*/
	size_t FileWriter::write(const unsigned char *data, size_t size)
	{
		size_t result = 0;

		if (size <= maxSize)
		{
			result = doWrite(data, size);
			maxSize -= size;
		}

		return result;
	}


#ifdef UNIX_FILE_SYSTEM

	// The following methods define UNIX specific methods for
	// reading files from the file system. WINDOWS method follow in
	// the subsequent section.


	//
	// When the ReadObject is created. read the whole file into a buffer
	// held by the object.
	//
	DefaultFileReader::DefaultFileReader(const char *fileName)
	{
		size = 0;
		buffer = NULL;

		struct stat status;
		Compiler::getInstance()->stat(fileName, &status);
		size = status.st_size;

		FILE *srcfile = SystemFopen(fileName, "rb");
		if (srcfile != NULL)
		{
			buffer = new char[size];
			size_t numread = SystemFread(
#ifdef HAVE_CONST_CAST
				const_cast<char*>(buffer)
#else
				(char *)buffer
#endif
				, sizeof(char), size, srcfile);
			assert(numread <= size); // FIXME: change to == when SystemFread uses "b"
			fclose(srcfile);
		}
	}


	/**
	* When the ReadObject is destroyed the release the memory buffer.
	*/
	DefaultFileReader::~DefaultFileReader()
	{
		delete[] buffer;
	}


	/**
	* Open a standard FILE pointer and get ready to write.
	*/
	DefaultFileWriter::DefaultFileWriter(const char *fileName, size_t maxSize)
		: Compiler::FileWriter(maxSize)
	{
		valid = false;
		file = SystemFopen(fileName, "wb");
		if (file == (FILE *)NULL)
			return;
		valid = true;
	}

	/**
	* Close the file when the write object is destroyed.
	*/
	DefaultFileWriter::~DefaultFileWriter()
	{
		if (valid)
			fclose(file);
	}

	int DefaultFileWriter::isValid() { return valid; }

	/**
	* Copy the data buffer to the file.
	*/
	size_t DefaultFileWriter::doWrite(const unsigned char *data, size_t size)
	{
		return fwrite(data, sizeof(u1), size, file);
	}


#elif defined(WIN32_FILE_SYSTEM) // ! UNIX_FILE_SYSTEM


	// Open a windows file and map the file onto processor memory.
	DefaultFileReader::DefaultFileReader(const char *fileName)
	{
		size = 0;
		buffer = NULL;
		auto _w_fileName = CharOperation::ConvertUtf8ToUnicode(fileName);
		srcfile = CreateFileW(_w_fileName.c_str() , GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (srcfile != INVALID_HANDLE_VALUE)
		{
			mapfile = CreateFileMapping(srcfile, NULL, PAGE_READONLY, 0, 0, NULL);
			if (mapfile != INVALID_HANDLE_VALUE)
			{
				buffer = (char *)MapViewOfFile(mapfile, FILE_MAP_READ, 0, 0, 0);
				size = (size_t)GetFileSize(srcfile, NULL);
			}
		}
	}


	// When the ReadObject is destroyed close all the associated files.
	// and unmap the memory.
	DefaultFileReader::~DefaultFileReader()
	{
		if (srcfile != INVALID_HANDLE_VALUE)
		{
			if (mapfile != INVALID_HANDLE_VALUE)
			{
				if (buffer)
				{
					UnmapViewOfFile((void *)buffer);
				}
				CloseHandle(mapfile);
			}
			CloseHandle(srcfile);
		}
	}


	// Create a windows file and map the file onto processor memory.
	DefaultFileWriter::DefaultFileWriter(const char *fileName, size_t maxSize)
		: FileWriter(maxSize)
	{
		auto _w_fileName =  CharOperation::ConvertUtf8ToUnicode(fileName);
		valid = false;
		dataWritten = 0;
		file = CreateFileW(_w_fileName.c_str() ,GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
			return;

		mapfile = CreateFileMapping(file, NULL, PAGE_READWRITE, 0, maxSize, NULL);
		if (mapfile == INVALID_HANDLE_VALUE)
			return;

		string_buffer = (u1 *)MapViewOfFile(mapfile, FILE_MAP_WRITE, 0, 0, maxSize);
		assert(string_buffer);
		valid = true;
	}

	// When the WriteObject is destroyed close all the associated files,
	// Thus writting the mory to the file system.
	DefaultFileWriter::~DefaultFileWriter()
	{
		if (file != INVALID_HANDLE_VALUE)
		{
			if (mapfile != INVALID_HANDLE_VALUE)
			{
				UnmapViewOfFile(string_buffer);
				CloseHandle(mapfile);
			}
			CloseHandle(file);
		}
	}

	int DefaultFileWriter::isValid()
	{
		return valid;
	}

	// Copy the input data to the mapped memory.
	size_t DefaultFileWriter::doWrite(const unsigned char* data, size_t size)
	{
		// This assumes that data never overlaps string_buffer.
		memcpy(&string_buffer[dataWritten], data, size * sizeof(u1));
		dataWritten += size;
		return size;
	}

#endif // WIN32_FILE_SYSTEM



} // Close namespace Jikes block



