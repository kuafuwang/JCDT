#include "stdafx.h"
#include "JarPackage.h"
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		JarPackage::JarPackage(const std::wstring& _name):name(_name)
		{
			_end = children.end();
		}

		JarPackage::~JarPackage()
		{
			for(auto it : children){
				delete it.second;
			}
		}

		wstring JarPackage::DEFAULT_PACKAGE_NAME;


	} // Close namespace JavaModel block

} // Close namespace Jikes block

