#include "stdafx.h"

#include "StatisticsMgr.h"
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include "StatisticsEngine.h"
#include <JavaModelLib/internal/lookup/SelectionRequestor.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include "FindReferenceOption.h"

namespace Jikes {

	namespace Search 
	{
		 StatisticsMgr::StatisticsMgr()
		{
			
		}

		 void StatisticsMgr::Process(InFilesFindReferenceOption* process_item,
			 JavaModel::JavaModel* pJM, map<wstring, ReferenceFileResultElement*>& result)
		 {
			 if (!process_item || !pJM)
				 return;

			 StatisticsEngine* engine = nullptr;

			 JavaModel::SelectionRequestor* requestor = nullptr;

		
			 Option* option = new Option();
			 option->bytecode = false;
			 JikesClassPaths collector;
			 map<wstring, JavaModel::ISelectionRequestor*> selectorRequestorMap;
			 set<wstring> filePaths;
			 for (auto it : process_item->files)
			 {
				 const wstring& projectName = it.first;
				 auto java_project = pJM->getJavaProject(projectName);
				 if (!java_project) {
					 continue;
				 }
				 pJM->getExpandClasspath(collector, projectName);
				 requestor = new JavaModel::SelectionRequestor(java_project, nullptr);
				 selectorRequestorMap.insert({ projectName,requestor });

				 for(auto file : (it.second))
				 {
					 filePaths.insert(file);
				 }
				
			 }



			 INameEnvironment* nameEnv = new INameEnvironment(option, collector);
			 set<wstring> has_problem_files;

			 engine = new StatisticsEngine(nameEnv, option, pJM, process_item);
			 engine->selectorRequestorMap.swap(selectorRequestorMap);

			
			 engine->Staticstics(filePaths);

			 result.swap(engine->result);

			 engine->clear();
			 delete engine;
		 }
	}
}
