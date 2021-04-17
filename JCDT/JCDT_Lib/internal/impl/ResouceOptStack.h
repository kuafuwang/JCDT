// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ResouceOptStack_INCLUDED
#define ResouceOptStack_INCLUDED
#include <vector>
#include <string>
#include <set>
#include <boost/thread/mutex.hpp>
namespace Jikes { // Open namespace Jikes block
	enum ResourceOptKind
	{
		FILE_ADD,
		FILE_CHANGE,
		FILE_REMOVE
	};
	struct ResouceOpt
	{
		ResourceOptKind kind;
		std::vector < std::string > file_path;
	};
	class ResouceOptStack
	{
		void PushOpt(ResouceOpt* opt){
			boost::mutex::scoped_lock lock(opt_mutex);

			if(!m_rebuild){
				stack.push_back(opt);
			}
			else{
				delete opt;
			}
		}
		void Rebuild()// 当工程依赖关系发生改变和 class改变时候，要重新build
		{
			boost::mutex::scoped_lock lock(opt_mutex);
			auto it = stack.begin();
			auto end = stack.end();
			for (; it != end; ++it){
				delete *it;
			}
			stack.clear();
			m_rebuild = true;
		}
		enum CompilerAct{
			REBUILD,
			INCREMENT,
			NOTHING
		};
		CompilerAct GetBuildOpt(std::set<std::string>& remove_files,
			std::set<std::string>& add_files,
			std::set<std::string>& chang_files
			)
		{
			boost::mutex::scoped_lock lock(opt_mutex);
			if(m_rebuild){
				m_rebuild = false;
				stack.clear();
				return REBUILD;
			}
			
			if (!stack.size())
				return NOTHING;	

			
			remove_files.clear();
			add_files.clear();
			chang_files.clear();

			std::map<std::string, ResourceOptKind> first_set;
			for (size_t i = stack.size(); i > 0; --i)
			{
				ResouceOpt* opt = stack[i];
				std::vector < std::string >& files = opt->file_path;
				size_t k = 0;
				size_t size = files.size();
				for (; k < size; ++k) {
					std::string& file_path = files[i];
					first_set.insert(make_pair(file_path, opt->kind));
					if (FILE_ADD == opt->kind){
						if(!remove_files.erase(file_path))
							chang_files.erase(file_path);

						add_files.insert(file_path);
					}
					else if (FILE_CHANGE == opt->kind){
						if(remove_files.erase(file_path))
							add_files.erase(file_path);

						chang_files.insert(file_path);
					}
					else {
						if(!chang_files.erase(file_path))
							add_files.erase(file_path);

						remove_files.insert(file_path);
					}
				}	
				delete opt;
			}

			auto it = add_files.begin();
			auto end = add_files.end();

			it = chang_files.begin();
			end = chang_files.end();
			for (; it != end; ++it) {
				ResourceOptKind kind = first_set[*it];
				if (FILE_ADD == kind)// 开始的时候是add,那说明这个文件对当前的编译状态来说，它是已经不存在的文件，新添加的文件
				{
					add_files.insert(*it);
					chang_files.erase(it);

				}
			}

			for (; it != end; ++it){
				ResourceOptKind kind = first_set[*it];
				if(FILE_ADD != kind)// 开始的时候不是add,那说明这个文件对当前的编译状态来说，它是已经存在的文件
				{
					chang_files.insert(*it);
					add_files.erase(it);
					
				}		
			}
			
			it = remove_files.begin();
			end = remove_files.end();
			for (; it != end; ++it) {
				ResourceOptKind kind = first_set[*it];
				if (FILE_ADD == kind)// 开始的时候是add,那说明这个文件对当前的编译状态来说，它是已经不存在的文件，新添加的文件
				{
					remove_files.erase(it);
				}
			}
			stack.clear();

			return INCREMENT;
		}
	private:
		boost::mutex opt_mutex;
		bool m_rebuild = true;
		std::vector<ResouceOpt*> stack;
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

