
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/problem/ErrorString.h"
#include "JCDT_Lib/internal/util/case.h"
#include "JCDT_Lib/internal/impl/option.h"
#include <boost/algorithm/string.hpp>
#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/impl/JikesClassPaths.h"

#include  <JCDT_Lib/internal/lookup/FileSymbol.h>
#include  <JCDT_Lib/internal/lookup/PathSymbol.h>
#include  <JCDT_Lib/internal/lookup/SystemTable.h>
#include  <JCDT_Lib/internal/lookup/DirectoryEntry.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/env/DirectoryZip.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <boost/filesystem/operations.hpp>

namespace Jikes { // Open namespace Jikes block

	
INameEnvironment::INameEnvironment(Option* option_, JikesClassPaths& paths)
   : option(option_)
    , dot_classpath_index(0)
    , system_table(NULL)
    , int_pool(&bad_value)   
    // storage for all literals seen in source
    , long_pool(&bad_value)
    , float_pool(&bad_value)
    , double_pool(&bad_value)
    , Utf8_pool(&bad_value)
{
	ProcessGlobals();
	
	ProcessPath(paths);
	
}

INameEnvironment::~INameEnvironment()
{
    unsigned i;
    for (i = 0; i < bad_zip_filenames.Length(); i++)
        delete [] bad_zip_filenames[i];
   
   
    for (i = 0; i < system_directories.Length(); i++)
        delete system_directories[i];


    delete system_table;

}



//
// When searching for a subdirectory in a zipped file, it must already be
// present in the hierarchy.
//
DirectorySymbol* INameEnvironment::FindSubdirectory(PathSymbol* path_symbol,
                                          const wchar_t* name, int name_length)
{
    wchar_t* directory_name = new wchar_t[name_length + 1];

    DirectorySymbol* directory_symbol = path_symbol -> RootDirectory();
    for (int start = 0, end;
         directory_symbol && start < name_length;
         start = end + 1)
    {
        end = start;
        for (int i = 0; end < name_length && name[end] != U_SLASH; i++, end++)
             directory_name[i] = name[end];
        NameSymbol* name_symbol = FindOrInsertName(directory_name,
                                                   end - start);
        directory_symbol =
            directory_symbol -> FindDirectorySymbol(name_symbol);
    }

    delete [] directory_name;
    return directory_symbol;
}
FileSymbol* INameEnvironment::GetVirtualFile()
{

	//
	// Some name, any name !!! We use dot_name_symbol as a bad file name
	// because no file can be named ".".
	//
	FileSymbol* file_symbol = classpath[dot_classpath_index] ->
		RootDirectory()->InsertFileSymbol(dot_name_symbol);

	file_symbol->directory_symbol = classpath[dot_classpath_index] ->
		RootDirectory();

	file_symbol->MarkVirtualFile();

	return file_symbol;
}

//
// When searching for a directory in the system, if it is not already present
// in the hierarchy insert it and attempt to read it from the system...
//
#ifdef UNIX_FILE_SYSTEM
DirectorySymbol* INameEnvironment::ProcessSubdirectories(const wchar_t* source_name,
                                                int source_name_length,
                                                bool source_dir)
{
    int name_length = (source_name_length < 0 ? 0 : source_name_length);
    char* input_name = new char[name_length + 1];
    for (int i = 0; i < name_length; i++)
        input_name[i] = source_name[i];
    input_name[name_length] = U_NULL;

    DirectorySymbol* directory_symbol = NULL;
    struct stat status;
    if (SystemStat(input_name, &status) == 0 &&
        (status.st_mode & JIKES_STAT_S_IFDIR))
    {
        directory_symbol = system_table ->
            FindDirectorySymbol(status.st_dev, status.st_ino);
    }

    if (! directory_symbol)
    {
        if (input_name[0] == U_SLASH) // file name starts with '/'
        {
            directory_symbol =
                new DirectorySymbol(FindOrInsertName(source_name, name_length),
                                    classpath[dot_classpath_index],
                                    source_dir);
            directory_symbol -> ReadDirectory();
            system_directories.Next() = directory_symbol;
            system_table -> InsertDirectorySymbol(status.st_dev,
                                                  status.st_ino,
                                                  directory_symbol);
        }
        else
        {
            wchar_t* name = new wchar_t[name_length + 1];
            for (int i = 0; i < name_length; i++)
                name[i] = source_name[i];
            name[name_length] = U_NULL;

            // Start at the dot directory.
            directory_symbol =
                classpath[dot_classpath_index] -> RootDirectory();

            wchar_t* directory_name = new wchar_t[name_length];
            int end = 0;
            for (int start = end; start < name_length; start = end)
            {
                int length;
                for (length = 0;
                     end < name_length && name[end] != U_SLASH;
                     length++, end++)
                {
                    directory_name[length] = name[end];
                }

                if (length != 1 || directory_name[0] != U_DOT)
                {
                    // Not the current directory.
                    if (length == 2 && directory_name[0] == U_DOT &&
                        directory_name[1] == U_DOT)
                    {
                        // keep the current directory
						if (directory_symbol->Identity() == dot_name_symbol ||
							directory_symbol->Identity() == dot_dot_name_symbol)
						{
							DirectorySymbol* subdirectory_symbol =
								directory_symbol->FindDirectorySymbol(dot_dot_name_symbol);
							if (!subdirectory_symbol)
								subdirectory_symbol =
								directory_symbol->InsertDirectorySymbol(dot_dot_name_symbol,
									source_dir);
							directory_symbol = subdirectory_symbol;
						}
						else
							//directory_symbol =    directory_symbol -> owner -> DirectoryCast();
							directory_symbol = DirectorySymbol::DirectoryCast(directory_symbol->owner);
                    }
                    else
                    {
                        NameSymbol* name_symbol =
                            FindOrInsertName(directory_name, length);
                        DirectorySymbol* subdirectory_symbol =
                            directory_symbol -> FindDirectorySymbol(name_symbol);
                        if (! subdirectory_symbol)
                            subdirectory_symbol =
                                directory_symbol -> InsertDirectorySymbol(name_symbol,
                                                                          source_dir);
                        directory_symbol = subdirectory_symbol;
                    }
                }

                for (end++;
                     end < name_length && name[end] == U_SLASH;
                     end++); // skip all extra '/'
            }

            //
            // Insert the new directory into the system table to avoid
            // duplicates, in case the same directory is specified with
            // a different name.
            //
            if (directory_symbol !=
                classpath[dot_classpath_index] -> RootDirectory())
            {
                // Not the dot directory.
                system_table -> InsertDirectorySymbol(status.st_dev,
                                                      status.st_ino,
                                                      directory_symbol);
                directory_symbol -> ReadDirectory();
            }

            delete [] directory_name;
            delete [] name;
        }
    }

    delete [] input_name;
    return directory_symbol;
}
#elif defined(WIN32_FILE_SYSTEM)
DirectorySymbol* INameEnvironment::ProcessSubdirectories(const wchar_t* source_name,
                                                int source_name_length,
                                                bool source_dir)
{
    DirectorySymbol* directory_symbol =
        classpath[dot_classpath_index] -> RootDirectory();

    int name_length = (source_name_length < 0 ? 0 : source_name_length);
    wchar_t* name = new wchar_t[name_length + 1];
    char* input_name = new char[name_length + 1];
    for (int i = 0; i < name_length; i++)
        input_name[i] = name[i] = source_name[i];
    input_name[name_length] = name[name_length] = U_NULL;

    if (name_length >= 2 && Case::IsAsciiAlpha(input_name[0]) &&
        input_name[1] == U_COLON) // a disk was specified
    {
        char disk = input_name[0];
        option->SaveCurrentDirectoryOnDisk(disk);
        if (SetCurrentDirectoryA(input_name))
        {
            // First, get the right size.
            DWORD directory_length = GetCurrentDirectoryA(0, input_name);
            char* full_directory_name = new char[directory_length + 1];
            DWORD length = GetCurrentDirectoryA(directory_length, full_directory_name);
            if (length <= directory_length)
            {
                // Turn '\' to '/'.
                for (char* ptr = full_directory_name; *ptr; ptr++)
                    *ptr = (*ptr != U_BACKSLASH ? *ptr : (char) U_SLASH);

				/*  char* current_directory = option->GetMainCurrentDirectory();
				  int prefix_length = strlen(current_directory);
				  int start = (prefix_length <= (int) length &&
							   Case::StringSegmentEqual(current_directory,
														full_directory_name,
														prefix_length) &&
							   (full_directory_name[prefix_length] == U_SLASH ||
								full_directory_name[prefix_length] == U_NULL)
							   ? prefix_length + 1
							   : 0);*/
				int start = 0;
                if (start > (int) length)
                    name_length = 0;
                else if (start <= (int) length) // note that we can assert that (start != length)
                {
                    delete [] name;
                    name_length = length - start;
                    name = new wchar_t[name_length + 1];
                    for (int k = 0, i = start; i < (int) length; i++, k++)
                        name[k] = full_directory_name[i];
                    name[name_length] = U_NULL;
                }
            }

            delete [] full_directory_name;
        }

        // Reset the current directory on this disk.
        option->ResetCurrentDirectoryOnDisk(disk);
        option->SetMainCurrentDirectory(); // Reset the real current directory.
    }

    int end;
    if (name_length > 2 && Case::IsAsciiAlpha(name[0]) &&
        name[1] == U_COLON && name[2] == U_SLASH)
    {
        end = 3;
    }
    else
    {
        for (end = 0;
             end < name_length && name[end] == U_SLASH;
             end++); // keep all extra leading '/'
    }

    wchar_t* directory_name = new wchar_t[name_length];
    int length;
    if (end > 0)
    {
        for (length = 0; length < end; length++)
            directory_name[length] = name[length];
        NameSymbol* name_symbol = FindOrInsertName(directory_name, length);
        DirectorySymbol* subdirectory_symbol =
            directory_symbol -> FindDirectorySymbol(name_symbol);
        if (! subdirectory_symbol)
            subdirectory_symbol =
                directory_symbol -> InsertDirectorySymbol(name_symbol,
                                                          source_dir);
        directory_symbol = subdirectory_symbol;
    }

    for (int start = end; start < name_length; start = end)
    {
        for (length = 0;
             end < name_length && name[end] != U_SLASH;
             length++, end++)
        {
            directory_name[length] = name[end];
        }

        if (length != 1 || directory_name[0] != U_DOT)
        {
            // Not the current directory.
            if (length == 2 && directory_name[0] == U_DOT &&
                directory_name[1] == U_DOT)
            {
                // Keep the current directory.
                if (directory_symbol -> Identity() == dot_name_symbol ||
                    directory_symbol -> Identity() == dot_dot_name_symbol)
                {
                    DirectorySymbol* subdirectory_symbol =
                        directory_symbol -> FindDirectorySymbol(dot_dot_name_symbol);
                    if (! subdirectory_symbol)
                        subdirectory_symbol =
                            directory_symbol -> InsertDirectorySymbol(dot_dot_name_symbol,
                                                                      source_dir);
                    directory_symbol = subdirectory_symbol;
                }
                else 
					//directory_symbol = directory_symbol -> owner -> DirectoryCast();
					directory_symbol = DirectorySymbol::DirectoryCast(directory_symbol->owner);
            }
            else
            {
                NameSymbol* name_symbol = FindOrInsertName(directory_name,
                                                           length);
                DirectorySymbol* subdirectory_symbol =
                    directory_symbol -> FindDirectorySymbol(name_symbol);
                if (! subdirectory_symbol)
                    subdirectory_symbol =
                        directory_symbol -> InsertDirectorySymbol(name_symbol,
                                                                  source_dir);
                directory_symbol = subdirectory_symbol;
            }
        }

        for (end++;
             end < name_length && name[end] == U_SLASH;
             end++); // skip all extra '/'
    }

    directory_symbol -> ReadDirectory();

    delete [] directory_name;
    delete [] name;
    delete [] input_name;
    return directory_symbol;
}
#endif // WIN32_FILE_SYSTEM


void INameEnvironment::ProcessNewInputFiles(SymbolSet& file_set, set<wstring>& inserting_files)
{
    bad_input_filenames.clear();
	unreadable_input_filenames.clear();
    // File must be of the form xxx.java where xxx is a
    // character string consisting of at least one character.
    //
	auto it = inserting_files.begin();
	auto end = inserting_files.end();
		for (; it != end; ++it) {
		 wstring file_name = *it;
		 bool bad = false;
		 if(file_name.size() < FileSymbol::java_suffix_length){
			 bad = true;
		 }
		 else{
			 auto ext = file_name.substr(file_name.size() - FileSymbol::java_suffix_length);
			 boost::to_upper(ext);
			 if(ext != L".JAVA"){
				 bad = true;
			 } 
		 }

		 if (bad) {	 
			 bad_input_filenames.push_back(file_name);
		 }
		 else
		 {
			 FileSymbol* file_symbol =
				 FindOrInsertJavaInputFile(file_name.c_str(),
					 file_name.size() - FileSymbol::java_suffix_length);

			 if (!file_symbol)
				 unreadable_input_filenames.push_back(file_name);
			 else
			 {
				 file_set.AddElement(file_symbol);
			 }
		 }
	 }
    
}

	void INameEnvironment::ProcessNewInputFiles(SymbolSet& file_set,   set<FileSymbol*>& inserting_files)
	{
		bad_input_filenames.clear();
		unreadable_input_filenames.clear();
		auto _end = inserting_files.end();
		auto _it = inserting_files.begin();
		for (; _it != _end;  ) {
			auto file_symbol = *_it;
			if(file_symbol->IsJava()){
				file_symbol->Name();
				
				bool bInsert = InsertJavaInputFile(file_symbol->Name(),
					file_symbol->NameLength() - FileSymbol::java_suffix_length, file_symbol);
				if (!bInsert){
					unreadable_input_filenames.push_back(file_symbol->Name());
					++_it;
				}	
				else{
					_it =inserting_files.erase(_it);
					file_set.AddElement(file_symbol);
				}
			}
		}
	}

	FileSymbol* INameEnvironment::FindOrInsertJavaInputFile(DirectorySymbol* directory_symbol,
                                               NameSymbol* file_name_symbol)
{
    FileSymbol* file_symbol = NULL;

    int length = file_name_symbol -> Utf8NameLength() +
        FileSymbol::java_suffix_length;
    char* java_name = new char[length + 1]; // +1 for \0
    strcpy(java_name, file_name_symbol -> Utf8Name());
    strcat(java_name, FileSymbol::java_suffix);

    DirectoryEntry* entry = directory_symbol -> FindEntry(java_name, length);
    if (entry)
    {
        file_symbol = directory_symbol -> FindFileSymbol(file_name_symbol);

        if (! file_symbol)
        {
            file_symbol =
                directory_symbol -> InsertFileSymbol(file_name_symbol);
            file_symbol -> directory_symbol = directory_symbol;
            file_symbol -> SetJava();
        }

        file_symbol -> mtime = entry -> Mtime();
    }

    delete [] java_name;
    return file_symbol;
}

	bool INameEnvironment::InsertJavaInputFile(DirectorySymbol* directory_symbol,
		FileSymbol* to_insert_file_symbol, NameSymbol* file_name_symbol)
	{
		int length = file_name_symbol->Utf8NameLength() +
			FileSymbol::java_suffix_length;
		char* java_name = new char[length + 1]; // +1 for \0
		strcpy(java_name, file_name_symbol->Utf8Name());
		strcat(java_name, FileSymbol::java_suffix);

		DirectoryEntry* entry = directory_symbol->FindEntry(java_name, length);
		delete[] java_name;
		if (entry){			
			directory_symbol->InsertFileSymbol(to_insert_file_symbol);
			to_insert_file_symbol->directory_symbol = directory_symbol;
			to_insert_file_symbol->SetJava();

			to_insert_file_symbol->mtime = entry->Mtime();
			return true;
		}
		else
		{
			return false;
		}
	}

	FileSymbol* INameEnvironment::FindOrInsertJavaInputFile(const wchar_t* name, int name_length)
{
    FileSymbol* file_symbol = NULL;

    //
    // The name has been preprocessed so that if it contains any
    // slashes it is a forward slash. In the loop below we look
    // for the occurrence of the first slash (if any) that separates
    // the file name from its directory name.
    //
    DirectorySymbol* directory_symbol;
    NameSymbol* file_name_symbol;
#ifdef UNIX_FILE_SYSTEM
    int len;
    for (len = name_length - 1; len >= 0 && name[len] != U_SLASH; len--)
        ;
    directory_symbol = ProcessSubdirectories(name, len, true);
    file_name_symbol = FindOrInsertName(&name[len + 1],
                                        name_length - (len + 1));
#elif defined(WIN32_FILE_SYSTEM)
    int len;
    for (len = name_length - 1;
         len >= 0 && name[len] != U_SLASH && name[len] != U_COLON;
         len--);

    directory_symbol = ProcessSubdirectories(name,
                                             (name[len] == U_COLON ? len + 1
                                              : len),
                                             true);
    file_name_symbol = FindOrInsertName(&name[len + 1],
                                        name_length - (len + 1));
#endif // WIN32_FILE_SYSTEM

    for (unsigned i = 1; i < classpath.Length(); i++)
    {
        if (i == dot_classpath_index) // the current directory (.).
        {
            file_symbol = FindOrInsertJavaInputFile(directory_symbol,
                                                    file_name_symbol);
            if (file_symbol)
                break;
        }
        else if (classpath[i] -> IsZip())
        {
            DirectorySymbol* directory_symbol = FindSubdirectory(classpath[i],
                                                                 name, len);
            if (directory_symbol)
            {
                file_symbol =
                    directory_symbol -> FindFileSymbol(file_name_symbol);
                if (file_symbol && file_symbol -> IsJava())
                     break;
                else file_symbol = NULL;
            }
        }
    }

    //
    // If the file was found, return it; otherwise, in case the (.) directory
    // was not specified in the classpath, search for the file in it...
    //
    return file_symbol ? file_symbol
        : FindOrInsertJavaInputFile(directory_symbol, file_name_symbol);
}

	bool INameEnvironment::InsertJavaInputFile(const wchar_t* name, int name_length, FileSymbol* file_symbol)
	{
		// The name has been preprocessed so that if it contains any
		// slashes it is a forward slash. In the loop below we look
		// for the occurrence of the first slash (if any) that separates
		// the file name from its directory name.
		//
		DirectorySymbol* directory_symbol;
	
#ifdef UNIX_FILE_SYSTEM
		int len;
		for (len = name_length - 1; len >= 0 && name[len] != U_SLASH; len--)
		{};
		directory_symbol = ProcessSubdirectories(name, len, true);
		
#elif defined(WIN32_FILE_SYSTEM)
		int len;
		for (len = name_length - 1;
			len >= 0 && name[len] != U_SLASH && name[len] != U_COLON;
			len--) {};

		directory_symbol = ProcessSubdirectories(name,
			(name[len] == U_COLON ? len + 1: len),true);
	
#endif // WIN32_FILE_SYSTEM
		auto file_name_symbol = FindOrInsertName(&name[len + 1],
			name_length - (len + 1));
		return InsertJavaInputFile(directory_symbol,
			file_symbol, file_name_symbol);
	}


	DirectoryEntry* INameEnvironment::FindInputFile(FileSymbol* file_symbol)
{
	int length
		= file_symbol->Utf8NameLength() + FileSymbol::java_suffix_length;

	char* java_name = new char[length + 1]; // +1 for '\0'
	strcpy(java_name, file_symbol->Utf8Name());
	strcat(java_name, FileSymbol::java_suffix);
	DirectoryEntry* java_entry
		= file_symbol->directory_symbol->FindEntry(java_name, length);

	delete[] java_name;
	return java_entry;
}



void INameEnvironment::RereadDirectory(DirectorySymbol* directory_symbol)
{
	directory_symbol->ResetDirectory();

	for (unsigned i = 0; i < directory_symbol->subdirectories.Length(); i++)
		RereadDirectory(directory_symbol->subdirectories[i]);
}


void INameEnvironment::RereadDirectories()
{
	for (unsigned i = (dot_classpath_index == 0 ? 0 : 1);
	i < classpath.Length(); i++)
	{
		PathSymbol* path_symbol = classpath[i];
		if (!path_symbol->IsZip())
			RereadDirectory(path_symbol->RootDirectory());
	}
}



//
// Turn a C string literal into a NameSymbol. Because it works even on
// non-ASCII systems (where 'a' != U_a), it is slightly inefficient. Hence it
// is private and only called from caching accessor methods.
//
NameSymbol* INameEnvironment::FindOrInsertSystemName(const char* name)
{
	int len = strlen(name);
	wchar_t* wname = new wchar_t[len + 1];
	for (int i = 0; i < len; i++)
	{
		// Only list the characters we expect in system names.
		switch (name[i])
		{
		case 'a': wname[i] = U_a; break;
		case 'b': wname[i] = U_b; break;
		case 'c': wname[i] = U_c; break;
		case 'd': wname[i] = U_d; break;
		case 'e': wname[i] = U_e; break;
		case 'f': wname[i] = U_f; break;
		case 'g': wname[i] = U_g; break;
		case 'h': wname[i] = U_h; break;
		case 'i': wname[i] = U_i; break;
		case 'j': wname[i] = U_j; break;
		case 'k': wname[i] = U_k; break;
		case 'l': wname[i] = U_l; break;
		case 'm': wname[i] = U_m; break;
		case 'n': wname[i] = U_n; break;
		case 'o': wname[i] = U_o; break;
		case 'p': wname[i] = U_p; break;
		case 'q': wname[i] = U_q; break;
		case 'r': wname[i] = U_r; break;
		case 's': wname[i] = U_s; break;
		case 't': wname[i] = U_t; break;
		case 'u': wname[i] = U_u; break;
		case 'v': wname[i] = U_v; break;
		case 'w': wname[i] = U_w; break;
		case 'x': wname[i] = U_x; break;
		case 'y': wname[i] = U_y; break;
		case 'z': wname[i] = U_z; break;
		case 'A': wname[i] = U_A; break;
		case 'B': wname[i] = U_B; break;
		case 'C': wname[i] = U_C; break;
		case 'D': wname[i] = U_D; break;
		case 'E': wname[i] = U_E; break;
		case 'F': wname[i] = U_F; break;
		case 'G': wname[i] = U_G; break;
		case 'H': wname[i] = U_H; break;
		case 'I': wname[i] = U_I; break;
		case 'J': wname[i] = U_J; break;
		case 'K': wname[i] = U_K; break;
		case 'L': wname[i] = U_L; break;
		case 'M': wname[i] = U_M; break;
		case 'N': wname[i] = U_N; break;
		case 'O': wname[i] = U_O; break;
		case 'P': wname[i] = U_P; break;
		case 'Q': wname[i] = U_Q; break;
		case 'R': wname[i] = U_R; break;
		case 'S': wname[i] = U_S; break;
		case 'T': wname[i] = U_T; break;
		case 'U': wname[i] = U_U; break;
		case 'V': wname[i] = U_V; break;
		case 'W': wname[i] = U_W; break;
		case 'X': wname[i] = U_X; break;
		case 'Y': wname[i] = U_Y; break;
		case 'Z': wname[i] = U_Z; break;
		case '0': wname[i] = U_0; break;
		case '1': wname[i] = U_1; break;
		case '2': wname[i] = U_2; break;
		case '3': wname[i] = U_3; break;
		case '4': wname[i] = U_4; break;
		case '5': wname[i] = U_5; break;
		case '6': wname[i] = U_6; break;
		case '7': wname[i] = U_7; break;
		case '8': wname[i] = U_8; break;
		case '9': wname[i] = U_9; break;
		case '_': wname[i] = U_UN; break;
		case '$': wname[i] = U_DS; break;
		case '(': wname[i] = U_LP; break;
		case ')': wname[i] = U_RP; break;
		case ';': wname[i] = U_SC; break;
		case '<': wname[i] = U_LT; break;
		case '>': wname[i] = U_GT; break;
		case '/': wname[i] = U_SL; break;
		case '[': wname[i] = U_LB; break;
		case ']': wname[i] = U_RB; break;
		case '-': wname[i] = U_MI; break;
		case '.': wname[i] = U_DO; break;
		case '?': wname[i] = U_QU; break;
		default: assert(false && "bad character in system name");
		}
	}
	wname[len] = U_NULL;
	NameSymbol* name_symbol = name_table.FindOrInsertName(wname, len);
	delete[] wname;
	if (!name_symbol->Utf8_literal)
		name_symbol->Utf8_literal =
		ConvertUnicodeToUtf8(name_symbol->Name());
	return name_symbol;
}



void INameEnvironment::FindPathsToDirectory(PackageSymbol* package)
{
	if (package->directory.Length() == 0)
	{
		PackageSymbol* owner_package = package->owner;
		if (owner_package) // package is a sub package?
		{
			for (unsigned i = 0; i < owner_package->directory.Length(); i++)
			{
				DirectorySymbol* owner_directory_symbol =
					owner_package->directory[i];
				DirectorySymbol* subdirectory_symbol =
					owner_directory_symbol->FindDirectorySymbol(package->Identity());
				if (!owner_directory_symbol->IsZip())
				{
					if (!subdirectory_symbol)
					{
						int length =
							owner_directory_symbol->DirectoryNameLength() +
							package->Utf8NameLength() + 1; // +1 for '/'
						char* directory_name = new char[length + 1];

						strcpy(directory_name,
							owner_directory_symbol->DirectoryName());
						if (owner_directory_symbol->DirectoryName()[owner_directory_symbol->DirectoryNameLength() - 1] != U_SLASH)
							strcat(directory_name, StringConstant::U8S_SL);
						strcat(directory_name, package->Utf8Name());

						if (SystemIsDirectory(directory_name))
							subdirectory_symbol = owner_directory_symbol ->
							InsertDirectorySymbol(package->Identity(),
								owner_directory_symbol->IsSourceDirectory());

						delete[] directory_name;
					}

					if (subdirectory_symbol)
						subdirectory_symbol->ReadDirectory();
				}

				if (subdirectory_symbol)
					package->directory.Next() = subdirectory_symbol;
			}
		}
		else
		{
			//
			// Recall that since classpath[0] contains the default directory,
			// we always start searching at location 1.
			//
			for (unsigned k = 1; k < classpath.Length(); k++)
			{
				PathSymbol* path_symbol = classpath[k];
				DirectorySymbol* directory_symbol =
					path_symbol->RootDirectory()->FindDirectorySymbol(package->Identity());
				if (!path_symbol->IsZip())
				{
					if (!directory_symbol)
					{
						int length = path_symbol->Utf8NameLength() +
							package->Utf8NameLength() + 1; // +1 for '/'
						char* directory_name = new char[length + 1];
						strcpy(directory_name, path_symbol->Utf8Name());
						char tail = path_symbol->Utf8Name()[path_symbol->Utf8NameLength() - 1];
						if (tail != U_SLASH && tail != U_BACKSLASH)
							strcat(directory_name, StringConstant::U8S_SL);
						strcat(directory_name, package->Utf8Name());

						if (SystemIsDirectory(directory_name))
							directory_symbol = path_symbol->RootDirectory() ->
							InsertDirectorySymbol(package->Identity(),
								path_symbol->RootDirectory()->IsSourceDirectory());
						delete[] directory_name;
					}

					if (directory_symbol)
						directory_symbol->ReadDirectory();
				}

				if (directory_symbol)
					package->directory.Next() = directory_symbol;
			}
		}
	}
}


void INameEnvironment::ProcessGlobals()
{
	// Some names are conditional on 1.5 VMs, which expanded the set of legal
	// VM names to include non-Java identifiers.
	access_name_symbol =
		FindOrInsertSystemName(option->source < JikesOption::SDK1_5
			? "access$" : "-");
	array_name_symbol = FindOrInsertSystemName("array");
	assert_name_symbol = FindOrInsertSystemName("assert");
	block_init_name_symbol = FindOrInsertSystemName("this");
	class_name_symbol = FindOrInsertSystemName("class");
	clinit_name_symbol = FindOrInsertSystemName("<clinit>");
	clone_name_symbol = FindOrInsertSystemName("clone");
	dot_name_symbol = FindOrInsertSystemName(".");
	dot_dot_name_symbol = FindOrInsertSystemName("..");
	Enum_name_symbol = FindOrInsertSystemName("Enum");
	equals_name_symbol = FindOrInsertSystemName("equals");
	false_name_symbol = FindOrInsertSystemName("false");
	hashCode_name_symbol = FindOrInsertSystemName("hashCode");
	init_name_symbol = FindOrInsertSystemName("<init>");
	length_name_symbol = FindOrInsertSystemName("length");
	null_name_symbol = FindOrInsertSystemName("null");
	Object_name_symbol = FindOrInsertSystemName("Object");
	package_info_name_symbol = FindOrInsertSystemName("package-info");
	question_name_symbol = FindOrInsertSystemName("??");
	serialPersistentFields_name_symbol =
		FindOrInsertSystemName("serialPersistentFields");
	serialVersionUID_name_symbol = FindOrInsertSystemName("serialVersionUID");
	this_name_symbol = FindOrInsertSystemName("this");
	true_name_symbol = FindOrInsertSystemName("true");
	val_name_symbol =
		FindOrInsertSystemName(option->source < JikesOption::SDK1_5
			? "val$" : "-");

	ConstantValue_literal = Utf8_pool.FindOrInsert(U8S_ConstantValue,
		strlen(U8S_ConstantValue));
	Exceptions_literal = Utf8_pool.FindOrInsert(U8S_Exceptions,
		strlen(U8S_Exceptions));
	InnerClasses_literal = Utf8_pool.FindOrInsert(U8S_InnerClasses,
		strlen(U8S_InnerClasses));
	Synthetic_literal = Utf8_pool.FindOrInsert(U8S_Synthetic,
		strlen(U8S_Synthetic));
	Deprecated_literal = Utf8_pool.FindOrInsert(U8S_Deprecated,
		strlen(U8S_Deprecated));
	LineNumberTable_literal =
		Utf8_pool.FindOrInsert(U8S_LineNumberTable,
			strlen(U8S_LineNumberTable));
	LocalVariableTable_literal =
		Utf8_pool.FindOrInsert(U8S_LocalVariableTable,
			strlen(U8S_LocalVariableTable));
	Code_literal = Utf8_pool.FindOrInsert(U8S_Code, strlen(U8S_Code));
	SourceFile_literal = Utf8_pool.FindOrInsert(U8S_SourceFile,
		strlen(U8S_SourceFile));
	EnclosingMethod_literal =
		Utf8_pool.FindOrInsert(U8S_EnclosingMethod,
			strlen(U8S_EnclosingMethod));
}


 
void INameEnvironment::ProcessPath(JikesClassPaths& paths)
{
	
#ifdef UNIX_FILE_SYSTEM
	NameSymbol* dot_path_name_symbol = dot_name_symbol;

	//
	// We need a place to start. Allocate a "." directory with no owner
	// initially. (Hence, the null argument.) Allocate a "." path whose
	// associated directory is the "." directory. Identify the "." path as
	// the owner of the "." directory. It's not a sourcepath, so pass false.
	//
	DirectorySymbol* default_directory = new DirectorySymbol(dot_name_symbol,
		NULL, false);
	// Since the "." directory may not be the first directory, set
	// dot_classpath_index to the first instance in classpath.
	dot_classpath_index = classpath.Length();
	classpath.Next() = classpath_table.InsertPathSymbol(dot_path_name_symbol,
		default_directory);
	// Note that the default_directory is reset after it has been assigned
	// the owner above.
	default_directory->ReadDirectory();
	system_directories.Next() = default_directory;

	system_table = new SystemTable();
	struct stat status;
	//FIXME: need to check for stat errors
	if (SystemStat(dot_name_symbol->Utf8Name(), &status) == 0 &&
		(status.st_mode & JIKES_STAT_S_IFDIR))
		system_table->InsertDirectorySymbol(status.st_dev, status.st_ino,
			default_directory);

#elif defined(WIN32_FILE_SYSTEM)

	char* main_current_directory = option->GetMainCurrentDirectory();
	int dot_path_name_length = strlen(main_current_directory);
	wchar_t* dot_path_name = new wchar_t[dot_path_name_length + 1];
	for (int i = 0; i < dot_path_name_length; i++)
		dot_path_name[i] = main_current_directory[i];
	dot_path_name[dot_path_name_length] = U_NULL;
	NameSymbol* dot_path_name_symbol = FindOrInsertName(dot_path_name,
		dot_path_name_length);
	delete[] dot_path_name;
	// We need a place to start. Allocate a "." directory with no owner
	// initially. (Hence, the null argument.) Allocate a "." path whose
	// associated directory is the "." directory. Identify the "." path as
	// the owner of the "." directory. It's not a sourcepath, so pass false.
	DirectorySymbol* default_directory = new DirectorySymbol(dot_name_symbol,NULL, false);
	// Since the "." directory may not be the first directory, set
	// dot_classpath_index to the first instance in classpath.
	dot_classpath_index = classpath.Length();
	classpath.Next() = classpath_table.InsertPathSymbol(dot_path_name_symbol,
		default_directory);
	// Note that the default_directory is reset after it has been assigned
	// the owner above.
	default_directory->ReadDirectory();
	system_directories.Next() = default_directory;
#endif // WIN32_FILE_SYSTEM


	for (auto it : paths._paths){
		AddClassPath(it.first, it.second);
	}
	// TODO: If the user did not specify "." in the class path we assume it.
	// javac makes that assumption also. Is that correct?
	//
	if (dot_classpath_index == 0)
		unnamed_package_dirs.Next() = classpath[0]->RootDirectory();
}


void INameEnvironment::AddClassPath(const std::wstring& _pathName, JikesClassPaths::DataElement* data_element)
{
		const wchar_t* path_name = _pathName.c_str();
		size_t path_name_length = _pathName.length();
		NameSymbol* name_symbol = FindOrInsertName(path_name, path_name_length);

		// If a directory is specified more than once, ignore the duplicates.
		if (classpath_table.FindPathSymbol(name_symbol)){
			if (name_symbol == dot_name_symbol) {
				// The next index
				dot_classpath_index = classpath.Length();
				// Share the "." directory.
				classpath.Next() = classpath[0];
				unnamed_package_dirs.Next() = classpath[0]->RootDirectory();
			}
			return;
		}

		// Check whether or not the path points to a system directory.
		// If not, assume it's a zip file.
		
		const wchar_t*  input_name = data_element->class_path.c_str();
		bool source_path_ony = data_element->source_only;
		size_t input_name_length = data_element->class_path.size();

		boost::system::error_code ec;

		if (boost::filesystem::is_directory(input_name, ec))
		{
			DirectorySymbol* dot_directory =
				ProcessSubdirectories(input_name, input_name_length, source_path_ony);
			unnamed_package_dirs.Next() = dot_directory;

			classpath.Next() =
				classpath_table.InsertPathSymbol(name_symbol,
					dot_directory);
		}
		else
		{
			bool record_bad_name = false;
			if (!source_path_ony)
			{
				bool no_zip_or_jar = false;
				if (input_name_length > 3) {
					const wchar_t* ending = &(input_name[input_name_length - 3]);
					// skip ., .., non-zip, and non-jar
					if (_wcsicmp(ending, L"zip") != 0 && _wcsicmp(ending, L"jar") != 0)
						no_zip_or_jar = true;
				}

				if (no_zip_or_jar == false) {
					errno = 0;
					DirectoryZip* zipinfo = new DirectoryZip(this, input_name);
					if (!zipinfo->IsValid())
					{
						// If the zipfile is all screwed up, give up here !!!
						wchar_t* name = new wchar_t[input_name_length + 1];
						for (int i = 0; i < input_name_length; i++)
							name[i] = input_name[i];
						name[input_name_length] = U_NULL;
						if (errno)
						{
							const char* std_err = strerror(errno);
							ErrorString err_str;
							err_str << '"' << std_err << '"'
								<< " while trying to open " << name;

							general_io_warnings.Next() = err_str.CallerFreeArray();
						}
						else
						{
							wchar_t* tail = &name[input_name_length - 3];
							if (Case::StringSegmentEqual(tail, US_zip, 3) ||
								Case::StringSegmentEqual(tail, US_jar, 3))
							{
								bad_zip_filenames.Next() = name;
							}
							else bad_dirnames.Next() = name;
						}
					}

					unnamed_package_dirs.Next() = zipinfo->RootDirectory();

					// Create the new path symbol and update the class path with it.

					PathSymbol* path_symbol =
						classpath_table.InsertPathSymbol(name_symbol,
							zipinfo->RootDirectory());
					path_symbol->zipfile = zipinfo;
					classpath.Next() = path_symbol;
				}
				else
				{
					record_bad_name = true;
				}
			}
			else
			{
				record_bad_name = true;
			}

			if (record_bad_name)
			{
				// We don't process zip files as source directories
				wchar_t* name = new wchar_t[input_name_length + 1];
				for (unsigned i = 0; i < input_name_length; i++)
					name[i] = input_name[i];
				name[input_name_length] = U_NULL;
				bad_dirnames.Next() = name;
			}
		}
		
}

	void INameEnvironment::AddClassPaths(JikesClassPaths& paths)
	{
		for (auto it : paths._paths) {
			AddClassPath(it.first, it.second);
		}
	}
} // Close namespace Jikes block


