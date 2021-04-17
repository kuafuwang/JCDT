#include "stdafx.h"
#include "CallHierarcky.h"
#include <JavaModelLib/internal/core/JavaModel.h>
#include <CodeAssistLib/select/SelectionEngine.h>
#include <CodeAssistLib/select/SelecctResult.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include "SearchEngine.h"
#include "JavaSearchScope.h"
#include "CallHierarckyCollector.h"
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <SearchLib/internal/pattern/AndPattern.h>
#include <ProjectModelLib/Builder/NameEnvironment.h>

using namespace Jikes::CodeAssist;

namespace Jikes { // Open namespace Jikes block


	
namespace Search{

	

	CallHierarcky::CallHierarcky(JavaModel::JavaModel* _pJM,
		CodeAssist::AssistInfo* _cur_info)
		: cur_info(_cur_info), pJM(_pJM)
	{

	}

	CallHierarcky::~CallHierarcky()
	{
		if (cur_info)
			delete cur_info;
	}

	void CallHierarcky::OpenCallHierarcky(CallHierarckyCollector* collector_) const
	{
		if (!collector_)
			return;

		auto _pos = pJM->getLayerPos(cur_info->doc_path);
		if (!_pos)
			return;

		auto java_project = pJM->getJavaProject(_pos->proj);
		if (!java_project) {
			delete _pos;
			return;
		}
		wstring projName = _pos->proj;
		auto  search =  new SearchEngine();
		JavaSearchScope* scope = new JavaSearchScope();
		std::map<int, bool> limits;
		scope->setLimitTo(limits);
		scope->add(projName, pJM);
		int limitTo = IJavaSearchConstants::REFERENCES;
		search->search(pJM, collector_->search_element, limitTo, scope, collector_);
		delete search;
	}
}


} // Close namespace Jikes block


