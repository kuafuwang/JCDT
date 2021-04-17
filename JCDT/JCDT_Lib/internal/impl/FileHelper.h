// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef FileHelper_INCLUDED
#define FileHelper_INCLUDED
#include <set>
#include <vector>

namespace Jikes {
	class FileWriter;
	class FileReader;

	// Open namespace Jikes block


	class AssistFileHelper
	{
	public:
		virtual ~AssistFileHelper()
		{
		}

		virtual FileReader* read(const char* filename) = 0;
		virtual FileWriter* write(const char* filename, size_t bytes) = 0;

	};
	class FileHelper
	{
	private:
		
		FileHelper() 
		{}
		vector<AssistFileHelper*> assist_list;

	public:

		static void ProcessNewInputFiles(set<wstring>& file_set, char** arguments);

		~FileHelper();
		static FileHelper* getInstance();
		static bool LastModifiedTime(const char*, time_t& m_time);
		static bool LastModifiedTime(const string&, time_t& m_time);

		FileReader* read(const char* );
		FileReader* read(const std::wstring&);
		FileReader* GetVirutalFile();
		FileWriter* write(const char* , size_t );
		bool RegisterAssist(AssistFileHelper*);
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

