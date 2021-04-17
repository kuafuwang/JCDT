#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <vector>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/JarPackageFragment.h>


#include <JavaModelLib/internal/core/BaseProject.h>
#include <PathModelLib/JarPath/JarPathManager.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	JarPackageFragmentRoot::~JarPackageFragmentRoot(){
	
	}

	IJavaElement* JarPackageFragmentRoot::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new JarPackageFragmentRoot(*this);
		clone->MarkClone();
		clone->fParent = parent;
	
		return clone;
	}

	JarPackageFragmentRoot::JarPackageFragmentRoot(const wstring& _path,
		IJavaProject* p, const wstring& name, const wstring& attach_path)
		: PackageFragmentRoot(_path,K_BINARY,p, name, attach_path,IJavaElementType::JAR_PACKAGE_FRAGMENT_ROOT)
	
	{

	}

	JarPackageFragmentRoot::JarPackageFragmentRoot(JarPackageFragmentRoot& o): PackageFragmentRoot(o),
		_jar_pkgs(o._jar_pkgs)
	{

	}

	void JarPackageFragmentRoot::computeChildren()
	{
		if (!fParent){
			return;
		}
	

		auto prject = reinterpret_cast<OriginalJavaProject*>(fParent);
		_jar_pkgs = prject->jar_path_manager_.getJarPackageRoot(resoucePath.wstring());
		if (!_jar_pkgs)
			return;

		auto _pkgs = _jar_pkgs->getChildren();
		for(const auto& it : _pkgs){
			auto pkg = new JarPackageFragment(it.first, this);
			children.insert(make_pair(it.first, pkg));
		}
	}

	IPackageFragment* JarPackageFragmentRoot::FindOrInsertPackageFragment(const wstring& compound_name)
	{
		
		auto find_it = children.find(compound_name);
		if (find_it != children.end())
		{
			return find_it->second;
		}

		JarPackageFragment* pkg = new JarPackageFragment(compound_name, this);
		children.insert(make_pair(compound_name, pkg));
		return pkg;
	}
	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




