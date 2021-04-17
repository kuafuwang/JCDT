#include "stdafx.h"
#include <JavaModelLib/internal/core/PackageFragment.h>
#include  <JavaModelLib/internal/core/PackageFragmentRoot.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/IClassFile.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/BaseJavaFile.h>
#include <JavaModelLib/internal/core/BaseProject.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JavaModelLib/internal/core/ClassFile.h>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	const wstring IPackageFragment::DEFAULT_PACKAGE_NAME = L"";

	IJavaElement* PackageFragment::DeepCopy(IJavaElement* parent)
	{
		auto clone = reinterpret_cast<PackageFragment*>(ShallowCopy(parent));
		if(clone)
			clone->HelpToDeepCopy(*this);
		return clone;
	}

	wstring PackageFragment::getPath()
	{
		if (!fParent)
			return{};
		auto root_path = fParent->getPath();
		if (!IsZip())
		{
			root_path.append(L"/").append(boost::replace_all_copy(fName, L".", L"/"));
		}
		return root_path;
	}
	std::wstring PackageFragment::getJavaCardDocUrlFragment()
	{
		wstring  url;
		auto idx = fName.find(L"*");
		if (idx != wstring::npos) {
			url = fName.substr(0, idx - 1);
		}
		else
		{
			url = fName;
		}
		boost::replace_all(url, L".", L"/");
		url.append(L"/package-summary.html");

		return url;
	}
	std::vector<IJavaElement*>* PackageFragment::getChildrenOfType(IJavaElementType _type)
	{
		LazyLoad();
		std::vector<IJavaElement*>* _ret = nullptr;
		if(_type == IJavaElementType::CLASS_FILE || _type == IJavaElementType::JAR_CLASS_FILE)
		{
			if (class_files)
			{

				_ret =  new std::vector<IJavaElement*>();
				for(const auto& it : *class_files){
					_ret->push_back(it.second);
				}
			}
		}
		else if (_type == IJavaElementType::COMPILATION_UNIT || _type == IJavaElementType::JAR_COMPILATION_UNIT) {
			if (compilations)
			{
				 _ret = new std::vector<IJavaElement*>();
				for (auto it : *compilations) {
					_ret->push_back(it.second);
				}
			
			}

		}
		return _ret;
	}

	//$NON-NLS-1$
	ICompilationUnit* PackageFragment::FindOrInsertCompilationUnit(const wstring& name)
	{
		bool in_jar = fLEType == JAR_PACKAGE_FRAGMENT ? true : false;
		IJavaElementType clone_type = in_jar ? JAR_COMPILATION_UNIT : COMPILATION_UNIT;
		if (!compilations) {
			compilations = new map<wstring, ICompilationUnit*>();
			
			auto _file = new CompilationUnit(clone_type, this, name);
			compilations->insert(make_pair(name, _file));
			return _file;
		}
		auto find_it = compilations->find(name);
		if (find_it == compilations->end()){
			auto _file = new CompilationUnit(clone_type, this, name);
			compilations->insert(make_pair(name, _file));
			return _file;
		}
		else
		{
			return find_it->second;
		}	
		return nullptr;
	}

	IClassFile* PackageFragment::FindOrInsertClassFile(const wstring& name) 
	{
		bool in_jar = fLEType == JAR_PACKAGE_FRAGMENT ? true : false;
		map<wstring, IClassFile*>* class_files_temp;
		IJavaElementType clone_type = in_jar ? JAR_CLASS_FILE : CLASS_FILE;
		if (name.find('$') != wstring::npos) {
			if (!innerClassesFiles) {
				innerClassesFiles = new map<wstring, IClassFile*>();
				auto _file = new ClassFile(clone_type, this, name);
				innerClassesFiles->insert(make_pair(name, _file));
				return _file;
			}
			class_files_temp = innerClassesFiles;
		}
		else {
			if (!class_files) {
				class_files = new map<wstring, IClassFile*>();
				auto _file = new ClassFile(clone_type, this, name);
				class_files->insert(make_pair(name, _file));
				return _file;
			}

			class_files_temp = class_files;
		}
		
		auto find_it = class_files_temp->find(name);
		if (find_it == class_files_temp->end()) {
			auto _file = new ClassFile(clone_type, this, name);
			class_files_temp->insert(make_pair(name, _file));
			return _file;
		}
		else{
			return find_it->second;
		}
	}


	PathModel::MosesPath* PackageFragment::getMosesPath()
	{
		if (moses)
			return moses;
		if (!fParent)
			return nullptr;

		auto parent_moses = reinterpret_cast<PkgRootMoses*>(fParent->getMosesPath());
		if (!parent_moses)
			return nullptr;
		bool binary;
		if (fLEType == JAR_PACKAGE_FRAGMENT)
		{
			binary = true;
		}
		else
		{
			binary = false;
		}
		moses = new PkgMoses(
			parent_moses->proj,
			parent_moses->root, fName, binary);

		return moses;
	}

	bool PackageFragment::hasChildren()
	{
		LazyLoad();
		if (class_files && class_files->empty() == false) {
			return true;
		}
		else if (compilations && compilations->empty() == false){
			return true;
		}
		else {
			return false;
		}
		
	}

	void PackageFragment::CopyChildren(IPackageFragment* parent)
	{
	}

	void PackageFragment::removeInfo()
	{
		if (class_files) {

			for (auto it : *class_files)
				delete it.second;

			class_files->clear();
		}
		if (compilations) {
			for (auto it : *compilations){
				delete it.second;
			}
			compilations->clear();
		}
		if (innerClassesFiles) {
			auto end = innerClassesFiles->end();
			for (auto it: * innerClassesFiles){
				delete it.second;
			}
			innerClassesFiles->clear();
		}
	}

	PackageFragment::~PackageFragment()
	{
		if (class_files) {
			for (auto it :*class_files)
				delete it.second;
			delete class_files;
		}
		if (compilations) {	
			for (auto it : *compilations){
				delete it.second;
			}
			delete compilations;
		}
		if (innerClassesFiles) {
			for (auto it : *innerClassesFiles){
				delete it.second;
			}
			delete innerClassesFiles;
		}

	}


	void PackageFragment::ProcessRemoveElement(map<wstring, NameLookUpNotify*>& remove_files)
	{
		// 这里这有 K_SOURCE 类型的 root 才会增量改变，K_BINARY 要是改变就全部改变
		auto end = compilations->end();
		
		for (auto it : remove_files) {
			auto __file_name = it.first;
			auto find_it = compilations->find(__file_name);
			if (find_it != end) {
				delete find_it->second;
				compilations->erase(find_it);
			}
		}
	}

	void PackageFragment::SynchronousProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify* >>& optResouces, 
		IPackageFragment* clone)
	{
		// 这里这有 K_SOURCE 类型的 root 才会增量改变，K_BINARY 要是改变就全部改变

		auto real_clone = dynamic_cast<PackageFragment*>(clone);
	    if(!compilations || compilations->empty()){
			real_clone->removeInfo();
			return;
	    }
		if(!real_clone->compilations){
			real_clone->compilations = new  map<wstring, ICompilationUnit*>();
		}

		auto end = compilations->end();
		for (auto it : optResouces)
		{
			ResourceOptKind _the_kind = it.first;
			if (_the_kind == ELEMENT_ADD || _the_kind  == ELEMENT_CHANGE)
			{
				map<wstring, NameLookUpNotify* >& files_map = it.second;
				for (auto it_files_map : files_map) {
					auto __file_name = it_files_map.first;
					auto find_it = compilations->find(__file_name);
					if (find_it != end) {
						
						auto find_in_duplicate = real_clone->compilations->find(__file_name);
						if (find_in_duplicate != real_clone->compilations->end() && find_in_duplicate->second) {
								find_in_duplicate->second->setIsStructureKnown(false);	
						}
						else 
						{
							if (!find_it->second)
								return;
							auto clone_child = reinterpret_cast<ICompilationUnit*>(find_it->second->ShallowCopy(real_clone));
							clone_child->setParent(real_clone);

							real_clone->compilations->insert(make_pair(__file_name, clone_child));
						}
					}
				}
			}
		}
	}

	void PackageFragment::IncrementUpdateProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*>>& optResouces)
	{
		// 这里这有 K_SOURCE 类型的 root 才会增量改变，K_BINARY 要是改变就全部改变	
		if (!compilations) {
			compilations = new  map<wstring, ICompilationUnit*>();
		}
		
		for (auto it : optResouces)
		{
			ResourceOptKind _the_kind = it.first;
			if (_the_kind == ELEMENT_ADD || _the_kind == ELEMENT_CHANGE)
			{
				map<wstring, NameLookUpNotify* >& files_map = it.second;
				for (auto it_files_map : files_map) {
					auto file_name = it_files_map.first;
					auto find_it = compilations->find(file_name);

					if (find_it != compilations->end() && find_it->second) {
						InterlockedIncrement(&(find_it->second->m_iDeclsVersion));
						find_it->second->setIsStructureKnown(false);	
					}
					else {
						auto clone_child = new CompilationUnit(IJavaElementType::COMPILATION_UNIT, this,file_name);
						compilations->insert(make_pair(file_name, clone_child));
					}
				}
			}
			else if (_the_kind == ELEMENT_REMOVE)
			{
				map<wstring, NameLookUpNotify* >& files_map = it.second;
				for (auto it_files_map : files_map) {
					auto file_name = it_files_map.first;
					auto find_it = compilations->find(file_name);
					if (find_it != compilations->end()) {
						delete find_it->second;
						compilations->erase(find_it);
					}
				}
			}
		}
	}

	wstring  PackageFragment::getCompoundName(){
		return  fName;
	}
	IJavaElement* PackageFragment::ShallowCopy(IJavaElement* parent){
		PackageFragment* clone;
		
		 clone  =  new PackageFragment(fName,fParent,fLEType);
		
		clone->fParent = parent;
		clone->MarkClone();
		return clone;
		
	}

	void PackageFragment::addChild(IJavaElement* child)
	{
		assert(child);
		if (!child)
			return;

		auto _type = child->getElementType();
		auto name = child->getElementName();
		switch (_type) {
		case IJavaElementType::CLASS_FILE :
		case IJavaElementType::JAR_CLASS_FILE:
		{
			if(name.find(L"$") != wstring::npos)
			{
				if (!innerClassesFiles)
					innerClassesFiles = new map<wstring, IClassFile*>();
				auto find_it = innerClassesFiles->find(name);
				if(  find_it == innerClassesFiles->end() ) {
					innerClassesFiles->insert(make_pair(name, dynamic_cast<IClassFile*>(child)));
				}
				else
				{
					delete find_it->second;
					find_it->second = dynamic_cast<IClassFile*>(child);
				}
			}
			else
			{
				if (!class_files)
					class_files = new map<wstring, IClassFile*>();
				auto find_it = class_files->find(name);
				if (find_it == class_files->end()) {
					class_files->insert(make_pair(name, dynamic_cast<IClassFile*>(child)));
				}
				else
				{
					delete find_it->second;
					find_it->second = dynamic_cast<IClassFile*>(child);
				}
			}
		}
			break;

		case IJavaElementType::COMPILATION_UNIT:
		case IJavaElementType::JAR_COMPILATION_UNIT:
		{
			if (!compilations)
				compilations = new map<wstring, ICompilationUnit*>();
			auto find_it = compilations->find(name);
			if (find_it == compilations->end()) {
				compilations->insert(make_pair(name, dynamic_cast<ICompilationUnit*>(child)));
			}
			else
			{
				delete find_it->second;
				find_it->second = dynamic_cast<ICompilationUnit*>(child);
			}
		}
			break;

		default:
			break;
		}
	}

	bool PackageFragment::generateInfos(IProgressMonitor* pm)
	{
		if(!CopyChildFromOriginalWhenIsClone())
			computeChildren();
		
		 return true;
	}

	

	std::vector<IJavaElement*>* PackageFragment::getChildren()
	{
		LazyLoad();
		std::vector<IJavaElement*>* _ret = nullptr;
		if (class_files) {
			_ret = new vector<IJavaElement*>();
			for (auto it:*class_files ){
				_ret->push_back(it.second);
			}
			
		}
		if (compilations) {
			if(!_ret)
				_ret = new vector<IJavaElement*>();		
			for (auto it : *compilations){
				_ret->push_back(it.second);
			}	
		}
		return _ret;
	}

	bool PackageFragment::CopyChildFromOriginalWhenIsClone() 
	{
		if (is_Clone && fParent)
		{
			auto pkg_root = dynamic_cast<IPackageFragmentRoot*>(fParent);
			if (!pkg_root)
				return true;

			auto proj = pkg_root->getJavaProject();
			if (!proj)
				return false;

			auto name = proj->getElementName();
			auto pJm = proj->getJavaModel();

			if(!pJm)
			{
				return false;
			}

			auto original_proj = pJm->getJavaProject(name);
			if (!original_proj)
				return true;

			original_proj->beginToOccupy();
			auto original_pkg = (PackageFragment*)original_proj->findPackageFragmentByName(pkg_root->getElementName(), fName);

			if (original_pkg)
			{
				auto _original_children = original_pkg->getInternalCompilationUnits();
				if (_original_children) {
					if (!compilations)
						compilations = new map<wstring, ICompilationUnit*>();
					for (auto it : *_original_children){
						if(!it.second)
						{
							continue;
						}
						auto temp = (ICompilationUnit*)(it.second->ShallowCopy(this));
						compilations->insert(make_pair(it.first, temp));
					}
						
				}
				if(original_pkg->class_files)
				{
					if(!class_files)
						class_files  =new map<wstring, IClassFile*>();
					for (auto it : *(original_pkg->class_files)) {
						if (!it.second)
						{
							continue;
						}
						auto temp = (IClassFile*)(it.second->ShallowCopy(this));
						class_files->insert(make_pair( it.first, temp ));
					}
				}
				if(original_pkg->innerClassesFiles)
				{
					if (!innerClassesFiles)
						innerClassesFiles = new map<wstring, IClassFile*>();
					for (auto it : *(original_pkg->innerClassesFiles)) {
						if (!it.second)
						{
							continue;
						}
						auto temp = (IClassFile*)(it.second->ShallowCopy(this));
						innerClassesFiles->insert(make_pair( it.first, temp ));
					}
				} 
			}	
			original_proj->endToOccupy();
			return true;
		}
		{
			return false;
		}
	}

	
	PackageFragment::PackageFragment(const wstring& name_,
		IJavaElement* owner_,
		IJavaElementType _type)
		: IPackageFragment(owner_,_type, name_)
		, class_files(nullptr), 
		compilations(nullptr),
		innerClassesFiles(nullptr)
	{	
	}

	 void PackageFragment::HelpToDeepCopy(PackageFragment& o)
	{
		if (o.class_files) {
			class_files = new map<wstring, IClassFile*>();
			for (auto it : *(o.class_files)){
				if (!it.second)
				{
					assert(false);
					continue;
				}
				auto temp = reinterpret_cast <IClassFile*>(it.second->DeepCopy(this));
				class_files->insert(make_pair(it.first, temp));
			}
		}
		if (o.compilations) {
			compilations = new map<wstring, ICompilationUnit*>();
			
			for (auto it : (* o.compilations) ){
				if (!it.second)
				{
					assert(false);
					continue;
				}
				auto temp = reinterpret_cast <ICompilationUnit*>(it.second->ShallowCopy(this));
				temp->setParent(this);
				compilations->insert(make_pair(it.first, temp));
			}
		}
		if (o.innerClassesFiles) {
			innerClassesFiles = new map<wstring, IClassFile*>();
			for (auto it : *(o.innerClassesFiles)){
				if (!it.second)
				{
					assert(false);
					continue;
				}
				auto temp = reinterpret_cast<IClassFile*>(it.second->DeepCopy(this));
				innerClassesFiles->insert(make_pair(it.first, temp));
			}
		}
	}

	PackageFragment::PackageFragment(PackageFragment&o)
		: IPackageFragment(o)
		, class_files(nullptr),
		compilations(nullptr),
		innerClassesFiles(nullptr)
	{
		
	}

	IClassFile* PackageFragment::getClassFile(const wstring& name)
	{
		LazyLoad();
		if(name.find('$') == wstring::npos){
			if (class_files){
				auto find_it = class_files->find(name);
				if (find_it != class_files->end()){
					return find_it->second;
				}
			}
		}
		else{
			if (innerClassesFiles){
				auto find_it = innerClassesFiles->find(name);
				if (find_it != innerClassesFiles->end()){
					return find_it->second;
				}
			}
		}
		return nullptr;

	};

	std::vector<IClassFile*> PackageFragment::getClassFiles()
	{	
		LazyLoad();
		std::vector<IClassFile*> ret;
		if (class_files){
			for (auto it : *class_files){
				ret.push_back(it.second);
			}
		}
		return ret;
	};

	ICompilationUnit* PackageFragment::getCompilationUnit(
		const wstring& name)
	{
		LazyLoad();
		if(compilations){
			auto find_it = compilations->find(name);
			if(find_it != compilations->end()){
				return find_it->second;
			}
		}
		return nullptr;
	};

	std::vector<ICompilationUnit*> PackageFragment::getCompilationUnits()
	{
		LazyLoad();
		std::vector<ICompilationUnit*> ret;
		if(compilations){
			for (auto it : *compilations){
				ret.push_back(it.second);
			}
		}
		return ret;
	};

	int PackageFragment::getKind()
	{
		return dynamic_cast<IPackageFragmentRoot*>(fParent)->getKind();
	};

	bool PackageFragment::hasSubpackages()
	{
		return true;
	};

	bool PackageFragment::isDefaultPackage()
	{
		if(fName == DEFAULT_PACKAGE_NAME) {
			return true;
		}
		return false;
	}

	void PackageFragment::computeClassChildren(){
		if (!fParent)
			return;

		auto parent_temp = dynamic_cast<PackageFragmentRoot*>(fParent);
		if (!parent_temp)
			return;
		auto folderPath(parent_temp->resoucePath);
		folderPath.append(boost::replace_all_copy(fName, L".", L"\\"));
		wstring extType = L".class"; //$NON-NLS-1$
		namespace fs = boost::filesystem;
		fs::directory_iterator _end;
		
		if (!fs::is_directory(folderPath)) {
			return;
		}
		for (fs::directory_iterator pos(folderPath); pos != _end; ++pos) {
			fs::path file(*pos);
			if (fs::is_directory(file) == false){
				auto temp_ext = file.extension().wstring();
				boost::to_lower(temp_ext);
				if (temp_ext == extType){				
					 FindOrInsertClassFile(file.stem().wstring());	
				}
			}
		}
	}


	void PackageFragment::computeChildren()
	{
		if (getKind() == IPackageFragmentRoot::K_BINARY) {
			computeClassChildren();
			return;	
		}
		auto pkg_root = dynamic_cast<IPackageFragmentRoot*>(fParent);
		if (!pkg_root)
			return ;

		auto proj = pkg_root->getJavaProject();
		if (!proj)
			return ;

		auto name = proj->getElementName();
		vector<wstring> fileNames;
		auto pJm = proj->getJavaModel();
		if (!pJm)
			return;
		pJm->GetAllFileNamesInPkg(name, fName, fileNames);
		auto _size = fileNames.size();
		for (auto i = 0; i < _size; ++i){
			FindOrInsertCompilationUnit(fileNames[i]);
		}	
	}

}// Close namespace JavaModel block
	

} // Close namespace Jikes block




