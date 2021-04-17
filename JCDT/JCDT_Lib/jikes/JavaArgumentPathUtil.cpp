// $Id: control.cpp,v 1.80 2004/12/12 04:35:52 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "stdafx.h"
#include "JavaArgumentPathUtil.h"

#include "JCDT_Lib/internal/util/case.h"
#include "JCDT_Lib/internal/impl/option.h"
#include <string>
#include <vector>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include "JCDT_Lib/internal/impl/JikesClassPaths.h"

using std::wstring;
using std::vector;

namespace Jikes { // Open namespace Jikes block


JavaArgumentPathUtil::JavaArgumentPathUtil(Option* option_)
   : option(option_)
{
	JikesClassPaths collector;
	PreProcessPath(option->bootclasspath, false, collector);
	PreProcessPath(option->classpath, false, collector);
	PreProcessPath(option->sourcepath, true, collector);

	
}

JavaArgumentPathUtil::~JavaArgumentPathUtil()
{
  
}

void JavaArgumentPathUtil::GetClassPaths(JikesClassPaths& collector)

{
	if(option->bootclasspath)
		PreProcessPath(option->bootclasspath, false, collector);
	
	if(option->classpath)
		PreProcessPath(option->classpath, false, collector);
	
	if (option->sourcepath)
		PreProcessPath(option->sourcepath, true, collector);
	if(option->extdirs)
		PreProcessExtDirs(option->extdirs, collector);
}

void JavaArgumentPathUtil::PreProcessPath(char* path_to_be_deal, bool source_path_ony, JikesClassPaths& collector)
{
	if (path_to_be_deal)
	{
		// The longest possible path name we can encounter
		int max_path_name_length = strlen(path_to_be_deal) + 1;
		wchar_t* path_name = new wchar_t[max_path_name_length + 1];

		wchar_t* input_name = NULL;
#ifdef WIN32_FILE_SYSTEM
		char* full_directory_name = NULL;
#endif

		for (char* path = path_to_be_deal,
			*path_tail = &path[strlen(path)];
			path < path_tail; path++)
		{
#ifdef WIN32_FILE_SYSTEM
			delete[] full_directory_name;
			delete[] input_name;
#endif
			char* head;
			for (head = path;
			path < path_tail
				&& *path != PathSeparator();
				path++);

				// If a separator was encountered, replace it by \0 to terminate
				// the string.
				*path = U_NULL;
			int input_name_length = path - head;

			int path_name_length = input_name_length;
			for (int i = 0; i < path_name_length; i++)
				path_name[i] = head[i];
			path_name[path_name_length] = U_NULL;

#ifdef UNIX_FILE_SYSTEM

			input_name = path_name;

#elif defined(WIN32_FILE_SYSTEM)

			input_name = NULL;
			full_directory_name = NULL;
			char disk = (input_name_length >= 2 &&
				Case::IsAsciiAlpha(head[0]) &&
				head[1] == U_COLON ? head[0] : 0);

			//
			// Look for the directory. If it is found, update input_name and
			// head.
			//
			option->SaveCurrentDirectoryOnDisk(disk);
			if (SetCurrentDirectoryA(head))
			{
				char tmp[1];
				// First, get the right size.
				DWORD directory_length = GetCurrentDirectoryA(0, tmp);
				full_directory_name = new char[directory_length + 1];
				DWORD length = GetCurrentDirectoryA(directory_length,
					full_directory_name);
				if (length <= directory_length)
				{
					for (char* ptr = full_directory_name; *ptr; ptr++)
					{
						*ptr = (*ptr != U_BACKSLASH
							? *ptr : (char)U_SLASH); // turn '\' to '/'.
					}

					input_name_length = length;
					input_name = new wchar_t[input_name_length + 1];
					for (int k = 0; k < input_name_length; k++)
						input_name[k] = full_directory_name[k];
					input_name[input_name_length] = U_NULL;
					head = full_directory_name;
				}
			}

			//
			// Default input_name, in case we do not succeed in finding the
			// directory.
			//
			if (!input_name)
			{
				input_name = new wchar_t[input_name_length + 1];
				for (int j = 0; j < input_name_length; j++)
					input_name[j] = path_name[j];
				input_name[input_name_length] = U_NULL;
			}

			// Reset the current directory on disk.
			option->ResetCurrentDirectoryOnDisk(disk);
			// Reset the real current directory...
			option->SetMainCurrentDirectory();
#endif // WIN32_FILE_SYSTEM

			if (input_name_length > 0)
			{
				wstring path_name_string(path_name, path_name + path_name_length);
				wstring input_name_string(input_name, input_name + input_name_length);
				collector.AddPath(path_name_string, input_name_string,L"", source_path_ony);
			}
		}

#ifdef WIN32_FILE_SYSTEM
		delete[] full_directory_name;
		delete[] input_name;
#endif // WIN32_FILE_SYSTEM
		delete[] path_name;
	}
}

void JavaArgumentPathUtil::PreProcessExtDirs(char* path_to_be_deal, JikesClassPaths& collector_secode)
{

	JikesClassPaths ext_collector;
	PreProcessPath(path_to_be_deal, false, ext_collector);

	auto it = ext_collector._paths.begin();
	auto end = ext_collector._paths.end();
	for (; it != end; ++it)
	{	
		//
		// Check whether or not the path points to a system
		// directory. TODO If not, should we print a warning ??
		//
		//
		// Check whether or not the path points to a system directory.
		// If not, assume it's a zip file.
		//
		const wchar_t*  input_name = it->second->class_path.c_str();
		bool source_path_ony = it->second->source_only;
		size_t input_name_length = it->second->class_path.size();
		char* head = new char[input_name_length * 3 + 1];
		CharOperation::ConvertUnicodeToUtf8(input_name, head);

		if (SystemIsDirectory(head))
		{
			//FIXME: should be in platform.cpp??
#ifdef UNIX_FILE_SYSTEM

			DIR* extdir = opendir(head);

			if (extdir)
			{
				for (dirent* entry = readdir(extdir); entry; entry =
					readdir(extdir))
				{
					int entry_length = strlen(entry->d_name);
					// + 1 for possible '/' between path and file.
					int fullpath_length = input_name_length +
						entry_length + 1;
					char* ending = &(entry->d_name[entry_length - 3]);
					// skip ., .., non-zip, and non-jar
					if (!strcmp(entry->d_name, ".") ||
						!strcmp(entry->d_name, "..") ||
						(strcasecmp(ending, "zip") &&
							strcasecmp(ending, "jar")))
					{
						continue;
					}

					char* extdir_entry = new char[fullpath_length + 1];
					// First put on path.
					strcpy(extdir_entry, head);

					// Add '/' if it's not already there
					if (head[input_name_length - 1] != U_SLASH)
						strcat(extdir_entry, U8S_SL);

					// Then add the filename.
					strcat(extdir_entry, entry->d_name);

					wchar_t* extdir_entry_name =
						new wchar_t[fullpath_length + 1];
					for (int i = 0; i < fullpath_length; ++i)
						extdir_entry_name[i] = extdir_entry[i];

					wstring input_name(extdir_entry_name, extdir_entry_name + wcslen(extdir_entry_name));
					collector_secode.AddPath(input_name, input_name, false);
				}
				closedir(extdir);
			}
#elif defined(WIN32_FILE_SYSTEM)

			// +2 for "/*" +1 for '\0'
			char* directory_name = new char[input_name_length + 3];
			strcpy(directory_name, head);
			if (directory_name[input_name_length - 1] != U_SLASH)
				directory_name[input_name_length++] = U_SLASH;
			directory_name[input_name_length++] = U_STAR;
			directory_name[input_name_length] = U_NULL;

			WIN32_FIND_DATAA entry;
			HANDLE file_handle = FindFirstFileA(directory_name, &entry);
			if (file_handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					int entry_length = strlen(entry.cFileName);
					// + 1 for possible '/' between path and file.
					int fullpath_length = input_name_length +
						entry_length + 1;
					char* ending = &(entry.cFileName[entry_length - 3]);
					// skip ., .., and not zip or jar
					if ((!strcmp(entry.cFileName, ".")) ||
						(!strcmp(entry.cFileName, "..")) ||
						(strcasecmp(ending, "zip") &&
							strcasecmp(ending, "jar")))
					{
						continue;
					}

					char* extdir_entry = new char[fullpath_length + 1];
					wchar_t* extdir_entry_name =
						new wchar_t[fullpath_length + 1];
					// First put path
					strcpy(extdir_entry, head);

					// If no slash, add slash before copying filename.
					if (head[input_name_length - 1] != U_SLASH)
					{
						int path_length = input_name_length + 1;
						strcat(extdir_entry, U8S_SL);

						for (int i = 0; i < entry_length; i++)
						{
							extdir_entry[i + path_length] =
								entry.cFileName[i] == U_BACKSLASH
								? (char)U_SLASH : entry.cFileName[i];
						}
					}
					else
					{ // If it's there, just append filename.
						for (int i = 0; i < entry_length; i++)
						{
							extdir_entry[i + input_name_length] =
								entry.cFileName[i] == U_BACKSLASH
								? (char)U_SLASH : entry.cFileName[i];
						}
					}

					for (int i = 0; i < fullpath_length; ++i)
						extdir_entry_name[i] = extdir_entry[i];

					wstring input_name(extdir_entry_name, extdir_entry_name + wcslen(extdir_entry_name));

					collector_secode.AddPath(input_name, input_name,L"", false);
				} while (FindNextFileA(file_handle, &entry));
				FindClose(file_handle);
			}

			delete[] directory_name;
#endif // WIN32_FILE_SYSTEM
		}
		else
		{
			collector_secode.AddPath(it->first, it->second->class_path,it->second->source_attach_path, false);
		}
		delete[] head;
	}
}




} // Close namespace Jikes block


