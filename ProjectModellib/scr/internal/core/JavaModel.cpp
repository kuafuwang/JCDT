#include "stdafx.h"
#include <string>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <windows.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <ProjectModelLib/ProjectModel/ProjectSolution/SolutionModel.h>
#include <PathModelLib/ClassPath/BaseCompilerEnvManager.h>
#include <ProjectModelLib/Builder/NameEnvironment.h>
#include <boost/make_shared.hpp>
#include <JavaModelLib/compiler/BuilderMgr.h>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>

#include <JCDT_Lib/internal/impl/FileHelper.h>
#include <boost/algorithm/string.hpp>
#include <CodeAssistLib/impl/CodeAssistMgr.h>
#include <ProjectModelLib/ProjectModel/JCIDE/ProjectModel.h>
#include <PathModelLib/DeltaPath/NameLookUpNotify.h>
#include <PathModelLib/JarPath/JarPathManager.h>
#include <PathModelLib/SourcePath/UnitManager.h>
#include <PathModelLib/ClassPath/ClasspathEntry.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	boost::shared_ptr<OriginalJavaProject> JavaModel::NO_PROJECT;

	IJavaElement* JavaModel::DeepCopy(IJavaElement*)
	{
		assert(false);
		return nullptr;
	}

	IJavaElement* JavaModel::ShallowCopy(IJavaElement* parent)
	{
		assert(false);
		return nullptr;
	}

	bool JavaModel::hasChildren()
	{
		LazyLoad();
		return (childrens.empty() == false);
	}

	std::vector<IJavaElement*>* JavaModel::getChildren(){
		return nullptr;
	}


	bool JavaModel::removeProject(const wstring& proj)
	{
		writeLock(rw_mutex);
		auto find_it = childrens.find(proj);
		if (find_it != childrens.end()) {
		    find_it->second = nullptr;
			childrens.erase(find_it);
			EnterCriticalSection(cs_generate);
			childrenCache.erase(proj);
			LeaveCriticalSection(cs_generate);

	
			return true;
		}
		return false;
	}

	void JavaModel::removeInfo()
	{
		writeLock(rw_mutex);
		
		childrens.clear();
		EnterCriticalSection(cs_generate);
		childrenCache.clear();
		LeaveCriticalSection(cs_generate);

	}

	

	void JavaModel::addChild(IJavaElement* child){
		auto temp = dynamic_cast<OriginalJavaProject*>(child);
		if(temp){
			addChild(boost::shared_ptr<OriginalJavaProject>(temp));
		}
		else{
			assert(false);
		}
	}

	void JavaModel::addChild(boost::shared_ptr<OriginalJavaProject>& child)
	{
		if (child) {
			writeLock(rw_mutex);
			auto proj_name = child->getElementName();
			auto find_it = childrens.find(proj_name);
			if(find_it != childrens.end())
				find_it->second = child;
			else
				childrens.insert(make_pair(proj_name, child));
		}
	}

	FileTypeInJavaProject JavaModel::WhatItIs(const wstring& _path)
	{
		readLock(rw_mutex);

		for(const auto& it : childrens)
		{
			auto  pos = it.second->getLayerPos(_path);
			if(pos)
			{
				auto _type = pos->projectPos._type;
				delete  pos;
				return _type;
			}
			
		}
		return FileTypeInJavaProject::NO_MY_GUY;

	}

	bool JavaModel::generateInfos(IProgressMonitor* pm)
	{
		writeLock(rw_mutex);
		
		map<std::wstring, ProjectModel*>&  projects = m_pOwner->map_ProjectModels;
		auto it = projects.begin();
		auto end = projects.end();
		std::wstring binDirPath;
		for (; it != end; ++it){
			ProjectModel* proj = it->second;
			if(proj && proj->IsExist()){
				auto temp = boost::make_shared<OriginalJavaProject>(
					this, 
					it->first,
					proj->GetUnixStyleProjectPath(),jar_manager_);

				childrens[it->first] = temp;
	
				temp->SetOutputDir(proj->GetJikesParseProjectPath());


				BuildPathConfigData& buildPaths = proj->getBuildPathConfigData();
				const std::vector<IClasspathEntry*> & classpathEntries = buildPaths.classpathEntry;
				size_t entr_size = classpathEntries.size();
				for (size_t i = 0; i < entr_size; ++i) {
					IClasspathEntry* pEntry = classpathEntries[i];
					assert(pEntry);
					if (!pEntry)
						continue;
					switch (pEntry->getEntryKind()) {
					case IClasspathEntry::CPE_LIBRARY: {
						auto lib_entry = reinterpret_cast<LibraryClasspathEntry*>(pEntry);
						auto _path = lib_entry->getAbolutePath().wstring();
						jar_manager_.addJarPackageRootReference(_path);
					}
					break;

					default:
						break;
					}
				}



				temp->buildStructure(false, nullptr);
			}
			
		}
		return true;
	}

	

	void JavaModel::GetProjectsInClassPaths(set<wstring>& project_expand_class_path, const wstring& proj)
	{
	
		std::map<std::wstring, ProjectModel*>&  projects = m_pOwner->map_ProjectModels;
		auto find_it = projects.find(proj);
		if(find_it != projects.end()){
			NameEnvironment::GetProjectsInClassPaths(projects, find_it->second, project_expand_class_path);
		}

	}

	

	JavaModel::~JavaModel()
	{

		if (builder_mgr_) {
			builder_mgr_->pm->setCanceled(true);
		}
		if (code_assist_mgr_) {
			code_assist_mgr_->pm->setCanceled(true);
		}

		
		DeleteCriticalSection(cs_code_assit_stack);
		delete cs_code_assit_stack;

		DeleteCriticalSection(cs_generate);
		delete cs_generate;

		delete cs_modifi_doc;

		build_cond->notify_all();
		code_assist_cond->notify_all();
		Sleep(20);

		delete  builder_mgr_;
		delete code_assist_mgr_;

	
		delete build_cond;
		delete build_mu;

		delete code_assist_cond;
		delete code_assist_mu;
		
	}

	JavaModel::JavaModel(SolutionModel* _owner, PathModel::BaseCompilerEnvManager* env_mgr)
		: IJavaModel(L"JCIDE"),m_pOwner(_owner),jar_manager_(*env_mgr->jar_manager_),m_CompilerEnvMgr(env_mgr)
	{
		cs_generate = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount(cs_generate, 2000);

		cs_modifi_doc = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount(cs_modifi_doc, 2000);
		chche_end = childrenCache.end();
		modified_doc_end = modifiedDocsCache.end();

	 
		cs_code_assit_stack = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount(cs_code_assit_stack, 2000);


		 build_cond = new boost::condition_variable_any();
		 build_mu = new boost::mutex();
		
		 code_assist_cond = new boost::condition_variable_any();
		 code_assist_mu = new boost::mutex();
		
		 
	}
	
	
	boost::shared_ptr<OriginalJavaProject>& JavaModel::getJavaProject(const std::wstring& name)
	{
		EnterCriticalSection(cs_generate);
		auto find_in_chache = childrenCache.find(name);
	    if(chche_end != find_in_chache)
	    {
			LeaveCriticalSection(cs_generate);
			return find_in_chache.value();
			
	    }
		readLock(rw_mutex);
		auto find_it = childrens.find(name);
		if (find_it != childrens.end()) {
			childrenCache.insert(find_it->first, find_it->second);

			boost::shared_ptr<OriginalJavaProject>& temp = find_it->second;
			LeaveCriticalSection(cs_generate);
			return temp;
		}
		else {
			LeaveCriticalSection(cs_generate);
			return NO_PROJECT;
		}
	}

	void JavaModel::getExpandClasspath(JikesClassPaths & paths, const wstring & projectName)
	{
		
		std::map<std::wstring, ProjectModel*>&  projects = m_pOwner->map_ProjectModels;
		auto find_it = projects.find(projectName);
		if (find_it != projects.end()) {
			const auto& containerCtrl = m_CompilerEnvMgr->getContainerCtrl(find_it->second->GetParent()->GetChipPack());
			set< ProjectModel*> visitedProjects;
			NameEnvironment::computeExpanBuildpathForJikesLib(
				projects,	
				containerCtrl,
				find_it->second,
				visitedProjects,
				paths);
		}
	}

	

	void JavaModel::getClasspathButNoPorjcetEntry(JikesClassPaths& paths, const wstring& projectName) const
	{
		std::map<std::wstring, ProjectModel*>&  projects = m_pOwner->map_ProjectModels;
		auto find_it = projects.find(projectName);
		if (find_it != projects.end()) {
			const auto& containerCtrl = m_CompilerEnvMgr->getContainerCtrl(find_it->second->GetParent()->GetChipPack());
			NameEnvironment::getClasspathButNoPorjcetEntry(find_it->second, containerCtrl, paths);
		}
	}
	 void JavaModel::getClasspathButNoPorjcetEntryForSearchScope( 
		 const wstring& projectName,
		const std::map<int, bool>& limits,
		map<wstring, wstring> & result)
	{
		std::map<std::wstring, ProjectModel*>&  projects = m_pOwner->map_ProjectModels;
		auto find_it = projects.find(projectName);
		if (find_it != projects.end()) {
			const auto& containerCtrl = m_CompilerEnvMgr->getContainerCtrl(find_it->second->GetParent()->GetChipPack());
			NameEnvironment::getClasspathButNoPorjcetEntryForSearchScope(find_it->second, containerCtrl, limits, result);
		}
	}

	map<wstring,boost::shared_ptr<OriginalJavaProject>>* JavaModel::getProjectsToBuild()
	{
		readLock(rw_mutex);
		map<wstring, boost::shared_ptr<OriginalJavaProject>>* projects = nullptr;
		auto it = childrens.begin();
		auto end_childrens = childrens.end();
		for (; it != end_childrens; ++it) {
			if(it->second->IsNeedBuild()){
				if (!projects) {
					projects = new map<wstring, boost::shared_ptr<OriginalJavaProject>>();
				}
				projects->insert( make_pair(it->first,it->second) );
			}
		}
		return projects;
	}

	void JavaModel::PushResourceOpt(const wstring& proj, 
		vector<NameLookUpNotify*>& _notisfys, PropagtionCode _code)
	{
		auto theProj = getJavaProject(proj);
		if (theProj)
		{
			theProj->PushResourceOpt(_notisfys);
			bool _include_project = (_code == REBUILD_PROJECT) ? true: false;
			PropagationProjectChange(proj, _include_project);
			UpdateBuildStamp();
		}
		else
		{
			for (size_t i = 0; i < _notisfys.size(); ++i){
				delete _notisfys[i];
			}
		}
	}

	void JavaModel::PushResourceOpt(const wstring& proj,
		NameLookUpNotify* _notisfy, PropagtionCode _code)
	{
		auto theProj = getJavaProject(proj);
		if(theProj)
		{
			theProj->PushResourceOpt(_notisfy);
			bool _include_project = (_code == REBUILD_PROJECT) ? true : false;
			PropagationProjectChange(proj, _include_project);
 			UpdateBuildStamp();

		}
		else
		{
			delete _notisfy;
		}
	}

	void JavaModel::getAllProjectsName(set<wstring>& projectNames)
	{
		readLock(rw_mutex);
	
		auto it = childrens.begin();
		auto end_childrens = childrens.end();
		for (; it != end_childrens; ++it) {
			projectNames.insert(it->first);
		}
		
	}

	

	set<wstring> JavaModel::getProjectsRelatedTo(const wstring projectName)
	{
		set<wstring> _ret;
		_ret = getProjectsThatNeeds(projectName);
		
		auto sons = getProjectsThatNeeds(projectName);
		for(const auto& son : sons){
			_ret.insert(son);
		}
		return _ret;
	}

	set<wstring> JavaModel::getProjectsThatNeeds(const wstring projectName)
	{
		auto _size = m_pOwner->vecBuildStep.size();
		if (!_size) {

			return {};
		}
		auto vecBuildStep(m_pOwner->vecBuildStep);


		set<wstring> project_expand_class_path;
		set<wstring> depended_projects;


		auto end_project_expand_class_path = project_expand_class_path.end();
		set<wstring> next_round;
		next_round.insert(projectName);
		// 这里 A依赖 B , 命名A为 啃老儿子, B为 被啃的老爸
		auto end_vecBuildStep = vecBuildStep.end();
		do {

			next_round.swap(depended_projects);/// 记录啃老儿子 的 啃老儿子
			next_round.clear();
			auto depended_projects_end = depended_projects.end();
			for (auto it = vecBuildStep.begin(); it != end_vecBuildStep; ++it) {
				auto find_it = depended_projects.find(it->second);
				if (find_it != depended_projects_end) {
					if (project_expand_class_path.find(it->first) == end_project_expand_class_path) {
						next_round.insert(it->first);// 记录啃老儿子
						project_expand_class_path.insert(it->first);
						end_project_expand_class_path = project_expand_class_path.end();
					}
				}
			}
		} while (next_round.empty() == false);// 看看是否还有啃老儿子

		return project_expand_class_path;
	}

	set<wstring> JavaModel::getProjectsNeededBy(const wstring projectName)
	{
		auto _size = m_pOwner->vecBuildStep.size();
		if (!_size) {

			return {};
		}
		auto vecBuildStep(m_pOwner->vecBuildStep);


		set<wstring> project_expand_class_path;
		set<wstring> depended_projects;


		auto end_project_expand_class_path = project_expand_class_path.end();
		set<wstring> next_round;
		next_round.insert(projectName);
		// 这里 A依赖 B , 命名A为 啃老族, B为 老爸
		auto end_vecBuildStep = vecBuildStep.end();
		do {

			next_round.swap(depended_projects);// 记录老爸的老爸
			next_round.clear();
			auto depended_projects_end = depended_projects.end();
			for (auto it = vecBuildStep.begin(); it != end_vecBuildStep; ++it) {
				auto find_it = depended_projects.find(it->second);
				if (find_it != depended_projects_end) {
					if (project_expand_class_path.find(it->first) == end_project_expand_class_path) {
						next_round.insert(it->second);// 记录老爸
						project_expand_class_path.insert(it->first);
						end_project_expand_class_path = project_expand_class_path.end();
					}
				}
			}
		} while (next_round.empty() == false);// 看看是否还有老爸

		return project_expand_class_path;
	}


	void JavaModel::GetAllFileNamesInPkg(const wstring& project, const wstring& pkgName, 
		vector<wstring>& names) const
	{
		auto find_proj = m_pOwner->GetProject(project);
		if (!find_proj)
			return;
		try {
			find_proj->GetAllClassNames(pkgName, names);
		}
		catch(ProjectError&)
		{
			
		}
	}

	void JavaModel::UpdateCodeAssistInfo(CodeAssist::AssistInfo* _pos) 
	{
		assert(code_assist_mgr_);
		if (code_assist_mgr_ == nullptr)
			return;
		code_assist_mgr_->UpdateInfo(_pos);
		code_assist_cond->notify_all();

	}

	void JavaModel::PropagationProjectChange(const wstring& proj, bool include_proj)
	{
		auto project_expand_class_path = getProjectsNeededBy(proj);

		if (!include_proj) {
			project_expand_class_path.erase(proj);
		}
		else{
			project_expand_class_path.insert(proj);
		}
		auto it = project_expand_class_path.begin();
		auto end = project_expand_class_path.end();

		readLock(rw_mutex);
		auto end_childrens =  childrens.end();
		for (; it != end; ++it){
			auto find_it = childrens.find(*it);
			if(find_it != end_childrens && find_it->second){
				auto namelookup = find_it->second->getNameLookupWhenNoLookupDonotGenerateIt();
				if (namelookup)
					namelookup->UpdateNameLookupStamp();
				find_it->second->MarkBraveCompilerRebuild();
			}
		}
	 
	}


	void buildProc(BuilderMgr* mgr){
		mgr->BuildProc();
	}
	
	void CodeAssistProc(CodeAssist::CodeAssistMgr* mgr) {
		mgr->DispathProc();
	}

	void JavaModel::InintJavaModel(AssistFileHelper* asssis_helper,  CodeAssist::CodeAssistMgr* assist, BuilderMgr* builder)
	{
		FileHelper::getInstance()->RegisterAssist(asssis_helper);

		builder_mgr_ = builder;
		code_assist_mgr_ = assist;

		buildStructure(false,nullptr);

		boost::thread temp(buildProc, builder_mgr_);
		temp.detach();
		RebuildAllProjects();

		boost::thread temp2(CodeAssistProc, code_assist_mgr_);
		temp2.detach();

	}

	void JavaModel::RebuildAllProjects()
	{
		assert(builder_mgr_);
		readLock(rw_mutex);
		auto it = childrens.begin();
		auto end_childrens = childrens.end();
		for (; it != end_childrens; ++it) {

			if (!it->second)
				continue;

			it->second->MarkBraveCompilerRebuild();
		}
		UpdateBuildStamp();
	}

	LayerPosInJavaModel* JavaModel::getLayerPos(const wstring& _path)
	{
		
		readLock(rw_mutex);
		auto it = childrens.begin();
		auto end_childrens = childrens.end();
		for (; it != end_childrens; ++it) {
			auto  _pos = it->second->getLayerPos(_path);
			if (_pos != nullptr)
				return _pos;

		}
		return nullptr;
	}

	JavaModel* JavaModel::getJavaModel()
	{
		return this;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


