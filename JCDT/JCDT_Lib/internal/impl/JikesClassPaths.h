// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef JikesClassPaths_INCLUDED
#define JikesClassPaths_INCLUDED
#include <map>
#include <vector>

namespace Jikes { // Open namespace Jikes block


	class JikesClassPaths
	{
	public:
		~JikesClassPaths()
		{
			for(auto it : _paths)
			{
				delete it.second;
			}
		}
		struct DataElement
		{
			DataElement(const std::wstring& path_,
				const std::wstring& source_attach,
				bool _source_only)
				:class_path(path_),
				source_attach_path(source_attach),
				source_only(_source_only)
			{
			}
			std::wstring class_path;
			std::wstring source_attach_path;
			bool source_only;
		};
		bool IsIsElement(const std::wstring& name)
		{
			if (_paths.find(name) != _paths.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool AddPath(const std::wstring& name, const std::wstring& path_, const std::wstring& source_attach, bool source_only)
		{
			if (_paths.find(name) != _paths.end()) {
				return false;
			}	
			_paths[name] = new DataElement(path_, source_attach, source_only);
			
			return false;
		}
		bool RemovePath(const std::wstring& name)
		{
			auto it = _paths.find(name);
			if (it != _paths.end()) {
				_paths.erase(it);
				return true;
			}
			else
			{
				return false;
			}
		}
		bool IsEmpty()
		{
			return _paths.empty();
		}
		std::map<std::wstring, DataElement*> _paths;
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

