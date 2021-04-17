#include "stdafx.h"
#include <JCDT_Lib/internal/env/ClassFileReader.h>
#include  <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/impl/FileBase.h>
#include <JCDT_Lib/internal/classfmt/class.h>

namespace Jikes { // Open namespace Jikes block


	class ClassFileReaderZip :public Zip
	{
	public:
		ClassFileReaderZip();
		~ClassFileReaderZip();
		ClassFile* read(wstring& path_in_zip, wstring& zip_paht_in_os);


	protected:


		std::set <std::wstring > pkgInfo;
		void ReadDirectory() override;


		void ProcessSubdirectoryEntries(char *, int);

		ClassFile* FindEntry(string& path_in_zip);

	};


	ClassFile* ClassFileReader::read(wstring& path_in_zip, wstring& zip_paht_in_os)
	{
		ClassFileReaderZip * zip = new ClassFileReaderZip();
		auto _ret = zip->read(path_in_zip, zip_paht_in_os);
		delete zip;
		return _ret;
	}


	// Upon successful termination of this function, IsValid() should yield true.
	// Each CEN header would have been read so the magic number would get reset
	// when the END header is again read.
	void ClassFileReaderZip::ReadDirectory()
	{
		// Not a sourcepath (since we don't read java files from zip files)
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
		}
	}

	

	ClassFileReaderZip::ClassFileReaderZip()
		:Zip(){
	}

	ClassFileReaderZip::~ClassFileReaderZip(){	
	}

	ClassFile* ClassFileReaderZip::read(wstring& path_in_zip, wstring& zip_paht_in_os){
		if(IsValid()){
			ReadZipContent(zip_paht_in_os.c_str());
			auto _utf_path = CharOperation::ConvertUnicodeToUtf8(path_in_zip);
			ReadDirectory();
			for (magic = GetU4(); magic == CEN_SIG; magic = GetU4()){
			   auto entry = FindEntry(_utf_path);
			   if (entry)
				   return entry;
			}
				
		}
		return nullptr;
	}

	
	void ClassFileReaderZip::ProcessSubdirectoryEntries(char *name, int name_length) 
	{
		if(name && name_length){
			wstring _compoundname = CharOperation::ConvertUtf8ToUnicode(string(name, name + name_length));
			boost::replace_all(_compoundname, L"/", L".");
			pkgInfo.insert(_compoundname);
		}
		
	}

	ClassFile* ClassFileReaderZip::FindEntry(string& path_in_zip)
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

		 // Note that we need to process all subdirectory entries
		 // that appear in the zip file, and not just the ones that
		 // contain java and class files. Recall that in java the
		 // dot notation is used in specifying a package. Therefore,
		 // in processing a qualified-name that represents a package,
		 // we need to recognize each name as a subpackage. E.g.,
		 // when processing "java.lang", we need to recognize "java"
		 // as a package before looking for "lang"...

		 // -1 to remove last '/'
		 if (name[file_name_length - 1] == U_SLASH)
			 return nullptr;

		bool	class_file = (file_name_length >= FileBase::class_suffix_length &&
				FileBase::IsClassSuffix(&name[file_name_length - FileBase::class_suffix_length]));

		if (class_file){
			string _fileNameString(name, name + file_name_length);
			if(path_in_zip == _fileNameString){
				Jikes::ClassFile* class_data = nullptr;
				ZipFile* zipfile = new ZipFile(this, uncompressed_size, relative_offset_of_local_header);
				auto buffer = zipfile->Buffer();
				if (buffer){
					class_data = new Jikes::ClassFile(buffer, uncompressed_size);
					if (!class_data->Valid()){
						delete class_data;
						class_data = nullptr;
					}
				}
				delete zipfile;
				return class_data;
			}
		}		
		return nullptr;	 
	 }
	//ClassFile* ClassFileReaderZip::ProcessDirectoryEntry()
	//{
	//	Skip(8); // u2 version_made_by           = GetU2();
	//			 // u2 version_needed_to_extract = GetU2();
	//			 // u2 general_purpose_bits      = GetU2();
	//			 // u2 compression_method        = GetU2();
	//	u2 last_mod_file_time = GetU2();
	//	u2 last_mod_file_date = GetU2();
	//	Skip(4); // u4 crc32                     = GetU4();
	//	Skip(4); // u4 compressed_size           = GetU4();
	//	u4 uncompressed_size = GetU4();
	//	u2 file_name_length = GetU2();
	//	u2 extra_field_length = GetU2();
	//	u2 file_comment_length = GetU2();
	//	Skip(8); // u2 disk_number_start         = GetU2();
	//			 // u2 internal_file_attributes  = GetU2();
	//			 // u4 external_file_attributes  = GetU4();
	//	u4 relative_offset_of_local_header = GetU4();

	//	u4 date_time = ((u4)last_mod_file_date) << 16 | last_mod_file_time;
	//	char *name = buffer_ptr;

	//	Skip(file_name_length + extra_field_length + file_comment_length);

	//	// Note that we need to process all subdirectory entries
	//	// that appear in the zip file, and not just the ones that
	//	// contain java and class files. Recall that in java the
	//	// dot notation is used in specifying a package. Therefore,
	//	// in processing a qualified-name that represents a package,
	//	// we need to recognize each name as a subpackage. E.g.,
	//	// when processing "java.lang", we need to recognize "java"
	//	// as a package before looking for "lang"...


	//	// -1 to remove last '/'
	//	string _fileNameString(name, name + file_name_length);
	//	if (name[file_name_length - 1] == U_SLASH)
	//		ProcessSubdirectoryEntries(name, file_name_length - 1);
	//	else
	//	{
	//		bool java_file = (file_name_length >= FileBase::java_suffix_length &&
	//			FileBase::IsJavaSuffix(&name[file_name_length - FileBase::java_suffix_length])),
	//			class_file = (file_name_length >= FileBase::class_suffix_length &&
	//				FileBase::IsClassSuffix(&name[file_name_length - FileBase::class_suffix_length]));

	//		if (java_file || class_file)
	//		{
	//			int name_length = file_name_length - (java_file ? FileBase::java_suffix_length : FileBase::class_suffix_length);
	//			int i;
	//			for (i = name_length - 1; i >= 0 && name[i] != U_SLASH; i--) {}

	//			if (i > 0) // directory specified?
	//			{
	//				ProcessSubdirectoryEntries(name, i);
	//			}


	//			auto  __file_name = ProcessFilename(&name[i + 1], name_length - (i + 1));
	//			if (!java_file) {

	//			}
	//			else {

	//			}

	//			// file_symbol->uncompressed_size = uncompressed_size;
	//			// file_symbol->offset = relative_offset_of_local_header;
	//			// file_symbol->date_time = date_time; 
	//		}
	//	}
	//}
} // Close namespace Jikes block



