#include "stdafx.h"
#include "ReorgCorrectionsSubProcessor.h"
#include <CodeCorrectionLib/Proposal/ReplaceCorrectionProposal.h>
#include "JavaCorrectionProcessor.h"
#include <JCDT_Lib/internal/problem/ErrorInfo.h>
#include <CodeAssistLib/impl/AssistInfo.h>
#include <CodeCorrectionLib/core/ProblemPosition.h>
#include <CodeCorrectionLib/Proposal/MultipleReplaceCorrectionProposal.h>
#include <JCDT_Lib/internal/util/stringex.h>
#include <JavaModelLib/compiler/BuildMsgInfo.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <CodeCorrectionLib/core/ASTResolving.h>
#include <CodeCorrectionLib/Proposal/RenameCUCompletionProposal.h>
#include <CodeCorrectionLib/Proposal/ChangeTypeNameCorrectionProposal.h>
#include <CodeCorrectionLib/core/CorrectionAssistASTVisitor.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/lookup/ExpandedMethodTable.h>
#include <JCDT_Lib/internal/lookup/MethodShadowSymbol.h>
#include <JCDT_Lib/internal/lookup/unitfilesymbol.h>
#include "ImportEditCtrl.h"
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <CodeCorrectionLib/Proposal/NewMethodCompletionProposal.h>
#include <CodeCorrectionLib/Proposal/MultipleNewMethodCorrectionProposal.h>
#include <JavaModelLib/compiler/DefaultProblem.h>
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		void ReorgCorrectionsSubProcessor::getWrongTypeNameProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{

			if (!problemPos || !problemPos->cur_info || !problemPos->layer_pos)
				return;

			auto& _info = *(problemPos->cur_info);
			ASTResolving* resolving = new ASTResolving(_info.line, _info.column, problemPos->pJm, problemPos->layer_pos->proj, _info.doc_path);

			auto   typeNames = resolving->GetTypeNames();
			delete resolving;
			if (typeNames.empty())
				return;

			RenameCUCompletionProposal* reNameProposal = new RenameCUCompletionProposal(typeNames[0],
				problemPos->layer_pos->proj, problemPos->layer_pos->projectPos.pkg);
			reNameProposal->oldFileName = problemPos->layer_pos->projectPos.name;
			reNameProposal->path = _info.doc_path;
			reNameProposal->fDisplayString = L"rename the file to "  + typeNames[0];

			reNameProposal->icon_type = CorrectionProposal::CHANGE_ICON;
			proposals.push_back(reNameProposal);


			auto proposal = new ChangeTypeNameCorrectionProposal(problemPos->layer_pos->proj, problemPos->layer_pos->projectPos.pkg, typeNames[typeNames.size() -1]);

			proposal->newName = problemPos->layer_pos->projectPos.name;
			proposal->fDisplayString = L"Change type name to " + proposal->newName;
			proposal->icon_type = CorrectionProposal::CHANGE_ICON;
			proposals.push_back(proposal);


		}
		//
		// Pass 4: Process the field declarations at the top level of the types
		//
		void CompleteSymbolTable(ProblemPosition* problemPos, ASTResolving* resolving ,std::vector<CorrectionProposal*>& proposals)
		{
			if (!problemPos || !resolving || !resolving->lexFile || !resolving->visitor || !resolving->visitor->astNode)
				return;


			auto semantic = resolving->lexFile->semantic;

			if (!semantic)
				return;

			auto class_body = reinterpret_cast<TypeDeclaration *>(resolving->visitor->astNode)->class_body;
			if (!class_body || !class_body->owner)
				return;

			auto  unit = resolving->lexFile->compilation_unit;
			if (!unit)
				return;

			TypeSymbol* this_type = class_body->owner->symbol;

			if (!this_type)
				return;

			if (!this_type->expanded_method_table)
				semantic->ComputeMethodsClosure(this_type, class_body->identifier_token);

			if (!this_type->super || this_type->Bad())
				return;
			
			if (this_type->ACC_ABSTRACT())
				return;	
			// We already checked that this class does not declare abstract
			// methods. Now see that there are no unimplemented abstract
			// methods in any of the superclasses or superinterfaces. Exploit
			// the fact that the method table is built with the first element
			// being from a superclass; all conflicts are inherited from
			// interfaces and are necessarily abstract.
			ExpandedMethodTable* expanded_table = this_type->expanded_method_table;

			if (!expanded_table)
				return;

			auto lex = resolving->lexFile->lex_stream;
			ImportEditCtrl* importEditCtrl = new ImportEditCtrl(lex,unit);

			
			int line_no;
			int column_no;
			auto tempNum = class_body->NumMethods();
			
			if(tempNum){

				auto methodAst = class_body->Method(tempNum - 1);
				if (!methodAst)
					return;

				auto rightToken = methodAst->RightToken();
				if (!rightToken)
					return;

				auto end_loc = rightToken->EndLocation();
				line_no = lex->LineFromPos(end_loc);
				column_no = lex->RightColumnFromPos(end_loc) + 1;
			
			}
			else{
				auto end_loc = class_body->right_brace_token->EndLocation();
				line_no = lex->LineFromPos(end_loc);
				column_no = lex->RightColumnFromPos(end_loc);		
			}

			auto pJm = problemPos->pJm;
			if (!pJm || !problemPos->layer_pos || !this_type->file_symbol)
				return;

			wstring projectName = problemPos->layer_pos->proj;
			auto project = pJm->getJavaProject(projectName);
			JavaModel::NameLookup* namel_lookup_ = nullptr;

			auto _fileName = this_type->file_symbol->getNameString();

			if (project){
				namel_lookup_ = project->getNameLookup();
				if (namel_lookup_)
					namel_lookup_->beginToOccupy();

			}
		
			auto length = expanded_table->symbol_pool.Length();
			auto multiple_replace_correction = new MultipleNewMethodCorrectionProposal();
			for (auto i = 0; i < length; i++) {
				MethodSymbol* method = expanded_table->symbol_pool[i]->method_symbol;
				if (method && method->ACC_ABSTRACT())
				{
					TypeSymbol* containing_type = method->containing_type;
					if (containing_type == this_type)
						continue;

					auto  numFormalParameters = method->NumFormalParameters();
					
					vector<wstring> parameterNames;
					wstring containPkgName = containing_type->ContainingPackageName();
					boost::replace_all(containPkgName, L"/", L".");

					auto typeInJavaModel = namel_lookup_->findType(containing_type->sourceName('.'), containPkgName, false, JavaModel::NameLookup::ACCEPT_CLASSES | JavaModel::NameLookup::ACCEPT_INTERFACES);
					wstring selector = method->getNameString();
					if (typeInJavaModel) 
					{
						
						vector<wstring>  parameterTypes;
						for (auto k = 0; k < numFormalParameters; k++) {
							auto  type = method->FormalParameter(k)->Type();
							wstring  compoundName = type->getCompoundName();
							boost::replace_all(compoundName, L"/", L".");
							parameterTypes.push_back(compoundName);
						}
						
						auto methodInJavaModel =  typeInJavaModel->getMethod(selector, parameterTypes);
						if(methodInJavaModel){
							methodInJavaModel->getNameRange();
							parameterNames = methodInJavaModel->getParameterNames();
						}
					}

					if(parameterNames.empty()){
						for (auto k = 0; k < numFormalParameters; k++) {
							Jikes::wstringex temp;
							temp.format(L"parameter_%d", k);
							parameterNames.push_back(temp);
						}
					}
					
					auto newMethod = new NewMethodCompletionProposal(_fileName, projectName, containPkgName);
					newMethod->access->SetFlags(*method);
					newMethod->selector = selector;
					newMethod->constructor = false;
					newMethod->paramenterNames = parameterNames;
					newMethod->isInterface = this_type->ACC_INTERFACE();
					newMethod->left_line_no = line_no;
					newMethod->left_column_no = column_no;
					newMethod->right_line_no = line_no;
					newMethod->right_column_no = column_no;
					
					for (auto l = 0; l < numFormalParameters; ++l) {
						TypeSymbol* arg_type = method->FormalParameter(l)->Type();
						if (!arg_type || arg_type->Bad()) {
							newMethod->formal_parameterTypes.push_back(L"Object");
						}
						else {
							auto typeName = arg_type->getNameString();
							newMethod->formal_parameterTypes.push_back(typeName);
							wstring coumpoundName = arg_type->getCompoundName();
							boost::replace_all(coumpoundName, L"/", L".");
							if (importEditCtrl->AddImportEdit(coumpoundName)) {
								auto proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
								proposal->fReplacementString = L"import " + coumpoundName + L";\n";
								multiple_replace_correction->proposals.push_back(proposal);

							}
						}
					}
					auto returnTypeSymbol = method->Type();
					if(!returnTypeSymbol || returnTypeSymbol->Bad()){
						newMethod->returnTypeName = L"Object";
					}
					else{
						auto typeName = returnTypeSymbol->getNameString();
						newMethod->returnTypeName = typeName;
						wstring coumpoundName = returnTypeSymbol->getCompoundName();
						boost::replace_all(coumpoundName, L"/", L".");
						if (importEditCtrl->AddImportEdit(coumpoundName)) {
							auto proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
							proposal->fReplacementString = L"import " + coumpoundName + L";\n";
							multiple_replace_correction->proposals.push_back(proposal);
						}
					}
					multiple_replace_correction->method_proposals.push_back(newMethod);
				}
			}

			if (namel_lookup_)
				namel_lookup_->endToOccupy();

			if(multiple_replace_correction->method_proposals.size()){
				proposals.push_back(multiple_replace_correction);
				multiple_replace_correction->fDisplayString = L"Generate the missing method";
			}
			else{
				delete multiple_replace_correction;
			}
			delete importEditCtrl;
		}


		void ReorgCorrectionsSubProcessor::get_NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD_Proposals(ProblemPosition* problemPos, 
			std::vector<CorrectionProposal*>& proposals)
		{

			if (!problemPos || !problemPos->cur_info)
				return;

			auto& _info = *(problemPos->cur_info);
			ASTResolving* resolving = new ASTResolving(_info.line, _info.column, problemPos->pJm, problemPos->layer_pos->proj, _info.doc_path);
			if (!resolving->findSelectedNode()) {
				delete resolving;
				return;
			}

			auto visitor = resolving->visitor;
			if (visitor && visitor->astNode) {
				switch (visitor->completNodeType)
				{
				case CorrectionAssistASTVisitor::CompletionOnTypeIdentifier:
				{
					resolving->BindingProcess();	
					CompleteSymbolTable(problemPos ,resolving, proposals);

				}
				break;


				default:
					break;

				}
			}
			delete resolving;

		}

		void ReorgCorrectionsSubProcessor::getWrongPackageDeclNameProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{


		}

	

		void ReorgCorrectionsSubProcessor::removeImportStatementProposals(
			ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{
			if (!problemPos || !problemPos->pJm || !problemPos->layer_pos)
				return;
			Jikes::CodeAssist::AssistInfo* info = problemPos->cur_info;
			if (!info ||! info->extraData || !info->extraData->semanticError)
				return;

			auto java_project = problemPos->pJm->getJavaProject(problemPos->layer_pos->proj);
			if (!java_project)
				return;
			java_project->beginToOccupy();
			boost::shared_ptr<Jikes::JavaModel::BuildProblem > old_problem = java_project->problems[problemPos->cur_info->doc_path];
			java_project->endToOccupy();

		

			auto extraError = info->extraData->semanticError;


			auto replaceItem = new  ReplaceCorrectionProposal(
				extraError->left_line_no,
				extraError->left_column_no,
				extraError->right_line_no,
				extraError->right_column_no);

			replaceItem->icon_type = CorrectionProposal::REMOVE_ICON;
			replaceItem->fDisplayString = L"Remove unused import";

			proposals.push_back(replaceItem);

			auto& _data = old_problem->data;
			auto size = _data.size();
			vector<JavaModel::DefaultProblem*> unusedImport;
			for (int i = 0; i < size; i++){
				auto error = _data[i];
				if (!error)
					continue;
				auto errorInfo = reinterpret_cast<JavaModel::DefaultProblem*>(error);
				if (!JavaCorrectionProcessor::UnusedImportProblem(errorInfo->id))
					continue;
				unusedImport.emplace_back(errorInfo);
			}

			if (unusedImport.size() > 1)
			{
				auto proposal = new MultipleReplaceCorrectionProposal();
				Jikes::wstringex format;
				format.format(L"fix %d problem in the same category in file", unusedImport.size());
				proposal->fDisplayString = format;
				for (auto it : unusedImport)
				{
					replaceItem = new  ReplaceCorrectionProposal(
						it->getLeftColumnNo(),
						it->getLeftColumnNo(),
						it->getRightLineNo(),
						it->getRightColumnNo());

					proposal->proposals.push_back(replaceItem);
				}
				proposals.push_back(replaceItem);
			}
		
		}

		void ReorgCorrectionsSubProcessor::getMethodOverLoadProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{
			if (!problemPos || !problemPos->cur_info || !problemPos->layer_pos)
				return;

			auto& _info = *(problemPos->cur_info);
			ASTResolving* resolving = new ASTResolving(_info.line, _info.column, problemPos->pJm, problemPos->layer_pos->proj, _info.doc_path);
			if(!resolving->findSelectedNode())
			{
				delete resolving;
				
			}
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

