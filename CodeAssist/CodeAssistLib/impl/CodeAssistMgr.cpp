#include "stdafx.h"
#include "CodeAssistMgr.h"
#include <CodeAssistLib/select/SelectionEngine.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/compiler/BuildMsgInfo.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <CodeAssistLib/select/SelecctResult.h>
#include <JavaModelLib/internal/core/Member.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IType.h>
#include <boost/algorithm/string.hpp>
#include <tchar.h>
#include "AssistResult.h"
#include <CodeAssistLib/complete/CompletionEngine.h>
#include <CodeAssistLib/complete/ResultCollector.h>
#include <CodeCorrectionLib/Processor/JavaCorrectionProcessor.h>
#include <CodeCorrectionLib/core/ProblemPosition.h>
#include <JCDT_Lib/internal/problem/SemanticError.h>
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include <CodeAssistLib/impl/AssistInfo.h>
#include "CodeAssistLib/complete/Proposal/IJavaCompletionProposal.h"
#include <boost/format.hpp>
#include <JavaModelLib/compiler/DefaultProblem.h>
using namespace Jikes::Correction;

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 void CodeAssistMgr::UpdateInfo(AssistInfo* _value) {
			
			 if (!_value)
				 return;

			EnterCriticalSection(pJM->cs_code_assit_stack);
			if (_info != nullptr) {
				delete _info;
			}
			_info = _value;
			LeaveCriticalSection(pJM->cs_code_assit_stack);
		}
		CodeAssistMgr::CodeAssistMgr(IProgressMonitor* p, JavaModel::JavaModel* javamodel)
			: pm(p), _info(nullptr),  pJM(javamodel)
		{

		}

		CodeAssistMgr::~CodeAssistMgr()
		{
			delete pm;
			delete _info;
		}

		void CodeAssistMgr::HookDispathProc()
		{
			assert(pm);
			boost::mutex::scoped_lock lock(*pJM->code_assist_mu);
			while (true) {

				while (true) {
					EnterCriticalSection(pJM->cs_code_assit_stack);
					if (_info != nullptr) {
						break;
					}

					LeaveCriticalSection(pJM->cs_code_assit_stack);

					pJM->code_assist_cond->wait(*pJM->code_assist_mu);

					if (pm->isCanceled()) {

						return;
					}
				}
				auto cur_info = _info;
				_info = nullptr;

				LeaveCriticalSection(pJM->cs_code_assit_stack);

				if (cur_info->what_for == AssistInfo::COMPILTE) {
					DispatchToCompelet(cur_info, pJM, pm);
				}
				else if (cur_info->what_for == AssistInfo::SELECT_FOR_EDIT_TIPS)
				{
					DispatchEvent(cur_info);
				}
				else {
					DispatchToSelect(cur_info, pJM, pm);
				}
			}
		}


		void CodeAssistMgr::DispatchToCorrectionError(
				AssistInfo* cur_info,
				JavaModel::JavaModel* pJM,
				IProgressMonitor* pm)
		{
			if (!cur_info || !pJM || !pm)
				return;


			bool twice_flag = false;
			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			auto java_project = pJM->getJavaProject(_pos->proj);
			if (!java_project) {
				delete _pos;
				delete cur_info;
				return;
			}

			if (!cur_info->extraData)
				return;

			ResultForCorrectionError* callback_info = new ResultForCorrectionError();
			callback_info->errorInfo.swap(cur_info->extraData->errorInfos);
			callback_info->info = cur_info;
			auto semanticError = cur_info->extraData->semanticError;
			if(semanticError && JavaCorrectionProcessor::hasCorrections(semanticError->msg_code)){

				ProblemPosition* problemPos = new ProblemPosition(_pos, pJM,cur_info, semanticError->msg_code,
					semanticError->left_line_no, semanticError->left_column_no,
					semanticError->right_line_no, semanticError->right_column_no);
		
				JavaCorrectionProcessor::collectCorrections(problemPos, callback_info->proposals);
				delete problemPos;
			}
			else
			{
				Option* option = new Option();
				option->bytecode = false;
				JikesClassPaths collector;
				pJM->getExpandClasspath(collector, _pos->proj);
				JavaModel::SelectionRequestor* 	requestor = new JavaModel::SelectionRequestor(java_project, _pos);
				INameEnvironment* nameEnv = new INameEnvironment(option, collector);
				set<wstring> has_problem_files;
				java_project->GetErrorFilse(has_problem_files);
				SelectionEngine* engine = new SelectionEngine(nameEnv, option, requestor, *cur_info, has_problem_files);

				auto _ret = engine->select();
				if (_ret){
					delete _ret;
					auto elements = requestor->getElements();
					if (elements.size()) {	
						wstring tipsCollect;
						const size_t declCount = elements.size();
						for (size_t i = 0; i < declCount; ++i) {
							if (!elements[i])
								continue;

							auto fullName = elements[i]->getFullName();
							if (wstring::npos == tipsCollect.find(fullName)) {
								if (i != 0)
									tipsCollect += _T("\n");
								tipsCollect += fullName;
							}
						}
						if (!tipsCollect.empty()) {
							int length = tipsCollect.size() + 1;
							wchar_t* pTipsInfo = new wchar_t[length];
							memset(pTipsInfo, 0x00, length + 1);
							wcscpy_s(pTipsInfo, length, tipsCollect.c_str());
							callback_info->pTipsInfo = pTipsInfo;
						
							twice_flag = true;
							callback_info->info->what_for = AssistInfo::SELECT_FOR_CORRECTION_ERROR;
							pm->endTask(callback_info, 100);
						}
					} 
				}
				delete requestor;
				engine->clear();
				delete engine;
			}
			if (false == twice_flag)
			{
				callback_info->info->what_for = AssistInfo::SELECT_FOR_CORRECTION_ERROR;
				pm->endTask(callback_info, 100);
			}
		}

		void CodeAssistMgr::DispatchToSelect(
			AssistInfo* cur_info,
			JavaModel::JavaModel* pJM, 
			IProgressMonitor* pm)
		{
			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			if(_pos == nullptr)
			{
				return;
			}
			auto java_project = pJM->getJavaProject(_pos->proj);
			if (!java_project){
				delete _pos;
				delete cur_info;
				return ;
			}

			SelectionEngine* engine = nullptr;
			JavaModel::SelectionRequestor* requestor = nullptr;
			

			Option* option = new Option();
			option->bytecode = false;
			JikesClassPaths collector;
			pJM->getExpandClasspath(collector, _pos->proj);
			requestor = new JavaModel::SelectionRequestor(java_project, _pos);

			INameEnvironment* nameEnv = new INameEnvironment(option, collector);
			set<wstring> has_problem_files;
			java_project->GetErrorFilse(has_problem_files);
			engine = new SelectionEngine(nameEnv, option, requestor, *cur_info, has_problem_files);
			
			auto _ret = engine->select();
			if (!_ret)
			{
				delete requestor;
				engine->clear();
				delete engine;
				delete cur_info;
				return;
			}
			auto __type = _ret->GetType();
			if (__type  == SelecctResult::SELECT_FOR_DEBUG_TYPE) {
				auto callback_info = new ResultForDebug();
				SelectForDebug* select_for_debug = reinterpret_cast< SelectForDebug*>(_ret);
				callback_info->info = cur_info;
				cur_info = nullptr;
				callback_info->expr_string_ = select_for_debug->expr_string_;
				callback_info->expr = select_for_debug->TransferExpr();
			
				pm->endTask(callback_info, 100);
			}
			else if(__type == SelecctResult::SELECT_FOR_SCOPE_TYPE)
			{
				auto callback_info = new ResultForScope();
				auto select_for_ = reinterpret_cast< SelectForScope*>(_ret);
				callback_info->info = cur_info;
				cur_info = nullptr;
			
				callback_info->range = select_for_->TransferRange();
				
				pm->endTask(callback_info, 100);
			}
			else
			{
				
				auto elements = requestor->getElements();
				if (elements.size())
				{
					if(	   cur_info->what_for == AssistInfo::SELECT_FOR_EDIT_TIPS
						|| cur_info->what_for == AssistInfo::SELECT_FOR_DEBUG){
						wstring tipsCollect;
						const size_t declCount = elements.size();
						for (size_t i = 0; i < declCount; ++i) {

							if (!elements[i])
								continue;

							auto fullName = elements[i]->getFullName();
							if (wstring::npos == tipsCollect.find(fullName)) {
								if(i != 0)
									tipsCollect += _T("\n");
								tipsCollect += fullName;
							}
						}
						if (!tipsCollect.empty()) {
						
							auto callback_info = new ResultForTips();
							callback_info->info = cur_info;
							callback_info->info->what_for = AssistInfo::SELECT_FOR_EDIT_TIPS;
							callback_info->tipsInfo.swap(tipsCollect) ;
							auto  decl_item = dynamic_cast<JavaModel::Member*>(elements[0]);
							if(decl_item)
							{
								callback_info->filePath = decl_item->getPath();
								auto rng = decl_item->getNameRange();
								if (rng)
								{
									callback_info->rng = rng->Clone();
								}
								wstring _path;
								do
								{
									auto _contain_type = decl_item->getDeclaringType();
									if (!_contain_type)
										break;

									auto _contain_file = reinterpret_cast<JavaModel::BaseJavaFile*>(_contain_type->getParent());
									if (!_contain_file)
										break;
									_path = _contain_file->getAttachSourcePath();
								} while (false);
								callback_info->filePath = _path;
							}
		
							cur_info = nullptr;
					
							pm->endTask(callback_info, 100);
						}
					}
					else if (cur_info->what_for == AssistInfo::SELECT_FOR_EDIT_GO_TO_DECL)
					{
						auto callback_info = new ResultForGoTodecl();
						callback_info->info = cur_info;

						cur_info = nullptr;
						const size_t declCount = elements.size();
						for (size_t i = 0; i < declCount; ++i) {
							auto it = elements[i];
							if (!it)
								continue;
							bool fit = false;
							switch(it->fLEType)
							{
								case JavaModel::IJavaElementType::TYPE:
								case JavaModel::IJavaElementType::METHOD:
								case JavaModel::IJavaElementType::FIELD :
								case JavaModel::IJavaElementType::LOCAL_FIELD :
									fit = true;
									break;
								default:
									break;
							}
							if (!fit)
								continue;

							auto decl_item = reinterpret_cast<JavaModel::Member*>(it);
							
							auto name_range = decl_item->getNameRange();
							if (!name_range)
								continue;

							auto fullName = decl_item->getFullName();
							wstring _path;
							{
								auto _contain_type = decl_item->getDeclaringType();
								if (!_contain_type)
									continue;

								auto _contain_file = reinterpret_cast<JavaModel::BaseJavaFile*>(_contain_type->getParent())  ;
								if (!_contain_file)
									continue;
								_path = _contain_file->getAttachSourcePath();

							}
							auto item = new GoToDeclItem(fullName, 
								decl_item->fLEType,
								decl_item->getAccessFlags(), 
								_path,
								name_range->Clone(),
								decl_item->isBinary());

							callback_info->items.push_back(item);
						}
		
						pm->endTask(callback_info, 100);
					}
				}
					
			}
			delete _ret;
			delete requestor;
			engine->clear();
			delete engine;
			delete cur_info;

		}
		void CodeAssistMgr::DispatchToCompelet(
			AssistInfo* cur_info,
			JavaModel::JavaModel* pJM,
			IProgressMonitor* pm)
		{

			if (!cur_info || !pJM || !pm)
				return;

			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			auto java_project = pJM->getJavaProject(_pos->proj);
			if (!java_project) {
				delete _pos;
				delete cur_info;
				return;
			}

			CompletionEngine* engine = nullptr;
			Option* option = new Option();
			option->bytecode = false;
			JikesClassPaths collector;
			pJM->getExpandClasspath(collector, _pos->proj);
		
			INameEnvironment* nameEnv = new INameEnvironment(option, collector);
			ResultCollector* _requestor = new ResultCollector();
			engine = new CompletionEngine(
				java_project,
				nameEnv,
				option,
				*cur_info,
				_pos, 
				_requestor
				);
			
			engine->complete();

			ResultCollector* tempRequstor = _requestor;
			auto itemSize = tempRequstor->items.size();
			auto& tokenFor = tempRequstor->tokenForMatch;
			auto tokenSize = tokenFor.size();
			if (itemSize && tokenSize)
			{
				vector<IJavaCompletionProposal*> items;
				items.reserve(tempRequstor->items.size());
				auto leftData = tokenFor.c_str();
				vector<IJavaCompletionProposal*> lowMatchItems;
				for (auto i = 0; i < itemSize; ++i) {
					auto proposal = tempRequstor->items[i];
					if (!proposal)
						continue;

					auto testToken = proposal->GetTokenFroMatch();
					if (proposal->needToCheckMatch)
					{
						auto  testTokenLen = wcslen(testToken);
						if (tokenSize <= testTokenLen)
						{
							auto relevane = engine->computeRelevanceForCaseMatching(leftData, testToken);

							if (relevane)
							{
								proposal->relevance += relevane;
								items.emplace_back(proposal);

							}
							else
							{
								delete proposal;
								tempRequstor->items[i] = nullptr;

							}
						}
						else {
							delete proposal;
							tempRequstor->items[i] = nullptr;
						}
					}
					else
					{
						items.emplace_back(proposal);
					}
				}
				items.swap(tempRequstor->items);
			}


			if(_requestor->items.size() || _requestor->tips.empty())
			{
				auto callback_info = new ResultCodeComplete();
				callback_info->info = cur_info;
				callback_info->lenEntered = engine->lenEntered;
				cur_info = nullptr;
				callback_info->collector = _requestor;
				_requestor = nullptr;
				pm->endTask(callback_info, 100);

			}
			delete _requestor;
			delete engine;
			delete cur_info;
		}


		void CodeAssistMgr::DispathProc()
		{
			HookDispathProc();
		}

		void CodeAssistMgr::DispatchEvent(AssistInfo* cur_info){

			if (!cur_info)
				return;

			auto _pos = pJM->getLayerPos(cur_info->doc_path);
			if (!_pos)
				return;

			auto java_project = pJM->getJavaProject(_pos->proj);
			java_project->beginToOccupy();
			boost::shared_ptr<Jikes::JavaModel::BuildProblem > old_problem = java_project->problems[cur_info->doc_path];
			java_project->endToOccupy();
			delete _pos;

			bool dispatchToSelect = true;
			if (old_problem){
				int line = cur_info->line;
				
				using namespace Jikes;
				vector<JavaModel::DefaultProblem*>& _data = old_problem->data;
				ExtraInfoForError* extraData = nullptr;	
				auto size = _data.size();
				for (int i = 0; i < size; i++){
					auto error = _data[i];
					if (!error)
						continue;

				/*	if(error->getSeverity() !=  Problem::JIKES_ERROR)
					{
						continue;
					}*/

					int _line = error->getLeftLineNo();
					if (line == _line && error->IsInRange(cur_info->start_pos))
					{
						dispatchToSelect = false;
						if (!extraData) {
							extraData = new ExtraInfoForError();
							extraData->semanticError = new MiniSemanticError(
								error->id, error->getLeftLineNo(), error->getLeftColumnNo(),
								error->getRightLineNo(), error->getRightColumnNo(),
								error->getErrorReport(), error->getSeverity());
						}

						extraData->errorInfos.push_back(new MiniError(
							 error->getLeftLineNo(), error->getLeftColumnNo(),
							error->getRightLineNo(), error->getRightColumnNo(),
							error->getErrorReport(), error->getSeverity()));
					
					}
				}
				cur_info->extraData = extraData;
			}
		
			if(dispatchToSelect)
				DispatchToSelect(cur_info, pJM, pm);
			else
				DispatchToCorrectionError(cur_info, pJM, pm);
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

