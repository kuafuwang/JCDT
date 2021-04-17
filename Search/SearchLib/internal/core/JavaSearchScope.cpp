#include "stdafx.h"
#include "JavaSearchScope.h"

#include <JavaModelLib/internal/core/JavaModel.h>
#include <ProjectModelLib/Builder/NameEnvironment.h>
#include "JavaSearchScopeData.h"
using namespace std;
namespace Jikes { // Open namespace Jikes block


	
namespace Search{

	 const wchar_t* JavaSearchScope::NO_BELONG_TO_ANY_PROJECT = L"{ 22B60A57 - 0756 - 40c6 - BB7B - 67310583EE4C }";

	void JavaSearchScope::setLimitTo(int limitType, bool supported)
	{
		limits[limitType] = supported;
	}

	const std::vector<GroupInfo*>& JavaSearchScope::getPaths()
	{
		return _paths;
	}

	JavaSearchScope::JavaSearchScope()
	{
		limits[NameEnvironment::IN_SOURCES] = true;
		limits[NameEnvironment::IN_REQURED_PROJECTS] = true;
		limits[NameEnvironment::JRE_LIBRAIRY] = false;
		limits[NameEnvironment::APPLICATION_LIB] = false;
		limits[NameEnvironment::USER_DEFINED_CONTAINER] = false;
	}

	JavaSearchScope::~JavaSearchScope()
	{
		for(auto it : _paths)
		{
			delete it;
		}
	}

	bool JavaSearchScope::IsLimitTo(int limit)
	{
		auto find_it = limits.find(limit);
		if(find_it != limits.end())
		{
			return find_it->second;
		}
		return true;
	}

	void JavaSearchScope::setLimitTo(std::map<int, bool>& _limits)
	{
		for(auto it : _limits)
		{
			limits[it.first] = it.second;
		}
	}

	std::map<int, bool>& JavaSearchScope::getLimits(){
		return limits;
	}

	bool JavaSearchScope::encloses(const wstring& resourcePath)
	{
		return true;
	}

	void JavaSearchScope::add(JavaModel::JavaModel* pJm)
	{
		if (!pJm)
			return;

		set< wstring  > projects;
		pJm->getAllProjectsName(projects);
		ProcessProjectPath(projects,pJm);
	}

	void JavaSearchScope::add(const wstring& project, JavaModel::JavaModel* pJm){
		if (!pJm)
			return;
		set< wstring > javaProjects;
		if (limits[NameEnvironment::IN_REQURED_PROJECTS]){
			// 获取相关javaproject
			 javaProjects = pJm->getProjectsRelatedTo(project);
		}
		javaProjects.insert(project);
		ProcessProjectPath(javaProjects,pJm);
	}
	bool JavaSearchScope::IsDuplicate(const wstring& fileCandidate)
	{
		auto data = _record.data();
		auto size = _record.size();
		
		for (auto i = 0 ; i < size; ++ i)
		{
			auto it = data[i];
			if (it.second)
			{
				if (it.first.find(fileCandidate) != wstring::npos)
				{
					return true;
				}
			}
			else
			{
				if (it.first == fileCandidate)
				{
					return true;
				}
			}
		}
		return false;
	}

	void JavaSearchScope::ProcessProjectPath(std::set<wstring>& projects, JavaModel::JavaModel* pJm)
	{
		if (!pJm)
			return;
		for(auto it : projects)
		{
			map< wstring, wstring> collect_paths;
			pJm->getClasspathButNoPorjcetEntryForSearchScope(it, limits, collect_paths);
		
			map< wstring, wstring> _group_paths;
			for(auto _path_candidate : collect_paths){
				if (IsDuplicate(_path_candidate.second)) {
					continue;
				}
				_group_paths.insert( _path_candidate);
				_record.push_back(std::make_pair(_path_candidate.second, true));
			}

			if (_group_paths.empty())
				continue;

			auto _group = FindGroupOrInGroup(it);
			for(auto _group_path : _group_paths){
				_group->items.emplace_back(new PathInfo(_group_path.first, _group_path.second, true));
			}
		}
	}

	GroupInfo* JavaSearchScope::FindGroupOrInGroup(const wstring& groupName){

		GroupInfo* group_info = nullptr;
		for (size_t i = 0, size = _paths.size(); i < size; ++i)
		{
			auto group = _paths[i];
			if (group->groupName == groupName)
			{
				group_info = group;
				break;
			}
		}
		if (!group_info) {
			group_info = new GroupInfo(groupName);
			_paths.push_back(group_info);
		}
		return group_info;
	}

	void JavaSearchScope::add(GroupInfo& files)
	{

	}

	void JavaSearchScope::add(std::vector<wstring>& files)
	{
		if (!files.size())
			return;


	   auto defaut_group = FindGroupOrInGroup(NO_BELONG_TO_ANY_PROJECT);
	   auto  items = defaut_group->items;
	   
	   auto filesData = files.data();
	   for(size_t i = 0 , size = files.size(); i < size; ++i)
	   {
		   bool bFind = false;
		   auto file_candidate =  filesData[i];
		   if(IsDuplicate(file_candidate)){
			   continue;
		   }
		  items.emplace_back( new PathInfo(file_candidate, file_candidate, false) );
		  _record.push_back ( std::make_pair(file_candidate, false) );
	   }
	}
}


} // Close namespace Jikes block



