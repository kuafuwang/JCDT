#include "stdafx.h"
#include <JavaModelLib/compiler/BuilderMgr.h>

#include <jni.h>
#include <PathModelLib/DeltaPath/ResouceOptStack.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <boost/function.hpp>
#include <boost/thread/future.hpp>

#include <ProjectModelLib/ProjectModel/ProjectSolution/SolutionModel.h>
#include <PathModelLib/SourcePath/UnitManager.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <PathModelLib/ClassPath/BaseCompilerEnvManager.h>
#include <Utillib/common/Util.h>
#include "JavaModelLib/compiler/JDTCompiler.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include "ProjectModelLib/ProjectModel/JCIDE/ProjectModel.h"
#include "ProjectModelLib/Builder/NameEnvironment.h"
#include <InterfaceLib/View/DocumentView.h>
namespace Jikes
{ // Open namespace Jikes block


	namespace JavaModel {

		void BingdingStageBuld(OriginalJavaProject* proj, IProgressMonitor* pm, JDTCompiler* compiler)
		{
			if (!proj || !pm)
			{
				return;
			}
			std::wstring tempArg;
			proj->CalculateOptStack();
			set<wstring> files;
			proj->getAllSourceFiles(files);
			JikesClassPaths collector;
			auto  pJm = proj->getJavaModel();
			auto fName = proj->getElementName();
			pJm->getExpandClasspath(collector, proj->getElementName());
			auto project_model = pJm->m_pOwner->GetProject(fName);
			if (!project_model || !project_model->IsExist())
			{
				return;
			}
			

			wstring m_strClassPath;
			for (const auto& it : collector._paths)
			{
				wstring shortName;
				if (!it.second->source_only)
				{
					shortName = Util::GetShortName(it.second->class_path);
					if (shortName.empty())
					{
						shortName = it.second->class_path;
					}
				}
				else
				{
					shortName = Util::GetShortName(it.second->source_attach_path);
					if (shortName.empty())
					{
						shortName = it.second->source_attach_path;
					}
				}
				m_strClassPath += Util::AddDoubleQuotation(shortName) + L";";
			}
			wstring_array vector_files;
			for(const auto& file_to_read : files)
			{
				auto _doc = pJm->getDocContext(file_to_read);
				if (!_doc)
				{
					vector_files.push_back(file_to_read);
					continue;
				}
					
				auto contextInfo = _doc->getDocContext();
				char* Buffer = contextInfo.first;
				if (!Buffer)
				{
					vector_files.push_back(file_to_read);
					continue;
				}
				compiler->AddRamUnit(Buffer, file_to_read, project_model->GetJikesParseProjectPath());
	
			}
			BuilderUtil::GetJavacCommandLineString(true, project_model->m_Data, m_strClassPath, vector_files, tempArg);
		

			
			std::wstring compilerInfo;
			//compiler->CompilerFiles(tempArg, compilerInfo);
			compiler->Process(tempArg, compilerInfo);
			auto buildInfo = proj->UpdateCompilationUnitInSourceEntry(compilerInfo);
			pm->endTask(buildInfo, 10);

		}
	
		unsigned core_count()
		{
			unsigned count;
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			count = si.dwNumberOfProcessors;
			return count;
		}
		const unsigned  CORE_COUNT = core_count();


		

		BuilderMgr::BuilderMgr(IProgressMonitor* p, JavaModel* javamodel, SolutionModel* ps)
			: pm(p), pJM(javamodel), pSolution(ps), _build_stamp(0), m_Compiler(nullptr)
		{
			
		
		}

		BuilderMgr::~BuilderMgr()
		{
			delete m_Compiler;
			delete pm;
		}
		void BuilderMgr::HookBuildProc()
		{
			auto compiler_env_manager = pJM->m_CompilerEnvMgr;
			auto  dllPath = compiler_env_manager->GetJvmDllPath();
			m_Compiler = new JDTCompiler(dllPath, compiler_env_manager->GetJDT_ToolDllPath());
			m_Compiler->LinkToJNI();
			if(!m_Compiler->Valid())
			{
				return;
			}
			
			assert(pm);
			boost::mutex::scoped_lock lock(*pJM->build_mu);
			while (true) {

				while (_build_stamp == pJM->build_stamp) {
					pJM->build_cond->wait(lock);
					if (pm->isCanceled())
						return;
				}

				_build_stamp = pJM->build_stamp;


				auto temp = pJM->getProjectsToBuild();

				if (!temp) {
					continue;;
				}
				if (pm->isCanceled())
					return;
				map<wstring, boost::shared_ptr<OriginalJavaProject>>& projects = *temp;

				//if (CORE_COUNT > 1)
				//	BuildMutipleCore(projects);
				//else
				BuildOnOneCore(projects);

				delete temp;

			}
		}

		void BuilderMgr::BuildProc()
		{
			HookBuildProc();

		}



		void BuilderMgr::GetBuildProject(
			map<wstring, boost::shared_ptr<OriginalJavaProject>>& projects,
			vector<OriginalJavaProject* >& sorted_project,
			vector<OriginalJavaProject*>& unorder_projects)
		{
			vector<wstring> order = pSolution->GetBuildOrder();
			size_t order_size = order.size();
			vector<wstring > sorted_project_name(order_size);
			set<wstring> unorder_projects_name;
			pJM->getAllProjectsName(unorder_projects_name);
			for (size_t i = 0; i < order_size; ++i) {
				auto it_find = unorder_projects_name.find(order[i]);
				if (it_find != unorder_projects_name.end()) {
					sorted_project_name[i] = (*it_find);
					unorder_projects_name.erase(it_find);
				}
			}

			if (!sorted_project_name.empty())
			{
				auto end_projects = projects.end();
				auto it = sorted_project_name.begin();
				auto end = sorted_project_name.end();

				for (size_t i = 0; i < order_size; ++i)
				{
					auto find_it = projects.find(sorted_project_name[i]);
					if (find_it != end_projects) {
						sorted_project.push_back(find_it->second.get());
					}
				}
			}
			if (!unorder_projects_name.empty())
			{
				auto end_unorder_projects_name = unorder_projects_name.end();
				auto it = projects.begin();
				auto end = projects.end();
				for (; it != end; ++it) {
					auto find_it = unorder_projects_name.find(it->first);
					if (find_it != end_unorder_projects_name)
						unorder_projects.push_back(it->second.get());
				}
			}
		}


		void DealSortedProjects(vector<OriginalJavaProject* >& sorted_projects,
			IProgressMonitor* pm, JDTCompiler* compiler)
		{
			auto _size = sorted_projects.size();
			if (!_size)
				return;
			for (size_t i = 0; i < _size; ++i) {
				BingdingStageBuld(sorted_projects[i], pm, compiler);
			}
		}



		void BuilderMgr::BuildOnOneCore(map<wstring, boost::shared_ptr<OriginalJavaProject>>& projects)
		{
			auto _size = projects.size();
			if (_size == 1)
				BingdingStageBuld(projects.begin()->second.get(), pm, m_Compiler);
			else {
				vector<OriginalJavaProject* > sorted_project;
				vector<OriginalJavaProject*> unorder_projects;
				GetBuildProject(projects, sorted_project, unorder_projects);

				_size = unorder_projects.size();
				for (size_t i = 0; i < _size; ++i)
					BingdingStageBuld(unorder_projects[i], pm, m_Compiler);

				DealSortedProjects(sorted_project, pm, m_Compiler);


				pm->done(nullptr);
			}
		}


		void BuilderMgr::BuildMutipleCore(map<wstring, boost::shared_ptr<OriginalJavaProject>>& projects)
		{


			auto _size = projects.size();
			if (_size == 1)
			{
				wstring projectName = projects.begin()->first;

				BingdingStageBuld(projects.begin()->second.get(), pm, m_Compiler);

				pm->done(nullptr);
			}
			else {

				vector<OriginalJavaProject* > sorted_project;
				vector<OriginalJavaProject*> unorder_projects;
				GetBuildProject(projects, sorted_project, unorder_projects);

				typedef boost::packaged_task<void> pti_t;
				typedef boost::unique_future<void>  f_t;
				vector<pti_t> ap;
				vector<f_t> au;
				_size = unorder_projects.size();
				for (size_t i = 0; i < _size; ++i)
				{
					ap.push_back(pti_t(boost::bind(BingdingStageBuld, unorder_projects[i], pm, m_Compiler)));
					auto index = ap.size() - 1;
					au.push_back(ap[index].get_future());
					boost::thread temp(boost::move(ap[index]));
					temp.detach();
				}

				_size = sorted_project.size();
				if (_size) {
					if (unorder_projects.size() != 0) {
						ap.push_back(pti_t(boost::bind(DealSortedProjects, sorted_project, pm, m_Compiler)));
						auto index = ap.size() - 1;
						au.push_back(ap[index].get_future());
						boost::thread temp(boost::move(ap[index]));
						temp.detach();
					}
					else {
						DealSortedProjects(sorted_project, pm, m_Compiler);
					}
				}
				if (unorder_projects.size() != 0) {
					boost::wait_for_all(au.begin(), au.end());
				}
				pm->done(nullptr);
			}
		}


	} // Close namespace JavaModel block

} // Close namespace Jikes block

