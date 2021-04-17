
#ifndef JavaModel_java_model_INCLUDED_INCLUDED
#define JavaModel_java_model_INCLUDED_INCLUDED
#include <JavaModelLib/impl/config.h>

#include "Openable.h"
#include <windows.h>
#include <set>
#include "IJavaModel.h"
#include <boost/thread.hpp>
#include "lru.hpp"
#include "PathModelLib/JarPath/JarPathManager.h"
using std::set;
using std::vector;
using std::wstring;

class SolutionModel;

class DocumentView;

namespace Jikes {
	class JikesClassPaths;
	class AssistFileHelper;
	namespace CodeAssist
	{
		class CodeAssistMgr;
		class AssistInfo;
	}
	namespace PathModel
		
	{
		class JarPathManager;
		class NameLookUpNotify;
		class UnitManager;
		class BaseCompilerEnvManager;
	}
}

namespace Jikes { // Open namespace Jikes block
	
	namespace JavaModel {
		class BuilderMgr;

		class NameLookup;
		class OriginalJavaProject;
		class LayerPosInJavaModel;
	
		enum FileTypeInJavaProject;

		class JavaModel :
			 public IJavaModel
			
		{
		public:
			IJavaElement* DeepCopy(IJavaElement*) override;
			const wchar_t* default_project_name = L"{49A26700-D9BA-432A-8641-AFA5E88971D0}[Guid(49A26700-D9BA-432A-8641-AFA5E88971D0)]";
			IJavaElement* ShallowCopy(IJavaElement* parent) override;;
			friend class SolutionModel;
			enum PropagtionCode{
				REBUILD_PROJECT,
				NO_REBUILD_PROJECT
			};
			bool hasChildren() override;
			std::vector<IJavaElement*>* getChildren() override;
			void removeInfo() override;
		
			bool removeProject(const wstring& proj);
			void addChild(IJavaElement* child) override;

			void addChild(boost::shared_ptr<OriginalJavaProject>& child);
			FileTypeInJavaProject WhatItIs(const wstring& _path);
			bool generateInfos(IProgressMonitor* pm) override;

			 void GetProjectsInClassPaths(set<wstring>&, const wstring& proj);

			void UpdateBuildStamp();

		
			~JavaModel();
			JavaModel(SolutionModel*, PathModel::BaseCompilerEnvManager*);
			boost::shared_ptr<OriginalJavaProject>& getJavaProject(const wstring& name);
		
			 void getExpandClasspath(JikesClassPaths& paths, const wstring& projectName);

			 void getClasspathButNoPorjcetEntry(JikesClassPaths& paths, const wstring& projectName) const;
			 void getClasspathButNoPorjcetEntryForSearchScope(
				 const wstring& projectName,
				 const std::map<int, bool>& limits,
				 map<wstring, wstring> & result);
			std::map< wstring, boost::shared_ptr<OriginalJavaProject> >* getProjectsToBuild();

			void PushResourceOpt(const wstring& proj,
				vector<PathModel::NameLookUpNotify*>&  _notisfys, PropagtionCode _code);
			void PushResourceOpt(const wstring& proj, PathModel::NameLookUpNotify*  _notisfy,
				PropagtionCode _code);
			void  getAllProjectsName(set<wstring>& projectNames);

		
			set<wstring>  getProjectsRelatedTo(const wstring projectName);
			set<wstring> getProjectsThatNeeds(const wstring projectName);// 用到这个工程的 工程

			set<wstring> getProjectsNeededBy( const wstring projectName);// 被这个工程用到的 工程

			void PropagationProjectChange(const wstring& proj,bool include_proj);

			 void InintJavaModel(AssistFileHelper*, CodeAssist::CodeAssistMgr*, BuilderMgr*);

			void RebuildAllProjects();
			
			LayerPosInJavaModel* getLayerPos(const wstring& _path);

			JavaModel* getJavaModel() override;
			void GetAllFileNamesInPkg(const wstring& project, const wstring& pkgName, vector<wstring>& names) const;
			void removeDocFromCache(const wstring& _name);

			void insertDocToCache(const wstring& _name, DocumentView* _doc);

			DocumentView*  getDocContext(const wstring& _name);
			void UpdateCodeAssistInfo(CodeAssist::AssistInfo* _pos) ;
		public:
			CRITICAL_SECTION* cs_generate;
			CRITICAL_SECTION* cs_modifi_doc;

			boost::condition_variable_any* build_cond;
			boost::mutex* build_mu;

			boost::condition_variable_any* code_assist_cond;
			boost::mutex* code_assist_mu;


			CRITICAL_SECTION* cs_code_assit_stack;
			CodeAssist::CodeAssistMgr* code_assist_mgr_ = nullptr;

			friend class BuilderMgr;
			SolutionModel* m_pOwner;
		
			PathModel::BaseCompilerEnvManager* m_CompilerEnvMgr;
		protected:
			volatile unsigned build_stamp = 0;

		private:
			
			PathModel::JarPathManager jar_manager_;
			std::map < wstring, boost::shared_ptr<OriginalJavaProject> >  childrens;
			lru::LRUNoCache<wstring, boost::shared_ptr<OriginalJavaProject> > childrenCache;
			lru::LRUNoCache<wstring, boost::shared_ptr<OriginalJavaProject> >::const_iterator chche_end;
			boost::shared_mutex rw_mutex;
		
			static boost::shared_ptr<OriginalJavaProject> NO_PROJECT;

			lru::LRUNoCache< wstring, DocumentView* > modifiedDocsCache;
			lru::LRUNoCache< wstring, DocumentView* >::const_iterator modified_doc_end;
			

		};


		inline void JavaModel::UpdateBuildStamp() {
			::InterlockedIncrement(&build_stamp);
			build_cond->notify_all();
		}
		inline void JavaModel::removeDocFromCache(const wstring& _name) {
		
			EnterCriticalSection(cs_modifi_doc);
			modifiedDocsCache.erase(_name);
			LeaveCriticalSection(cs_modifi_doc);
		}

		inline void JavaModel::insertDocToCache(const wstring& _name, DocumentView* _doc) {
		
			EnterCriticalSection(cs_modifi_doc);
			modifiedDocsCache.insert(_name, _doc);
			LeaveCriticalSection(cs_modifi_doc);
		}

		inline DocumentView* JavaModel::getDocContext(const wstring& _name)
		{
			
			EnterCriticalSection(cs_modifi_doc);
			auto find_it = modifiedDocsCache.find(_name);
			LeaveCriticalSection(cs_modifi_doc);
			if (modified_doc_end != find_it) {	
				return find_it.value();
			}
			else
			{
				return nullptr;
			}
		}
		
	}// Close namespace JavaModel block


} // Close namespace Jikes block


#endif // _INCLUDED

