#include "stdafx.h"
#include <JavaModelLib/internal/core/PackageFragmentRoot.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/IJavaProject.h>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	IPackageFragment* PackageFragmentRoot::FindOrInsertPackageFragment(const wstring& compound_name)
	{
		auto find_ = getPackageFragment(compound_name);
		if(find_ != nullptr){
			return find_;
		}

		PackageFragment* pkg = new PackageFragment(compound_name, this);
		children.insert(make_pair(compound_name, pkg));
		return pkg;

	}
	bool PackageFragmentRoot::CopyChildFromOriginalWhenIsClone()
	{
		if (is_Clone && fParent)
		{
			auto proj = getJavaProject();
			
			if (!proj)
				return false;
			auto pJm = getJavaModel();
			if (!pJm)
				return false;
			auto name = proj->getElementName();
			auto original_proj = pJm->getJavaProject(name);
			if (!original_proj)
				return false;

			original_proj->beginToOccupy();
			
		
			auto original_pkg = reinterpret_cast<PackageFragmentRoot*>(original_proj->findPackageFragmentRootByName(fName));

			if (original_pkg){
				sourceAttachmentRootPath = original_pkg->sourceAttachmentRootPath;
				auto _origianl_children = original_pkg->getInternalChildren();
				for(const auto& it : _origianl_children)
				{
					if(!it.second)
					{
						continue;
					}
					auto temp = reinterpret_cast<IPackageFragment*>(it.second->ShallowCopy(this));
					children.insert(make_pair(it.first, temp));
				}
			}

			original_proj->endToOccupy();

			return true;
		}
		
		return false;
		
	}

	PackageFragmentRoot::~PackageFragmentRoot()
	{
		for (auto it : children){
			delete it.second;
		}
		children.clear();
	}

	const map<wstring, IPackageFragment*>& PackageFragmentRoot::getInternalChildren()
	{
		LazyLoad();
		return children;
	}

	void PackageFragmentRoot::addChild(IJavaElement* child)
	{
		assert(child);
		if (!child)
			return;

		auto _type = child->getElementType();
		if (_type == IJavaElementType::JAR_PACKAGE_FRAGMENT ||
			_type == IJavaElementType::PACKAGE_FRAGMENT)
		{
			auto real_child = dynamic_cast<IPackageFragment*>(child);
			if (real_child)
			{
				addChild(real_child->getCompoundName(), real_child);
			}
		}
	}


	bool PackageFragmentRoot::equals(IJavaElement* o)
	{
		
			if (this == o)
				return true;
			if (!o) return false;

			if (o->fLEType == JAR_PACKAGE_FRAGMENT_ROOT || o->fLEType == PACKAGE_FRAGMENT_ROOT) {
				auto other = (PackageFragmentRoot*)o;
				return resoucePath == other->resoucePath;
			}
			
			return false;
		
	}

	IJavaElement* PackageFragmentRoot::DeepCopy(IJavaElement* parent)
	{
		auto clone = reinterpret_cast<PackageFragmentRoot*>(ShallowCopy(parent));
		if(clone)
			clone->HelpToDeepCopy(*this);
		return clone;
	}

	wstring PackageFragmentRoot::getPath()
	{
		return resoucePath.wstring();
	}

	void PackageFragmentRoot::HelpToDeepCopy(PackageFragmentRoot& o)
	{
		for (auto it : o.children) {

			if(!it.second)
			{
				continue;
			}

			auto temp = reinterpret_cast<IPackageFragment*>(it.second->DeepCopy(this));
			children.insert(make_pair(it.first, temp));
		}
	}


	std::vector<IJavaElement*>* PackageFragmentRoot::getChildrenOfType(IJavaElementType _type)
	{
		LazyLoad();
		if (_type == JAR_PACKAGE_FRAGMENT || 
			_type ==  PACKAGE_FRAGMENT)
		{
			if (!children.empty()){
				auto _ret = new std::vector<IJavaElement*>();
				for (auto it : children)
					_ret->push_back(it.second);

				return _ret;
			}
		}
		return nullptr;
	}

	int PackageFragmentRoot::getKind()
	{
		return root_kind;
	}

	void PackageFragmentRoot::addChild(const wstring& compoundName, IPackageFragment* child)
	{
		assert(child);
		if (!child)
			return;
		auto find_it = children.find(compoundName);
		if(find_it != children.end()){
			delete find_it->second;
			find_it->second = child;
		}
		else{
			children.insert(make_pair(compoundName, child));
		}

	}

	bool PackageFragmentRoot::hasChildren()
	{

		LazyLoad();
		if (!children.empty()) {
			return true;
		}
		else{
			return false;
		}

	}

	std::vector<IJavaElement*>* PackageFragmentRoot::getChildren()
	{
		LazyLoad();
		if (!children.empty()) {
			auto _ret = new std::vector<IJavaElement*>();
			for (auto it : children){
				_ret->push_back(it.second);
			}
			return _ret;
		}
		return nullptr;
	}

	void PackageFragmentRoot::removeInfo()
	{
		for (auto it : children){
			delete it.second;
		}
		children.clear();	
	}

	IPackageFragment* PackageFragmentRoot::getPackageFragment(const wstring& packageName)
	{
		LazyLoad();
		auto find_it = children.find(packageName);
		if (find_it != children.end()) 
			return find_it->second;
		else
			return nullptr;
	}

	void PackageFragmentRoot::ProcessRemoveElement(const wstring& remove_pkg)
	{
		auto find_it = children.find(remove_pkg);
		if (find_it != children.end()) {
			delete find_it->second;
			children.erase(find_it);
		}
	}

	void PackageFragmentRoot::ProcessRemoveElement(map<wstring, NameLookUpNotify*>& remove_pkgs)
	{
		// 这里这有 K_SOURCE 类型的 root 才会增量改变，K_BINARY 要是改变就全部改变
		for (auto it : remove_pkgs){
			auto pkg_name = it.first;
			auto find_it = children.find(pkg_name);
			if (find_it != children.end()) {
				delete find_it->second;
				children.erase(find_it);
			}
		}
	}

	void PackageFragmentRoot::IncrementUpdateProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >& optResouces)
	{
		// 这里这有 K_SOURCE 类型的 root 才会增量改变，K_BINARY 要是改变就全部改变
		
		assert(root_kind != K_BINARY);

		for (auto it : optResouces)
		{
			ResourceOptKind _the_kind = it.first;
			if (_the_kind == ELEMENT_ADD || _the_kind == ELEMENT_CHANGE)
			{
				map<wstring, NameLookUpNotify*>& pkgs = it.second;
				for (auto it_pkgs : pkgs) {
					auto pkg_name = it_pkgs.first;
					auto find_it = children.find(pkg_name);
					
					if (find_it != children.end() && find_it->second) {
						find_it->second->setIsStructureKnown(false);
					}
					else
					{
						auto clone_child = new PackageFragment(pkg_name, this);
						children.insert(make_pair(pkg_name, clone_child));
					}
				}
			}
			else if(_the_kind == ELEMENT_REMOVE)
			{
				map<wstring, NameLookUpNotify*>& pkgs = it.second;
				for (auto it_pkgs : pkgs) {
					auto pkg_name = it_pkgs.first;
					auto find_it = children.find(pkg_name);
					if (find_it != children.end()) {
						delete find_it->second;
						children.erase(find_it);
					}
				}
			}
		}
	}

	PathModel::MosesPath* PackageFragmentRoot::getMosesPath()
	{
		if (moses)
			return moses;
		if (!fParent)
			return nullptr;
		bool binary;
		if(fLEType == JAR_PACKAGE_FRAGMENT_ROOT)
		{
			binary = true;
		}
		else
		{
			binary = false;
		}
		moses = new PkgRootMoses(fParent->getElementName(), fName, binary);

		return moses;
	}

	void PackageFragmentRoot::SynchronousProcess(map< ResourceOptKind, map<wstring, NameLookUpNotify*> >& optResouces,
		IPackageFragmentRoot* clone)
	{
		auto real_clone = dynamic_cast<PackageFragmentRoot*>(clone);

		if (!real_clone)
			return;

		auto end = children.end();
		for (auto it : optResouces)
		{	
			ResourceOptKind _the_kind = it.first;
			if (_the_kind != ELEMENT_ADD &&  _the_kind != ELEMENT_CHANGE)
				continue;
			map<wstring, NameLookUpNotify*>& pkgs = it.second;
			for (auto it_pkgs : pkgs) {
				auto pkg_name = it_pkgs.first;
				auto find_it = children.find(pkg_name);
				if (find_it != end) {

					auto find_in_duplicate = real_clone->children.find(pkg_name);
					if (find_in_duplicate != real_clone->children.end() && find_in_duplicate->second) {
						find_in_duplicate->second->setIsStructureKnown(false);
					}
					else {
						if(find_it->second)
						{
							auto clone_child = reinterpret_cast<IPackageFragment*>(find_it->second->ShallowCopy(real_clone));
							real_clone->children.insert(make_pair(pkg_name, clone_child));
						}
					}
				}
			}
		}
	}

	IJavaElement* PackageFragmentRoot::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new PackageFragmentRoot(*this);
		clone->fParent = parent;
		clone->MarkClone();
		return clone;
	}

	
	IPackageFragment* PackageFragmentRoot::getPackageFragment(wstring& packageName)
	{
		auto find_it = children.find(packageName);
		if (find_it != children.end())
			return  find_it->second;
		else
			return nullptr;
	}

	wstring PackageFragmentRoot::getSourceAttachmentPath()
	{
		return sourceAttachmentRootPath;
	}

	wstring PackageFragmentRoot::getSourceAttachmentRootPath()
	{
		return sourceAttachmentRootPath;
	}

	bool PackageFragmentRoot::isArchive()
	{
		return false;
	}

	bool PackageFragmentRoot::isExternal()
	{
		return false;
	}

	bool PackageFragmentRoot::generateInfos(IProgressMonitor* pm){
		if(! CopyChildFromOriginalWhenIsClone() ){
			computeChildren();
		}
		return true;
	}

	

	void PackageFragmentRoot::computeChildren()
	{
		namespace fs = boost::filesystem;
	
		if (!fs::exists(resoucePath)) {
			return;
		}
		if (!fs::is_directory(resoucePath)) {
			return;
		}
		wstring prefix = IPackageFragment::DEFAULT_PACKAGE_NAME;
		computeFolderChildren(resoucePath.wstring(), prefix);
		
	}
	void PackageFragmentRoot::computeFolderChildren(const wstring& folder,const wstring& prefix)
	{
		PackageFragment* pkg = new PackageFragment(prefix, this);
		
		children.insert(make_pair(prefix, pkg));
		
		namespace fs = boost::filesystem;
		fs::directory_iterator _end;
		fs::path original_path(folder);
		for (fs::directory_iterator pos(original_path); pos != _end; ++pos) {
			fs::path file(*pos);
			auto new_prefix( prefix);
			if (fs::is_directory(file) == true ) {
				if(new_prefix != IPackageFragment::DEFAULT_PACKAGE_NAME)
					new_prefix += L"." +  file.filename().wstring() ;
				else
					new_prefix += file.filename().wstring();
				computeFolderChildren(file.wstring(), new_prefix);
			}
		}
	}

	PackageFragmentRoot::PackageFragmentRoot(
		const wstring& _path,
		int _root_type,
		IJavaProject* p,
		const wstring& name,
		const wstring& attach_path,
		IJavaElementType _type)
		: IPackageFragmentRoot( p,_type, name),
		resoucePath(_path),
		root_kind(_root_type),
		sourceAttachmentRootPath(attach_path)
	{
		
	}

	PackageFragmentRoot::PackageFragmentRoot(PackageFragmentRoot&o)
		: IPackageFragmentRoot(o), 
		resoucePath(o.resoucePath),
		root_kind(o.root_kind),
		sourceAttachmentRootPath(o.sourceAttachmentRootPath)
	{
		
		
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




