#include "stdafx.h"
#include <JavaModelLib/internal/core/BaseProject.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/IPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <PathModelLib/DeltaPath/ResouceOptStack.h>
#include  <map>
#include <set>
using std::map;
using std::set;
namespace Jikes { // Open namespace Jikes block
namespace JavaModel{
	bool BaseProject::equals(IJavaElement* o)
	{
	
			if (this == o)
				return true;
			if (!o)
				return  false;

			if(o->fLEType != JAVA_PROJECT)
			{
				return false;
			}

			return fName == o->getElementName();
		

	}

	wstring BaseProject::getPath()
	{
		return unix_style_project_path;
	}

	std::vector<IJavaElement*>* BaseProject::getChildren()
	{
		LazyLoad();
		if(!all_children.empty()){
			auto _ret = new std::vector<IJavaElement*>();
			for (auto it : all_children)
				_ret->push_back(it.second);	
			return _ret;
		}
		return nullptr;
	}
	bool BaseProject::hasChildren()
	{
		LazyLoad();
		return all_children.empty() == false;
	}
	void BaseProject::removeInfo()
	{
		for (auto it : all_children){
			delete it.second;
		}
		source_children.clear();
		all_children.clear();
	}
	void BaseProject::calculateSourceEntry()
	{
		source_children.clear();
		for (auto it :all_children){
			if(!it.second)
			{
				continue;
			}
			if (it.second->getKind() != IPackageFragmentRoot::K_SOURCE)
				continue;
			source_children.insert(make_pair(it.first, it.second));
		}
	}
	void BaseProject::beginToOccupy()
	{
		if (!usage_reference) {
			auto cs_generate = getJavaModel()->cs_generate;
			EnterCriticalSection(cs_generate);
			if (!usage_reference) {
				auto  temp = new CRITICAL_SECTION();
				InitializeCriticalSectionAndSpinCount(temp, 2000);
				usage_reference = temp;
			}
			LeaveCriticalSection(cs_generate);
		}

		EnterCriticalSection(usage_reference);
	}
	void BaseProject::endToOccupy()
	{
		if(usage_reference){
			LeaveCriticalSection(usage_reference);
		}
	}
	JavaModel* BaseProject::getJavaModel()
	{	
		return dynamic_cast<JavaModel*>(fParent);	
	}
	IPackageFragment* BaseProject::findPackageFragmentByName(const std::wstring& simpleRootName,
		const std::wstring& pkgName)
	{
		auto root = findPackageFragmentRootByName(simpleRootName);
		if (root)
			return root->getPackageFragment(pkgName);
		return nullptr;
	}
	IPackageFragmentRoot* BaseProject::findPackageFragmentRootByName(const std::wstring& simpleName)
	{
		auto find_it = all_children.find(simpleName);
		if(find_it != all_children.end())
			return	 find_it->second;
		return nullptr;
		
	}
	BaseProject::~BaseProject(){
		for (auto it : all_children) {
			delete it.second;
		}
		all_children.clear();

		if (usage_reference) {
			DeleteCriticalSection(usage_reference);
			delete usage_reference;
			usage_reference = nullptr;
		}
		if (opt_stack)
		{
			delete opt_stack;
			opt_stack = nullptr;
		}
			
		if (opt_cs) {
			DeleteCriticalSection(opt_cs);
			delete opt_cs;
			opt_cs = nullptr;
		}
	}
	void BaseProject::addChild(IJavaElement* child){
		if(!child)
		{
			return;
		}
		auto root = dynamic_cast<IPackageFragmentRoot*>(child);
		if(root){
			all_children.insert( make_pair( root->getElementName(), root));
			if (root->getKind() == IPackageFragmentRoot::K_SOURCE){
				source_children.insert(make_pair(root->getElementName(), root));
			}
		}
	}
	IPackageFragment* BaseProject::getPackageFrament(const wstring& root_name, const wstring& pkg_name)
	{
		LazyLoad();
		auto find_it = all_children.find(root_name);
		if(find_it != all_children.end()){
			auto root = find_it->second;
			if (!root)
				return nullptr;

			auto pkg =  root->getPackageFragment(pkg_name);
			return pkg;
		}
		else{
			return nullptr;
		}
	}
	BaseProject::BaseProject(IJavaModel* p, const wstring& projectPath, const wstring& name)
		:IJavaProject(p, name),
		usage_reference(nullptr),unix_style_project_path(projectPath)
	{
		opt_stack = new PathModel::ResouceOptStack();
		opt_cs = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount(opt_cs, 2000);
	}
	BaseProject::BaseProject(BaseProject&o)
		: IJavaProject(o), usage_reference(nullptr),unix_style_project_path(o.unix_style_project_path)
	{
		opt_stack = new PathModel::ResouceOptStack();
		opt_cs = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount(opt_cs, 2000);
		
	}
	const map<wstring, IPackageFragmentRoot*>& BaseProject::getInternalSourcChildren()
	{
		LazyLoad();
		return source_children;
	}

	void BaseProject::HelpToDeepCopy(BaseProject& o)
	{
		for (auto it : o.all_children) 
		{

			if (!it.second)
				continue;

			auto temp = (IPackageFragmentRoot*)it.second->ShallowCopy(this);
			temp->setParent(this);
			all_children.insert(std::make_pair(it.first, temp));

		}
	}

	const map<wstring, IPackageFragmentRoot*>& BaseProject::getInternalChildren()
	{
		LazyLoad();
		return all_children;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




