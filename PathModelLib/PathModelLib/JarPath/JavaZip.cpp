#include "stdafx.h"
#include "JavaZip.h"

#include  <boost/algorithm/string.hpp>
#include "JarPackageRoot.h"
#include <JCDT_Lib/internal/impl/FileBase.h>
#include "JarPackage.h"

namespace Jikes { // Open namespace Jikes block


namespace PathModel {

	// Upon successful termination of this function, IsValid() should yield true.
	// Each CEN header would have been read so the magic number would get reset
	// when the END header is again read.
	void JavaZip::ReadDirectory()
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
			for (magic = GetU4(); magic == CEN_SIG; magic = GetU4())
				ProcessDirectoryEntry();
		}
	}
	JavaZip::JavaZip(JarPackageRoot* root)
		:Zip(), pkgFrgRoot(root){
	}

	JavaZip::~JavaZip(){	
	}
	
	wstring ProcessFilename(char *name, int name_length)
	{
		wchar_t *input_filename = new wchar_t[name_length];
		for (int i = 0; i < name_length; i++)
			input_filename[i] = name[i];
		wstring ret(input_filename, input_filename+ name_length);
		
		delete[] input_filename;

		return ret;
	}
	JarPackage *JavaZip::ProcessSubdirectoryEntries(char *name, int name_length) const
	{
		wchar_t *directory_name = new wchar_t[name_length];
		vector<wstring> names;
		for (int start = 0, end; start < name_length; start = end + 1)
		{
			end = start;
			for (int i = 0; end < name_length && name[end] != U_SLASH; i++, end++)
				directory_name[i] = name[end];

			names.push_back( 
				wstring(directory_name, directory_name + end - start)
				);
		}

		delete[] directory_name;
		auto _compoundname = boost::join(names, L".");
		return pkgFrgRoot->FindOrInsertPackageFragment(_compoundname);
	}

	  void JavaZip::ProcessDirectoryEntry()
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

		
		 // -1 to remove last '/'
		 if (name[file_name_length - 1] == U_SLASH)
			 ProcessSubdirectoryEntries(name,file_name_length - 1);
		 else
		 {
			 bool java_file = (file_name_length >= FileBase::java_suffix_length &&
				 FileBase::IsJavaSuffix(&name[file_name_length - FileBase::java_suffix_length])),
				 class_file = (file_name_length >= FileBase::class_suffix_length &&
					 FileBase::IsClassSuffix(&name[file_name_length - FileBase::class_suffix_length]));

			 if (java_file || class_file)
			 {
				 int name_length = file_name_length - (java_file ? FileBase::java_suffix_length : FileBase::class_suffix_length);
				 int i;
				 for (i = name_length - 1; i >= 0 && name[i] != U_SLASH; i--){}
				 JarPackage *directory_symbol;
				 if (i > 0) // directory specified?
				 {
					 directory_symbol = ProcessSubdirectoryEntries(name, i);
				 }
				 else
				 {
					 directory_symbol =  pkgFrgRoot->FindOrInsertPackageFragment(JarPackage::DEFAULT_PACKAGE_NAME);
				 }

				 auto  __file_name = ProcessFilename(&name[i + 1],name_length - (i + 1));

				 
				 if (!java_file) {
					 PathModel::ClassFile* file_symbol = new ClassFile(__file_name, pkgFrgRoot, directory_symbol,
						 uncompressed_size, relative_offset_of_local_header);
					 directory_symbol->children.insert(std::make_pair(__file_name, file_symbol));
					
				 }
				 else {
					// auto temp = (BaseJavaFile*)directory_symbol->FindOrInsertCompilationUnit(__file_name);
					// file_symbol = dynamic_cast<BaseJavaFile*>(temp);
				 }

				
			 }
		 }
	 }
}// Close namespace JavaModel block
	
} // Close namespace Jikes block



