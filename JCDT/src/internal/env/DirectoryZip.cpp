#include "stdafx.h"
#include <JCDT_Lib/internal/env/DirectoryZip.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>

namespace Jikes { // Open namespace Jikes block

				  //
				  // Upon successful termination of this function, IsValid() should yield true.
				  // Each CEN header would have been read so the magic number would get reset
				  // when the END header is again read.
				  //
	void DirectoryZip::ReadDirectory()
	{
		// Not a sourcepath (since we don't read java files from zip files)
		root_directory = new DirectorySymbol(nameEnv->dot_name_symbol, NULL, false);

		if (IsValid())
		{
			Skip(8); // u2 number_of_this_disk              = GetU2();
					 // u2 number_of_the_disk_with_the_star = GetU2();
					 // u2 start_of_the_central_directory   = GetU2();
					 // u2 total_number_of_entries_in_the_  = GetU2();
			u4 central_directory_size = GetU4();

#ifdef UNIX_FILE_SYSTEM
			u4 central_directory_offset = GetU4();
			Skip(2); // u2 comment_length                   = GetU2();
			int rc = fseek(zipfile, central_directory_offset, SEEK_SET);

			assert(rc == 0);

			delete[] zipbuffer;
			zipbuffer = new char[central_directory_size + END_SIZE];
			buffer_ptr = zipbuffer;
			SystemFread(buffer_ptr, sizeof(char),
				central_directory_size + END_SIZE,
				zipfile);
#elif defined(WIN32_FILE_SYSTEM)
			Skip(6); // u4 central_directory_offset         = GetU4();
					 // u2 comment_length                   = GetU2();
			buffer_ptr -= END_SIZE + central_directory_size;
#endif
			for (magic = GetU4(); magic == CEN_SIG; magic = GetU4())
				ProcessDirectoryEntry();
		}
	}
	DirectoryZip::DirectoryZip(INameEnvironment * nameEnv_, const char *zipfile_name_)
		:root_directory(nullptr)
	{
		nameEnv = (nameEnv_);
		ReadZipContent(zipfile_name_);
	}
	DirectoryZip::DirectoryZip(INameEnvironment *nameEnv_, const wchar_t *zipfile_name_)
		: root_directory(nullptr)
	{
		nameEnv = (nameEnv_);
		ReadZipContent(zipfile_name_);
	}

	DirectoryZip::~DirectoryZip()
	{
		delete root_directory;
		root_directory = NULL;
	}
	void DirectoryZip::Clear()
	{

		Zip::Clear();
		delete root_directory;
		root_directory = NULL;
	}

	 DirectorySymbol *DirectoryZip::ProcessSubdirectoryEntries(DirectorySymbol *directory_symbol, char *name, int name_length)
	{
		wchar_t *directory_name = new wchar_t[name_length];

		for (int start = 0, end; start < name_length; start = end + 1)
		{
			end = start;
			for (int i = 0; end < name_length && name[end] != U_SLASH; i++, end++)
				directory_name[i] = name[end];
			NameSymbol *name_symbol = nameEnv->FindOrInsertName(directory_name, end - start);
			DirectorySymbol *subdirectory_symbol = directory_symbol->FindDirectorySymbol(name_symbol);
			if (!subdirectory_symbol)
				subdirectory_symbol = directory_symbol->InsertDirectorySymbol(name_symbol, false);
			directory_symbol = subdirectory_symbol;
		}

		delete[] directory_name;

		return directory_symbol;
	}

	 NameSymbol *DirectoryZip::ProcessFilename(char *name, int name_length)
	 {
		 wchar_t *input_filename = new wchar_t[name_length];
		 for (int i = 0; i < name_length; i++)
			 input_filename[i] = name[i];
		 NameSymbol *name_symbol = nameEnv->FindOrInsertName(input_filename, name_length);

		 delete[] input_filename;

		 return name_symbol;
	 }



	  void DirectoryZip::ProcessDirectoryEntry()
	 {
		 Skip(8); // u2 version_made_by           = GetU2();
				  // u2 version_needed_to_extract = GetU2();
				  // u2 general_purpose_bits      = GetU2();
				  // u2 compression_method        = GetU2();
		 u2 last_mod_file_time = GetU2();
		 u2 last_mod_file_date = GetU2();
		 Skip(4); // u4 crc32                     = GetU4();
		 Skip(4); // u4 compressed_size           = GetU4();
		 u4 uncompressed_size = GetU4();
		 u2 file_name_length = GetU2();
		 u2 extra_field_length = GetU2();
		 u2 file_comment_length = GetU2();
		 Skip(8); // u2 disk_number_start         = GetU2();
				  // u2 internal_file_attributes  = GetU2();
				  // u4 external_file_attributes  = GetU4();
		 u4 relative_offset_of_local_header = GetU4();

		 u4 date_time = ((u4)last_mod_file_date) << 16 | last_mod_file_time;
		 char *name = buffer_ptr;

		 Skip(file_name_length + extra_field_length + file_comment_length);

		 //
		 // Note that we need to process all subdirectory entries
		 // that appear in the zip file, and not just the ones that
		 // contain java and class files. Recall that in java the
		 // dot notation is used in specifying a package. Therefore,
		 // in processing a qualified-name that represents a package,
		 // we need to recognize each name as a subpackage. E.g.,
		 // when processing "java.lang", we need to recognize "java"
		 // as a package before looking for "lang"...

		 // start at the "." directory.
		 DirectorySymbol *directory_symbol = root_directory;
		 // -1 to remove last '/'
		 if (name[file_name_length - 1] == U_SLASH)
			 ProcessSubdirectoryEntries(directory_symbol,
				 name,
				 file_name_length - 1);
		 else
		 {
			 bool java_file = (file_name_length >= FileBase::java_suffix_length &&
				 FileBase::IsJavaSuffix(&name[file_name_length - FileBase::java_suffix_length])),
				 class_file = (file_name_length >= FileBase::class_suffix_length &&
					 FileBase::IsClassSuffix(&name[file_name_length - FileBase::class_suffix_length]));

			 if (java_file || class_file)
			 {
				 int name_length = file_name_length - (java_file ? FileSymbol::java_suffix_length : FileSymbol::class_suffix_length);
				 int i;
				 for (i = name_length - 1; i >= 0 && name[i] != U_SLASH; i--)
					 ;
				 if (i > 0) // directory specified?
					 directory_symbol = ProcessSubdirectoryEntries(directory_symbol,
						 name, i);
				 NameSymbol *name_symbol = ProcessFilename(&name[i + 1],
					 name_length - (i + 1));

				 //
				 // Search for a file of that name in the directory.
				 // If one is not found, then insert ... Otherwise,
				 // either a class file of that name was previously
				 // processed and now we found a java file with the
				 // same name or vice-versa... In that case keep
				 // (or replace with) the file with the most recent
				 // date stamp.
				 //
				 FileSymbol *file_symbol = directory_symbol ->
					 FindFileSymbol(name_symbol);
				 if (!file_symbol)
				 {
					 file_symbol = directory_symbol->InsertFileSymbol(name_symbol);

					 file_symbol->directory_symbol = directory_symbol;
					 if (java_file)
						 file_symbol->SetJava();
					 else file_symbol->SetClassOnly();

					 file_symbol->uncompressed_size = uncompressed_size;
					 file_symbol->offset = relative_offset_of_local_header;
					 file_symbol->date_time = date_time;
				 }
				 else if (file_symbol->date_time < date_time)
				 {
					 if (java_file)
						 file_symbol->SetJava();
					 else file_symbol->SetClass();

					 file_symbol->uncompressed_size = uncompressed_size;
					 file_symbol->offset = relative_offset_of_local_header;
					 file_symbol->date_time = date_time;
				 }
			 }
		 }
	 }

} // Close namespace Jikes block



