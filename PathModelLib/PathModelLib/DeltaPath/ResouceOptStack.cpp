#include "stdafx.h"
#include "ResouceOptStack.h"
#include <boost/algorithm/string.hpp>
using namespace std;
namespace Jikes { // Open namespace Jikes block
	namespace PathModel {
		typedef std::vector<NameLookUpNotify*>  Classify1th;
		typedef std::map<wstring, Classify1th > Classify2th;
		typedef std::map<wstring, Classify2th > Classify3th;
		CompilerAct first_phase_filter(std::vector<NameLookUpNotify*>& temp_old_stack,
			Classify1th& pkg_roots, Classify1th& pkgs, Classify1th& files)
		{
			for (auto _notify :temp_old_stack) {
				auto _lv = _notify->level;
				if (_lv == PKG_ROOT_LV)
					pkg_roots.push_back(_notify);
				else if (_lv == NotifyLevel::PKG_LV)
					pkgs.push_back(_notify);
				else if (_lv == NotifyLevel::FILE_LV)
					files.push_back(_notify);
				else
					return  REBUILD;
			}
			return INCREMENT;
		}

		void second_phase_filter(NotifyResult::RootT& _roots,Classify2th& second_clasify_pkg)
		{
			for (auto _root_it : _roots) {
				auto& pkg_root = _root_it.second;
				if (pkg_root.empty()) 
					continue;	
				auto it = second_clasify_pkg.begin();
				auto end = second_clasify_pkg.end();
				for (; it != end; ) {
					if (pkg_root.find(it->first) != pkg_root.end())
						it = second_clasify_pkg.erase(it);
					else 
						++it;
				}
			}
		}
		void second_phase_filter(NotifyResult::RootT& _roots,NotifyResult::FileT& _files){	
			for (auto _root_it : _roots) {
				auto& pkg_root = _root_it.second;
				if (pkg_root.empty()) 
					continue;	
				for (auto it_pkg_root : pkg_root){
					_files.erase(it_pkg_root.first);
				}	
			}
		}
		
		void third_phase_filter(NotifyResult::PkgT& pkg_result,NotifyResult::FileT& clasify_files)
		{
			auto it = clasify_files.begin();
			auto end = clasify_files.end();
			for (; it != end; ) {
				NotifyResult::PkgT& files_in_pkg = it->second;
				auto find_root = pkg_result.find(it->first);
				if (find_root != pkg_result.end()) {
					for (auto classify_pkgs : find_root->second) {
						auto& pkgs = classify_pkgs.second;
						for (auto it_in_pkg : pkgs)
							files_in_pkg.erase(it_in_pkg.first);
					}
				}
				if (files_in_pkg.empty())
					it = clasify_files.erase(it);
				else
					++it;
			}
		}
		void ClassifyPkgs(Classify2th& second_clasify_pkg, Classify1th& pkgs)
		{
			for (size_t i = 0; i < pkgs.size(); ++i)
			{
				auto pkg = (PackageFragmentMsg*)pkgs[i];

				if (!pkg)
					continue;

				second_clasify_pkg[pkg->root_name].push_back(pkg);
			}
		}
		void ClassifyFiles(Classify3th& third_clasify_files, Classify1th& files)
		{
			Classify2th second_clasify_files;
			for (size_t i = 0; i < files.size(); ++i)
			{

				auto _files = (FileMsg*)files[i];
				if (!_files)
					continue;

				second_clasify_files[_files->root_name].push_back(_files);

			}
			for (auto it_2th : second_clasify_files )
			{
				const wstring& root_name = it_2th.first;
				const Classify1th& _array = it_2th.second;
				for (size_t i = 0; i < _array.size(); ++i){
					auto _file = (FileMsg*)_array[i];

					if (!_file)
						continue;

					third_clasify_files[root_name][_file->pkg_name].push_back(_file);
				}
			}
		}
		void GetBuildOpt(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >& result,
			std::vector<NameLookUpNotify*>& stack)
		{
			if (!stack.size())
				return;
			auto& add_files = result[ELEMENT_ADD];
			auto& chang_files = result[ELEMENT_CHANGE];
			auto& remove_files = result[ELEMENT_REMOVE];
			std::map<std::wstring, ResourceOptKind> first_set;
			for (int size = stack.size(), i = 0; i < size; ++i)
			{
				auto opt = stack[i];
				if(!opt)
					continue;

				std::wstring& _name = opt->name;
				first_set.insert(make_pair(_name, opt->kind));
				if (ELEMENT_ADD == opt->kind) {
					if (!remove_files.erase(_name))
						chang_files.erase(_name);
					add_files.insert(make_pair(_name, opt) );
				}
				else if (ELEMENT_CHANGE == opt->kind) {
					if (!remove_files.erase(_name))
						add_files.erase(_name);
					chang_files.insert(make_pair(_name, opt));
				}
				else {
					if (!chang_files.erase(_name))
						add_files.erase(_name);
					remove_files.insert(make_pair(_name, opt));
				}
			}
			for (auto it = chang_files.begin(),
			 end = chang_files.end()
				; it != end; ) {
				ResourceOptKind kind = first_set[it->first];
				if (ELEMENT_ADD == kind)// 开始的时候是add,那说明这个文件对当前的编译状态来说，它是已经不存在的文件，新添加的文件
				{
					add_files.insert(*it);
					it =chang_files.erase(it);
				}
				else
					++it;
			}	
			for (auto it = add_files.begin(),end = add_files.end(); it != end;) {
				ResourceOptKind kind = first_set[it->first];
				if (ELEMENT_ADD != kind)// 开始的时候不是add,那说明这个文件对当前的编译状态来说，它是已经存在的文件
				{
					chang_files.insert(*it);
					it = add_files.erase(it);
				}
				else
					++it;
			}
			for (auto it = remove_files.begin(),end = remove_files.end(); it != end;) {
				ResourceOptKind kind = first_set[it->first];
				if (ELEMENT_ADD == kind)// 开始的时候是add,那说明这个文件对当前的编译状态来说，它是已经不存在的文件，新添加的文件	
				{
					it = remove_files.erase(it);
				}
					
				else
					++it;
			}
			stack.clear();
		}	
		void getAffectSourceFiles(NotifyResult_For_Origianal* _notifys){
			assert(_notifys);		
			for (auto it : _notifys->_files){
				for (auto pkg_it : it.second){
					map < ResourceOptKind,
						map<wstring, NameLookUpNotify*> >&  kind_files = pkg_it.second;
				
					for (auto kind_it : kind_files){
						if (kind_it.second.empty())
							continue;
					
						for (auto  file_it : kind_it.second) {
							auto fileMsg = reinterpret_cast<FileMsg*>(file_it.second);
							if (!fileMsg || fileMsg->file_path.empty() == true)
								continue;
							switch (kind_it.first)
							{
							case ELEMENT_ADD:
								_notifys->add_file_set.insert(    make_pair(fileMsg, fileMsg->file_path));
								break;
							case ELEMENT_CHANGE:
								_notifys->chang_file_set.insert( make_pair(fileMsg, fileMsg->file_path));
							
								break;
							case ELEMENT_REMOVE:
								_notifys->remove_file_set.insert( make_pair(fileMsg, fileMsg->file_path));
								break;
							default:
								assert(false);
								break;
							}
						}
					}
				}
			}
		}

		NotifyResult* ResouceOptStack::GetClassifyNotify(vector<NameLookUpNotify*>& stack, bool for_orginal)
		{
			//分类 
			Classify1th pkg_roots;
			Classify1th pkgs;
			Classify1th files;
			NotifyResult *ret;
			if(for_orginal)
			   ret = new NotifyResult_For_Origianal();
			else
				ret = new NotifyResult();

			auto act = first_phase_filter(stack, pkg_roots, pkgs, files);
			if (act == REBUILD) {
				ret->act = act;
				return ret;
			}

			Classify2th second_clasify_pkg;
			ClassifyPkgs(second_clasify_pkg, pkgs);

			Classify3th clasify_files;
			ClassifyFiles(clasify_files, files);
			std::vector<NameLookUpNotify*> pkg_roots_bak(pkg_roots);
			// 得到 _roots
			GetBuildOpt(ret->_roots, pkg_roots);

			//过滤掉 root 来说和他们相关的second_clasify_pkg和second_clasify_files都 不需要考虑
			second_phase_filter(ret->_roots, second_clasify_pkg);
			
			// 得到_pkgs		
			for (auto it : second_clasify_pkg) {
				GetBuildOpt(ret->_pkgs[it.first], it.second);
			}

			// 得到_files
			for (auto root_it : clasify_files) {
				for (auto classify_pkgs : root_it.second) {
					for (auto it_files_in_pkg : root_it.second) {
						GetBuildOpt(ret->_files[root_it.first][it_files_in_pkg.first], it_files_in_pkg.second);
					}
				}
			}

			if (for_orginal)
				getAffectSourceFiles((NotifyResult_For_Origianal*)ret);

			second_phase_filter(ret->_roots, ret->_files);
			// 过滤 clasify_files
			third_phase_filter(ret->_pkgs, ret->_files);

			if (for_orginal){		
				auto all_notify = new std::vector<NameLookUpNotify*>();
			
				all_notify->insert(all_notify->end(), pkg_roots_bak.begin(), pkg_roots_bak.end());
				for (auto it : second_clasify_pkg) {
					const Classify1th& _notify = it.second;
					all_notify->insert(all_notify->end(), _notify.begin(), _notify.end());
				}
				for (auto it : clasify_files) {
					for (auto second_it : it.second) {
						const Classify1th& _notify = second_it.second;
						all_notify->insert(all_notify->end(), _notify.begin(), _notify.end());
					}
				}
				((NotifyResult_For_Origianal*)ret)->all_notify = all_notify;
			}
			return ret;
		}
		ResouceOptStack::~ResouceOptStack(){
			for (auto it : stack) {
				delete it;
			}
			stack.clear();
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

