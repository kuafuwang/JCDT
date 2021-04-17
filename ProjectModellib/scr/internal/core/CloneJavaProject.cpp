#include "stdafx.h"
#include <JavaModelLib/internal/core/CloneJavaProject.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/IPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>

#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>

using namespace Jikes::PathModel;

namespace Jikes { // Open namespace Jikes block
	namespace JavaModel {
		void  CloneJavaProject::SynchronousProcessRoot(OriginalJavaProject* orignal,
			NotifyResult::RootT& _roots, set<ResourceOptKind>& recordAffectLevel)
		{	
			if (!orignal)
				return;
			// 这里复制处理add 和  change的拷贝
			auto end_children = orignal->all_children.end();
		
			for (auto _outer_it : _roots) {
				auto __the_kind = _outer_it.first;
				if(__the_kind == ELEMENT_CHANGE || __the_kind == ELEMENT_ADD)
				{
					auto __roots = _outer_it.second;
					if(!__roots.empty())
						recordAffectLevel.insert(__the_kind);
					for (auto it : __roots) {
						auto find_it = orignal->all_children.find(it.first);
						if (find_it != end_children){
							auto old_pkg_root = all_children.find(it.first);
							if (old_pkg_root != all_children.end()) {
								if (old_pkg_root->second   && old_pkg_root->second->isStructureKnown()) {
									old_pkg_root->second->setIsStructureKnown(false);
								}	
								
							}
							else {
								if(find_it->second)
								{
									auto temp = (IPackageFragmentRoot*)find_it->second->ShallowCopy(this);
									temp->setParent(this);
									all_children[it.first] = temp;
								}			
							}
						}
						else{
							auto old_pkg_root = all_children.find(it.first);
							if (old_pkg_root != all_children.end()) {
								all_children.erase((old_pkg_root));
							}
						}
					}
				}
			}

			calculateSourceEntry();

		}

		void CloneJavaProject::SynchronousProcessPkg(
			OriginalJavaProject* orignal,
			NotifyResult::PkgT& _pkgs,
			set<ResourceOptKind>& recordAffectLevel)
		{
			for (auto it : _pkgs)
			{
				recordAffectLevel.insert(ELEMENT_CHANGE);

				auto pkg_root_name = it.first;
				auto find_it_original = orignal->findPackageFragmentRootByName(pkg_root_name);
				if (find_it_original){
					auto find_it_clone = findPackageFragmentRootByName(pkg_root_name);
					if (find_it_clone){
						auto& _files = it.second;
						find_it_original->SynchronousProcess(_files, find_it_clone);
					}
				}
				else{
					auto find_it = all_children.find(pkg_root_name);
					if (find_it != all_children.end()){
						delete find_it->second;
						all_children.erase(find_it);
					}			 
				}
			}
		}
		void CloneJavaProject::SynchronousProcessFiles(OriginalJavaProject* orignal,
			NotifyResult::FileT& _files)
		{
			
			for (auto it : _files) {
				auto pkg_root_name = it.first;
			
				for (auto it_pkg : it.second) {

					auto pkg_name = it_pkg.first;	
					auto root = findPackageFragmentRootByName(pkg_root_name);
					if (!root) { continue; }
					auto find_it_clone = root->getPackageFragment(pkg_name);

					if (find_it_clone){
						auto  find_it_original = orignal->findPackageFragmentByName(pkg_root_name, pkg_name);
						if (find_it_original){
							if (it_pkg.second.empty())
								continue;
							find_it_original->SynchronousProcess(it_pkg.second, find_it_clone);		
						}
						else {
							root->ProcessRemoveElement(pkg_name);
						}
					}
				}
			}
		}
		void CloneJavaProject::ProcessRemoveRoot(NotifyResult::RootT& _roots, set<ResourceOptKind>& recordAffectLevel)
		{
			auto remove_root_find = _roots.find(ELEMENT_REMOVE);
			if (remove_root_find != _roots.end()) {	

				if (remove_root_find->second.empty())
					return;

				recordAffectLevel.insert(ELEMENT_REMOVE);
				for (auto it : remove_root_find->second)
				{

					auto find_it = all_children.find(it.first);
					if (find_it != all_children.end()) {
						delete find_it->second;
						all_children.erase(find_it);
					}
				}
				calculateSourceEntry();
			}
		}
		void CloneJavaProject::ProcessRemovePkg(NotifyResult::PkgT& _pkgs,
			set<ResourceOptKind>& recordAffectLevel)
		{
			for (auto it : _pkgs) {
				auto reomve_kind_find = it.second.find(ELEMENT_REMOVE);
				if (reomve_kind_find != it.second.end()) {
					recordAffectLevel.insert(ELEMENT_REMOVE);
					auto pkg_root_name = it.first;
					auto find_it_original = findPackageFragmentRootByName(pkg_root_name);
					if (find_it_original) {
						find_it_original->ProcessRemoveElement(reomve_kind_find->second);
						
					}
				}
			}
		}


		void CloneJavaProject::ProcessRemoveFiles(NotifyResult::FileT& _files)
		{
			
			for (auto it :_files) {
				auto pkg_root_name = it.first;

				auto end_it_pkg = it.second.end();
				for (auto it_pkg : it.second)
				{
					auto reomve_kind_find = it_pkg.second.find(ELEMENT_REMOVE);
					if (reomve_kind_find != it_pkg.second.end()) {

						auto pkg_name = it_pkg.first;
						auto  find_it_original = findPackageFragmentByName(pkg_root_name, pkg_name);
						if (find_it_original)
						{
							if (reomve_kind_find->second.empty())
								continue;

							find_it_original->ProcessRemoveElement(reomve_kind_find->second);
						}
					}
				}
			}
		}


		void CloneJavaProject::CloneAcceptNotify(vector<PathModel::NameLookUpNotify*>& _notifys){
			EnterCriticalSection(opt_cs);
			opt_stack->PushNotify(_notifys);
			LeaveCriticalSection(opt_cs);
		}

		bool CloneJavaProject::generateInfos(IProgressMonitor* pm)
		{
			auto original = getJavaModel()->getJavaProject(fName);
			if (!original)
				return false;
			original->beginToOccupy();
			auto end = original->all_children.end();
			for (auto it = original->all_children.begin(); it != end; ++it) {
				auto temp = (IPackageFragmentRoot*)it->second->ShallowCopy(this);
				if (!temp)
					continue;
				temp->setParent(this);
				all_children.insert(make_pair(it->first, temp));
			}

			original->endToOccupy();
			
			return true;
		}

		void CloneJavaProject::removeInfo()
		{
			opt_stack->Clear();
			BaseProject::removeInfo();
		}

		IJavaElement* CloneJavaProject::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<CloneJavaProject*>( ShallowCopy(parent) );
			if(clone)
				clone->HelpToDeepCopy(*this);

			return clone;
		}

		IJavaElement* CloneJavaProject::ShallowCopy(IJavaElement* parent)
		{
			auto pJm = reinterpret_cast<JavaModel*>(fParent);
			if (!pJm)
				return nullptr;
			auto original = pJm->getJavaProject(fName);
			if (!original) {
				return nullptr;
			}
			original->beginToOccupy();

			auto clone = reinterpret_cast<CloneJavaProject*>(original->ShallowCopy(parent));
			original->endToOccupy();
			return clone;
		}

		CloneJavaProject::CloneJavaProject(IJavaModel* p,  const wstring& projectPath, const wstring& name)
			: BaseProject(p, projectPath, name)
		{
			is_Clone = true;
		}

		CloneJavaProject::CloneJavaProject(OriginalJavaProject& o)
			: BaseProject(o)
		{
			is_Clone = true;
		}

		CloneJavaProject::~CloneJavaProject()
		{
		}

		void CloneJavaProject::SynchronousClone(
			boost::shared_ptr<OriginalJavaProject>& original, 
			map<ModifyLevel, set<ResourceOptKind> >& recordAffectLevel)
		{
			NotifyResult* _notifys;
			 EnterCriticalSection(opt_cs);
			if (opt_stack->empty()) {
				LeaveCriticalSection(opt_cs);
				return;
			}
			vector<NameLookUpNotify*> statck;
			statck.swap(opt_stack->stack);
			LeaveCriticalSection(opt_cs);

			_notifys = PathModel::ResouceOptStack::GetClassifyNotify(statck, false);
			if (!_notifys || !original)
				return;

			for (auto it : statck)
			{
				delete it;
			}
			original->beginToOccupy();
			if(_notifys->act == REBUILD){
				original->endToOccupy();
				makeConsistent(nullptr);
				recordAffectLevel[PROJECT].insert(ELEMENT_CHANGE);
				delete _notifys;
				return;
			}
			auto temp_original = original.get();
			SynchronousProcessRoot(temp_original, _notifys->_roots, recordAffectLevel[PKG_ROOT]);
			SynchronousProcessPkg(temp_original, _notifys->_pkgs, recordAffectLevel[PKG]);
			SynchronousProcessFiles(temp_original, _notifys->_files);
			original->endToOccupy();;//这一部分不需要original 工程的数据，所以复制工作结束可以释放

			// 这里负责remove部分 ,
			ProcessRemoveRoot(_notifys->_roots, recordAffectLevel[PKG_ROOT]);
			ProcessRemovePkg(_notifys->_pkgs, recordAffectLevel[PKG]);
			ProcessRemoveFiles(_notifys->_files);
			delete _notifys;

		}
		/*CloneJavaProject::~CloneJavaProject() {	
			
	   }*/

	} // Close namespace JavaModel block

} // Close namespace Jikes block

