#include "stdafx.h"
#include <JavaModelLib/internal/core/JarPackageFragment.h>
#include <JavaModelLib/internal/core/IPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/IClassFile.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <PathModelLib/JarPath/JarPackageRoot.h>
#include <PathModelLib/JarPath/JarPackage.h>
#include <JavaModelLib/internal/core/ClassFile.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	 JarPackageFragment::JarPackageFragment(const wstring& compound_name, IJavaElement* owner_)
		: PackageFragment(compound_name, owner_, IJavaElementType::JAR_PACKAGE_FRAGMENT)
	{
		
	}

	void JarPackageFragment::computeChildren()
	 {
		 //JarPackageFragmentRoot
		 if (!fParent)
			 return;
		 auto _root = reinterpret_cast<JarPackageFragmentRoot*>(fParent);
		 auto childe_in_jar_path = _root->_jar_pkgs->getChildren();
		
		 if (childe_in_jar_path.empty())
			 return;
		 auto find_it = childe_in_jar_path.find(fName);
		 if(find_it != childe_in_jar_path.end() && find_it->second)
		 {
			 auto _jar_pkg = find_it->second;

			 for(auto it : _jar_pkg->children)
			 {
				 auto _file = new Jikes::JavaModel::ClassFile(JAR_CLASS_FILE, this, it.first);
				 _file->_class_in_jar = it.second;
				 if(it.first.find('$') == wstring::npos)
				 {
					 if (!class_files)
						 class_files = new map<wstring, IClassFile*>();
					 class_files->insert(std::make_pair(it.first, _file));
				 }
				 else
				 {
				
					if (!innerClassesFiles)
							 innerClassesFiles = new map<wstring, IClassFile*>();
					 innerClassesFiles->insert(std::make_pair(it.first, _file));
				 }
			 }
		 }
	 }


	IJavaElement* JarPackageFragment::ShallowCopy(IJavaElement* parent)
	{
		JarPackageFragment* clone;
		
		clone = new JarPackageFragment(fName, fParent);

		clone->setIsStructureKnown(false);
			
	
		clone->fParent = parent;
		clone->MarkClone();
		return clone;
	}

}// Close namespace JavaModel block
	

} // Close namespace Jikes block




