
#ifndef JarPackage_jikes_PathModel_INCLUDED_INCLUDED
#define JarPackage_jikes_PathModel_INCLUDED_INCLUDED
#include <string>
#include <vector>
#include <map>
#include "ClassFile.h"


namespace Jikes { // Open namespace Jikes block
	

namespace PathModel{
	class JarPathManager;
	class ClassFile;
	class JarPackage
	{
	public:
		 static std::wstring DEFAULT_PACKAGE_NAME;
		std::wstring name;
		JarPackage(const std::wstring& _name);
		~JarPackage();
		inline PathModel::ClassFile* GetClassFile(std::wstring& _name)
		{
			auto find_it = children.find(_name);
			if(find_it != _end){
				return find_it->second;
			}	
			return nullptr;	
		}
		void  AddClassFile(PathModel::ClassFile* _file)
		{
			auto find_it = children.find(_file->name);
			if (find_it != _end){
				delete  find_it->second;
				find_it->second = _file;
			}
			else{
				children.insert(std::make_pair(_file->name, _file));
			}
		}

		std::map<std::wstring,PathModel::ClassFile*> children;
		std::map<std::wstring, PathModel::ClassFile*>::const_iterator _end;
		
	};

}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

