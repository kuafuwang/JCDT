#include "stdafx.h"
#include "UnresolvedElementsSubProcessor.h"
#include <string>
#include <CodeCorrectionLib/core/SimilarElement.h>
#include <CodeCorrectionLib/core/SimilarElementsRequestor.h>
#include <CodeCorrectionLib/core/ProblemPosition.h>
#include <JavaModelLib/internal/core/ImportDeclaration.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include <CodeCorrectionLib/Proposal/InsertCorrectionProposal.h>
#include <CodeCorrectionLib/Proposal/ReplaceCorrectionProposal.h>
#include <CodeCorrectionLib/Proposal/NewCUCompletionProposal.h>
#include <CodeCorrectionLib/Proposal/ProjrctSetupCorrectionProposal.h>
#include <CodeCorrectionLib/Proposal/MultipleReplaceCorrectionProposal.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <CodeAssistLib/impl/AssistInfo.h>

#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <CodeCorrectionLib/core/TypeSearchRequestor.h>
#include <CodeCorrectionLib/core/ASTResolving.h>
#include <JavaModelLib/internal/lookup/SourceMapperFile.h>
#include <JCDT_Lib/internal/ast/ScopeUtil.h>
#include <JCDT_Lib/internal/lookup/unitfilesymbol.h>
#include <CodeCorrectionLib/core/CorrectionAssistASTVisitor.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/problem/SemanticError.h>
#include <JCDT_Lib/internal/lookup/MethodShadowSymbol.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/lookup/ExpandedMethodTable.h>
#include "ImportEditCtrl.h"
#include <CodeCorrectionLib/Proposal/NewMethodCompletionProposal.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JCDT_Lib/internal/util/stringex.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/compiler/DefaultProblem.h>
using namespace  std;
using namespace  Jikes::JavaModel;
using namespace  Jikes::CodeAssist;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		AstImportDeclaration*  findImport(const wstring& simpleName, map<wstring, AstImportDeclaration*>& importDecls){
			for (auto currImport : importDecls){
				const wstring& curr = currImport.first;
				if (CharOperation::suffixEquals(simpleName, curr, true)){
					int dotPos = curr.length() - simpleName.length() - 1;
					if ((dotPos == -1) || (dotPos > 0 && curr[dotPos] == '.')) {
						return currImport.second;
					}
				}
			}
			return  nullptr;
		}
		std::pair< wstring , wstring  > getPackageAndSimpleName(const wstring& name){
			auto idx = name.rfind(L".");
			if(idx == wstring::npos){
				return{ L"",name };
			}
			
			auto simpleName =  name.substr(idx + 1, name.size());
			auto package = name.substr(0, idx);
			return{ package,simpleName };
			
		}


		void getTypeProposalResult(vector<wstring>& typeNames, int kind, int acceptFlags, ProblemPosition* problemPos,
			std::vector<CorrectionProposal*>& proposals, FileSymbol* lexFile)
		{
			if (!problemPos || !problemPos->layer_pos || !problemPos->pJm)
				return;

			auto typeName = typeNames[typeNames.size() - 1];
			wstring& projectName = problemPos->layer_pos->proj;

		
			auto pJm = problemPos->pJm;
			auto project = pJm->getJavaProject(projectName);
			if (!project)
				return;
			JavaModel::NameLookup* namel_lookup_ = project->getNameLookup();
			if (!namel_lookup_)
				return;

			TypeSearchRequestor*  requestor = new TypeSearchRequestor(new SimilarElementsRequestor(typeName, kind, 8, L""));
			namel_lookup_->seekTypes(boost::join(typeNames, L"."), nullptr, true, acceptFlags, requestor->nameLookupRequestor);



			auto&  elements = requestor->requestor->fResult;


			if (!elements.empty()) {
				project->beginToOccupy();
				int  insetLineNo = 0;
				
				auto unit = lexFile->compilation_unit;
				auto lex = lexFile->lex_stream;
				if(unit->package_declaration_opt){
					auto r_token = unit->package_declaration_opt->RightToken();
					if(r_token)
						insetLineNo = lex->LineFromPos(r_token->start_location) + 1;
				}
				map<wstring, AstImportDeclaration*>  importDecls;
				auto num = unit->NumImportDeclarations();
				for (unsigned i = 0; i < num; i++) {
					auto _import_decl = unit->ImportDeclaration(i);
					if (!_import_decl)
						continue;

					auto _import_name = _import_decl->getImportName('.');
					boost::to_lower(_import_name);
					importDecls.insert(make_pair(_import_name, _import_decl));
				}
			
				for (auto it : elements) {

					if (!it)
						continue;

					auto curr = it->fName;
					auto packageAndsimpleName = getPackageAndSimpleName(curr);
					auto simpleName = packageAndsimpleName.second;
					auto replaceName = simpleName;

					auto existingImport = findImport(simpleName, importDecls);

					if (existingImport != nullptr) {
						if (existingImport->getImportName('.') != curr) {
							// import for otehr type already exists -> use qualified
							replaceName = curr;
						}
					}
					else {

					}
					if (packageAndsimpleName.first != problemPos->layer_pos->projectPos.pkg){
						AccessFlags access;
						access.SetFlags(it->modifiers);
						if (!access.ACC_PUBLIC()){
							continue;
						}
					}
					bool  importOnly = replaceName == (typeName);
					if (!importOnly) {
						MultipleReplaceCorrectionProposal* import_correction = new  MultipleReplaceCorrectionProposal();
						import_correction->fDisplayString = L"change to " + replaceName + L" (" + curr + L")";

						auto proposal = new ReplaceCorrectionProposal(insetLineNo, 1, insetLineNo, 0);
						proposal->fReplacementString = L"import " + curr + L";\n";

						import_correction->proposals.push_back(proposal);

						proposal = new ReplaceCorrectionProposal(
							problemPos->left_line_no,
							problemPos->left_column_no,
							problemPos->right_line_no,
							problemPos->right_column_no);
						proposal->fReplacementString += replaceName;

						import_correction->proposals.push_back(proposal);
						import_correction->icon_type = CorrectionProposal::CHANGE_ICON;
						proposals.emplace_back(import_correction);
					}
					else {
						InsertCorrectionProposal* proposal = new InsertCorrectionProposal(insetLineNo, 0, L"import " + curr + L";\n");
						proposals.emplace_back(proposal);
						proposal->fDisplayString = proposal->fInsertionString;
						proposal->icon_type = CorrectionProposal::IMPORT_ICON;

					}
				}
				project->endToOccupy();
			}

			delete requestor;

			CorrectionProposal* proposal = new ProjrctSetupCorrectionProposal(problemPos->layer_pos->proj);
			proposal->fDisplayString = L"Fix project config .. ";
			proposals.push_back(proposal);
			proposal->icon_type = CorrectionProposal::CHANGE_ICON;


			wstring  addedCUName = typeName + L".java";


			wstring packageName;
			IPackageFragment* pkg = nullptr;
			if (typeNames.size() > 1) {
				vector<wstring> pkgNames(typeNames.begin(), typeNames.begin() + typeNames.size() - 1);
				packageName = boost::join(pkgNames, L".");

				auto nameLook =  project->getNameLookup();
				nameLook->beginToOccupy();
				auto pkgFragments =  nameLook->findPackageFragments(packageName, false);
				
				for(auto it : pkgFragments)
				{
					if(it && it->getKind() == IPackageFragmentRoot::K_SOURCE){
						pkg = it;
					}
				}
			}
			else{
				packageName = problemPos->layer_pos->projectPos.pkg;
				pkg = project->getPackageFrament(problemPos->layer_pos->projectPos.root, packageName);
			}


			if (pkg) {
				auto  addedCU = pkg->getCompilationUnit(addedCUName);
				if (addedCU) {
					return;
				}
			}
			else {
				return ;
			}

			proposal = new NewCUCompletionProposal(typeName, problemPos->layer_pos->proj, packageName);
			proposal->fDisplayString = L"Create class " + typeName;
			proposals.push_back(proposal);
			proposal->icon_type = CorrectionProposal::CLASS_ICON;
		}



		void UnresolvedElementsSubProcessor::getVariableProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{
			if (!problemPos || !problemPos->pJm || !problemPos->layer_pos || !problemPos->cur_info)
				return;

			auto pJm = problemPos->pJm;
			wstring& projectName = problemPos->layer_pos->proj;
			auto project = pJm->getJavaProject(projectName);
			if (!project)
				return;

			auto& _info = *(problemPos->cur_info);
			ASTResolving* resolving = new ASTResolving(_info.line, _info.column, pJm, projectName, _info.doc_path);
			boost::shared_ptr<ASTResolving> pRef(resolving);

			if (!resolving->findSelectedNode()){
				
				return ;
			}

			auto visitor = resolving->visitor;
			bool stop = false;
			Token* identifier_token = nullptr;
			bool onlyFieldProposal = false;
			if (visitor->astNode){
				switch (visitor->completNodeType)
				{
				case CorrectionAssistASTVisitor::CompletionOnMemberAccess: {
					auto selectedNode = reinterpret_cast<FieldReference*>(visitor->astNode);
					if(selectedNode->base == nullptr || dynamic_cast<AstThisExpression*>(selectedNode->base)){
						identifier_token = selectedNode->identifier_token;
						if(selectedNode->base != nullptr)
						{
							onlyFieldProposal = true;
						}
					}
					else{
						stop = true;
					}
				}
				break;
				case CorrectionAssistASTVisitor::CompletionOnNameReference: 
				{
					auto selectedNode = reinterpret_cast<AstName*>(visitor->astNode);
					if(selectedNode->IsQualifiedName()){

						stop = true;
					}
					else{
						if (!selectedNode->identifier_token)
							break;
						auto	acceptFlags = NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES;
						vector<wstring> typeNames = { selectedNode->identifier_token->getName() };

						getTypeProposalResult(typeNames, SimilarElementsRequestor::REF_TYPES, acceptFlags,
							problemPos, proposals, resolving->lexFile);

						identifier_token = selectedNode->identifier_token;
					}

				}	
				break;
		
				default:
					stop = true;
					break;

				}
			}

		    if(stop ||! visitor->astNode || !resolving->lexFile || !resolving->lexFile->lex_stream || !resolving->lexFile->compilation_unit){
				
				return;
		    }
			auto lex = resolving->lexFile->lex_stream;
			
			auto unit = resolving->lexFile->compilation_unit;
			vector<SimilarElement*> elements;
			wstring   name =  identifier_token->getName();
			SimilarElementsRequestor::findSimilarVariableElement(elements, problemPos, name,SimilarElementsRequestor::VARIABLES);
			auto size = elements.size();
			if(size)
			{	
				if(!identifier_token)
				{
					return;
				}
				auto  left_line_no = lex->LineFromPos(identifier_token->start_location);
				auto left_column_no = lex->ColumnFromPos(identifier_token->start_location);
				auto  right_line_no = lex->LineFromPos(identifier_token->start_location);
				auto  right_column_no = lex->RightColumnFromPos(identifier_token->EndLocation());
				for (int i = 0; i < size; i++) {
					SimilarElement* curr = elements[i];
					if (!curr)
						continue;

					auto propasl = new ReplaceCorrectionProposal(left_line_no,left_column_no,right_line_no, right_column_no);
					propasl->fReplacementString = curr->fName;
					propasl->fDisplayString = L"Change to " + curr->fName;
					proposals.push_back(propasl);

				}
			}

			if(!visitor->completeScope)
			{
				return;
			}

			
			resolving->BindingProcess();
			auto insertType1 = resolving->getTypeBinding();
			if(!insertType1)
			{
				return;
			}
			bool notNeedInsertImportDecl ;
			ImportEditCtrl* importEditCtrl = new ImportEditCtrl(lex,unit);
			wstring compoundName = insertType1->getCompoundName();
			if(importEditCtrl->AddImportEdit(compoundName)){
				notNeedInsertImportDecl = false;
			}
			else{
				notNeedInsertImportDecl = true;
			}
			wstring insertType_name_string = insertType1->getNameString();

		

			ReplaceCorrectionProposal * importProposal = nullptr;
		
			if (!notNeedInsertImportDecl) {			
				importProposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
				importProposal->fReplacementString = L"import " + compoundName + L";\n";
			}
			delete importEditCtrl;


			auto method_scope = ScopeUtil::enclosingMethod(visitor->completeScope);
			auto method = dynamic_cast<AbstractMethodDeclaration*>(method_scope);

			auto scope = ScopeUtil::enclosingType(visitor->completeScope);
			if (scope){
				auto declaration = dynamic_cast<TypeDeclaration*>(scope);
				int left_line_no;
				int left_column_no;

				auto class_body = declaration->class_body;
				if(!class_body)
				{
					return;
				}
				unsigned int end_loc = 0;
				Token* rightToken = nullptr;
				auto num = class_body->NumInstanceVariables();
				if (num) {
					auto field = class_body->InstanceVariable(num - 1);

					if (!field)
						return;

					rightToken = field->RightToken();

					if (!rightToken)
						return;

					end_loc = rightToken->EndLocation();
				}
				num = class_body->NumClassVariables();

				if (num) {
					auto field = class_body->ClassVariable(num - 1);

					if (!field)
						return;

					if (!field->RightToken())
						return;

					auto end_loc_temp = field->RightToken()->EndLocation();
					if (end_loc_temp > end_loc) {
						end_loc = end_loc_temp;
						rightToken = field->RightToken();
					}
				}

				if (!end_loc) {
					rightToken = class_body->left_brace_token;
					if (!rightToken)
						return;

					end_loc = class_body->left_brace_token->EndLocation();
					
				}

				left_line_no = lex->LineFromPos(end_loc);

				left_column_no = lex->RightColumnFromPos(end_loc);

				wstring rightTokenString = rightToken->getName();

				wstring insert_string;
				insert_string.push_back(rightTokenString[rightTokenString.size() - 1]);
				insert_string.push_back('\n');
				insert_string.reserve(left_column_no + 10);
				for (auto i = 0; i < left_column_no; ++i) {
					insert_string.push_back(' ');
				}

				auto proposal = new ReplaceCorrectionProposal(left_line_no, left_column_no, left_line_no, left_column_no);
				
				if(method && method->method_symbol && method->method_symbol->ACC_STATIC()){
					insert_string += L"static ";
				}

				insert_string += L"public " + insertType_name_string + L" " + name + L";";
				proposal->fReplacementString = insert_string;

				auto multiple_replace_correction = new MultipleReplaceCorrectionProposal();
				if (importProposal) {
					importProposal = importProposal->Clone();
					multiple_replace_correction->proposals.push_back(importProposal);
				}

				multiple_replace_correction->proposals.push_back(proposal);

				multiple_replace_correction->fDisplayString = L"Create field " + name;

				multiple_replace_correction->icon_type = CorrectionProposal::CLASS_ICON;

				proposals.push_back(multiple_replace_correction);

			}

			if (onlyFieldProposal)
			{
				delete 	importProposal;
				return;
			}
			if (!method)
				return;
	   
			// 添加local
			// 添加field
			AstMethodBody* body;
			if(method->kind != Ast::CONSTRUCTOR){
				body = reinterpret_cast<MethodDeclaration*>(method)->method_body_opt;
			}
			else{
				body = reinterpret_cast<ConstructorDeclaration*>(method)->constructor_body;
			}
			if (!body)
				return;

			int left_line_no;
			int left_column_no;
			Token* leftToken = nullptr;
			if(body->NumStatements()){
				auto statement = body->GetStatement(0);
				if (!statement)
					return;
				leftToken = statement->LeftToken();		 
			}
			else{
				leftToken = body->left_brace_token;
			}

			if (!leftToken)
				return;

			wstring   insert_string;
			left_line_no = lex->LineFromPos(leftToken->start_location);
			left_column_no = lex->ColumnFromPos(leftToken->start_location);
			auto multiple_replace_correction = new MultipleReplaceCorrectionProposal();
			if (importProposal){
				importProposal = importProposal->Clone();
				multiple_replace_correction->proposals.push_back(importProposal);	
			}
			
			auto proposal = new ReplaceCorrectionProposal(left_line_no, left_column_no, left_line_no, left_column_no);
			wstring temp(left_column_no > 0 ? left_column_no - 1:0,' ');		
			insert_string += insertType_name_string + L" " + name + L";\n";
			insert_string += temp;
			insert_string.push_back(leftToken->getName()[0]);
			proposal->fReplacementString = insert_string;

			multiple_replace_correction->proposals.push_back(proposal);
			multiple_replace_correction->fDisplayString = L"Create local variable " + name;
			multiple_replace_correction->icon_type = CorrectionProposal::CLASS_ICON;
			proposals.push_back(multiple_replace_correction);

			multiple_replace_correction = new MultipleReplaceCorrectionProposal();
			if (importProposal){
				multiple_replace_correction->proposals.push_back(importProposal);
				importProposal = nullptr;
			}
			// 添加 parameter
			auto start_location = method->method_declarator->left_parenthesis_token->start_location;
			left_line_no = lex->LineFromPos(start_location);
			left_column_no = lex->ColumnFromPos(start_location);

			proposal = new ReplaceCorrectionProposal(left_line_no, left_column_no, left_line_no, left_column_no);
			insert_string = L"( " + insertType_name_string + L" " + name +  L", ";
			proposal->fReplacementString = insert_string;


			multiple_replace_correction->proposals.push_back(proposal);

			multiple_replace_correction->fDisplayString = L"Create parameter " + name;
			multiple_replace_correction->icon_type = CorrectionProposal::CLASS_ICON;
			proposals.push_back(multiple_replace_correction);

		
			delete importProposal;
		}

		void UnresolvedElementsSubProcessor::getTypeProposals(
			ProblemPosition* problemPos,
			int kind,
			std::vector<CorrectionProposal*>& proposals
		)
		{
			if (!problemPos || !problemPos->cur_info)
				return;

			auto& _info = *(problemPos->cur_info);
			ASTResolving* resolving = new ASTResolving(_info.line, _info.column, problemPos->pJm, problemPos->layer_pos->proj, _info.doc_path);

			auto   typeNames = resolving->GetTypeNames();

			auto visitor = resolving->visitor;
			auto assistNodeIsClass = visitor->nextTypeReferenceIsClass;
			auto assistNodeIsInterface = visitor->nextTypeReferenceIsInterface;
			auto assistNodeIsException = visitor->nextTypeReferenceIsException;
		
			if (typeNames.empty()){
				delete resolving; return;
			}
				
			int acceptFlags;
			if (assistNodeIsClass || assistNodeIsException) {
				acceptFlags = NameLookup::ACCEPT_CLASSES;
			}
			else if (assistNodeIsInterface) {
				acceptFlags = NameLookup::ACCEPT_INTERFACES;
			}
			else {
				acceptFlags = NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES;
			}

			getTypeProposalResult(typeNames, kind, acceptFlags, problemPos, proposals, resolving->lexFile);
			delete resolving;
		}

		wstring getParameterNameFromParameterAst(Expression* argment,unsigned i)
		{
			Jikes::wstringex parameterName;
			if (!argment)
				return parameterName;

			switch (argment->kind)
			{
				case Ast::NAME: {
					auto identifier_token = reinterpret_cast<AstName*>(argment)->identifier_token;
					if(identifier_token)
						parameterName = identifier_token->getName();

				}
				break;

				case Ast::CAST:{
					auto castExpresion = reinterpret_cast<CastExpression*>(argment)->expression;
					return  getParameterNameFromParameterAst(castExpresion, i);
				}
				break;

				case Ast::DOT:
				{
					auto identifier_token = reinterpret_cast<FieldReference*>(argment)->identifier_token;
					parameterName = identifier_token->getName();
				}
				default:

					parameterName.format(L"parameter_%d", i);
					break;

			}
			return parameterName;

		}

		void getMethodProposal(ASTResolving* resolving,MethodSymbol* best_match, AstArguments* arguments,std::vector<CorrectionProposal*>& proposals)
		{
			if (!resolving || !resolving->lexFile || !resolving->lexFile->lex_stream || !resolving->lexFile->compilation_unit)
				return;

			if (!best_match)
				return;

			auto lex = resolving->lexFile->lex_stream;

			auto  unit = resolving->lexFile->compilation_unit;
			const int numArgumens = arguments->NumArguments();
			auto importEditCtrl = new ImportEditCtrl(lex, unit);
			ImportEditCtrl* allImportEditCtrl = importEditCtrl->Clone();
			MultipleReplaceCorrectionProposal* all_parameter_correction = new  MultipleReplaceCorrectionProposal();
			int numBestMatchAiguments = best_match->NumFormalParameters();
			int minArguments = numArgumens > numBestMatchAiguments ? numBestMatchAiguments : numArgumens;
			int affectArguments = 0;
			for (auto i = 0; i < minArguments; ++i)
			{
				auto _temp_parameter = best_match->FormalParameter(i);
				if (!_temp_parameter)
					continue;

				auto bestMatchParameterSymbol = _temp_parameter->Type();
				if (!bestMatchParameterSymbol || bestMatchParameterSymbol->Bad()) {
					continue;
				}
				wstring best_match_symbol_compoundName = bestMatchParameterSymbol->getCompoundName();
				auto arg = arguments->Argument(i);
				if (!arg || arg->Type() == bestMatchParameterSymbol) {
					continue;
				}
				++affectArguments;


				wstring parameterText;
				auto leftToken = arg->LeftToken();
				auto rightToken = arg->RightToken();

				if (!leftToken || !rightToken)
					continue;

				lex->GetText(leftToken->start_location, rightToken->EndLocation() + 1, parameterText);
				MultipleReplaceCorrectionProposal* import_correction = new  MultipleReplaceCorrectionProposal();

				auto typeName = bestMatchParameterSymbol->getNameString();
				import_correction->fDisplayString = L"cast parameter " + parameterText + L" to type " + typeName;
				ReplaceCorrectionProposal* proposal;
				if (importEditCtrl->IsNeedToImportEdit(best_match_symbol_compoundName)) {
					proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);

					proposal->fReplacementString = L"import " + best_match_symbol_compoundName + L";\n";
					import_correction->proposals.push_back(proposal);
				}

				if (allImportEditCtrl->AddImportEdit(best_match_symbol_compoundName))
				{
					proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);

					proposal->fReplacementString = L"import " + best_match_symbol_compoundName + L";\n";
					all_parameter_correction->proposals.push_back(proposal);
				}

				auto left_line_no = lex->LineFromPos(leftToken->start_location);
				auto left_column_no = lex->ColumnFromPos(leftToken->start_location);

				proposal = new ReplaceCorrectionProposal(
					left_line_no,
					left_column_no,
					left_line_no,
					left_column_no);



				proposal->fReplacementString.append(L"(" + typeName + L")");
				proposal->fReplacementString.push_back(leftToken->getName()[0]);

				import_correction->proposals.push_back(proposal);


				import_correction->icon_type = CorrectionProposal::CHANGE_ICON;
				proposals.emplace_back(import_correction);
				all_parameter_correction->proposals.emplace_back(proposal->Clone());
			}
			delete allImportEditCtrl;
			delete 	 importEditCtrl;

			if (affectArguments <= 1) {
				delete all_parameter_correction;
			}
			else {
				all_parameter_correction->fDisplayString = L"Cast all parameter";
				all_parameter_correction->icon_type = CorrectionProposal::CHANGE_ICON;
				proposals.emplace_back(all_parameter_correction);
			}
		}



		void ReportMethodNotFound(ProblemPosition* problemPos , ASTResolving* resolving,std::vector<CorrectionProposal*>& proposals  ){
			if (!problemPos || !resolving || !resolving->visitor || !resolving->visitor->astNode)
				return;

			int problemId = problemPos->problemId;
		   auto	method_call = reinterpret_cast<AstMethodInvocation *>(resolving->visitor->astNode);
			TypeSymbol* type = nullptr;
			Expression* base = method_call->base_opt;
			if(base && base->Type()->file_symbol != nullptr){//&& base->symbol != nullptr
				//auto kind = base->symbol->_kind;
				//if(kind == Symbol::METHOD)
				//{
				//	type = base->Type();
				//}
				//else if(kind == Symbol::VARIABLE)
				//{
				//	type = base->Type();
				//}
				//else if(kind == Symbol::TYPE)
				//{
				//	type = base->TYPE;
				//}
				////type = base->Type();
				//else
				//{
				//	type = ScopeUtil::enclosingSourceType(resolving->visitor->completeScope);
				//}
				type = base->Type();
			}
			else{
				type = ScopeUtil::enclosingSourceType(resolving->visitor->completeScope);
			}

			if (!resolving->lexFile || !resolving->lexFile->lex_stream || !resolving->lexFile->semantic)
				return;
			auto lex = resolving->lexFile->lex_stream;

			auto semantic = resolving->lexFile->semantic;
		
			Token* id_token = method_call->identifier_token;
			if (!id_token)
				return;

			NameSymbol* name_symbol = id_token->GetNameSymbol();
			MethodShadowSymbol* method_shadow;	
			// Search for an accessible method with different arguments. Favor the
			// earliest method found with the smallest difference in parameter count.
			// Since the JVMS limits methods to 255 parameters, we initialize our
			// difference detection with 255.
			MethodSymbol* best_match = NULL;
			
			auto  unit = resolving->lexFile->compilation_unit;

			if (!method_call->arguments)
				return;

			const int numArgumens = method_call->arguments->NumArguments();
			if(problemId == IProblem::UndefinedMethod ||
				problemPos->problemId == IProblem::ParameterMismatch 
				)
			{
				auto typeScope = ScopeUtil::enclosingType(resolving->visitor->completeScope);
				if (!typeScope )
					return;

				auto thisType = dynamic_cast<TypeDeclaration*>(typeScope)->symbol;
				if (!thisType || thisType->Bad())
					return;
				for (; typeScope;){
					if (base){
						type = base->Type();
						typeScope = nullptr;
					}
					else{	
						typeScope = ScopeUtil::enclosingType(typeScope->parent_scope);
					}
					if (!type)
						continue;

					if (!type->expanded_method_table)
						semantic->ComputeMethodsClosure(type, method_call->identifier_token);

					if (!type->expanded_method_table)
						return;


					int difference = 255;
					for (method_shadow = type->expanded_method_table->FindMethodShadowSymbol(name_symbol);
						method_shadow; method_shadow = method_shadow->next_method)
					{
						MethodSymbol* method = method_shadow->method_symbol;

						if (!method)
							continue;

						if (!method->IsTyped())
							method->ProcessMethodSignature(semantic, id_token);
						if (semantic->MemberAccessCheck(type, method, thisType, base) ||
							method_shadow->NumConflicts() > 0)
						{
							int diff = method_call->arguments->NumArguments() -
								method->NumFormalParameters();
							if (diff < 0)
								diff = -diff;
							if (diff < difference)
							{
								best_match = method;
								difference = diff;
							}
						}
					}
					if (best_match){
						auto  best_match_name = best_match->getNameString();
						if (best_match_name == name_symbol->Name()){
							getMethodProposal( resolving,best_match, method_call->arguments, proposals);
						}
						else
						{
							// change 的干活

							auto identifer = method_call->identifier_token;
							if (!identifer)
								return;

							auto  left_line_no = lex->LineFromPos(identifer->start_location);
							auto left_column_no = lex->ColumnFromPos(identifer->start_location);
							auto right_line_no = lex->LineFromPos(identifer->EndLocation());
							auto right_column_no = lex->RightColumnFromPos(identifer->EndLocation());
							//method_call
							auto proposal = new ReplaceCorrectionProposal(
								left_line_no,
								left_column_no,
								right_line_no,
								right_column_no);

							proposal->fReplacementString += best_match_name;
							proposal->fDisplayString = L"Change to " + best_match_name;

							proposal->icon_type = CorrectionProposal::CHANGE_ICON;
							proposals.emplace_back(proposal);

						}
						best_match = nullptr;
					}

				}
				
			}
			//else if(SemanticError::METHOD_NAME_MISSPELLED == problemId)
			//{
			//	// Search for a misspelled method name.
			//	auto typeScope = ScopeUtil::enclosingType(resolving->visitor->completeScope);
			//	if (!typeScope)
			//		return;

			//	auto thisType = dynamic_cast<TypeDeclaration*>(typeScope)->symbol;
			//	if (!thisType || thisType->Bad())
			//		return;

			//	for (; typeScope;) {
			//		if (base) {
			//			type = base->Type();
			//			typeScope = nullptr;
			//		}
			//		else {
			//			type = dynamic_cast<TypeDeclaration*>(typeScope)->symbol;
			//			typeScope = ScopeUtil::enclosingType(typeScope->parent_scope);
			//		}
			//		best_match = semantic->FindMisspelledMethodName(type, method_call, name_symbol, thisType);
			//		if (best_match) {
			//			auto  best_match_name = best_match->getNameString();
			//			// change 的干活

			//			auto identifer = method_call->identifier_token;
			//			if (!identifer)
			//				return;

			//			auto  left_line_no = lex->LineFromPos(identifer->start_location);
			//			auto left_column_no = lex->ColumnFromPos(identifer->start_location);
			//			auto right_line_no = lex->LineFromPos(identifer->EndLocation());
			//			auto right_column_no = lex->RightColumnFromPos(identifer->EndLocation());
			//			//method_call
			//			auto proposal = new ReplaceCorrectionProposal(
			//				left_line_no,
			//				left_column_no,
			//				right_line_no,
			//				right_column_no);

			//			proposal->fReplacementString += best_match_name;
			//			proposal->fDisplayString = L"Change to " + best_match_name;

			//			proposal->icon_type = CorrectionProposal::CHANGE_ICON;
			//			proposals.emplace_back(proposal);

			//		}
			//	}
			//}

			if (!type || !type->file_symbol)
				return;

			auto _fileName = type->file_symbol->getNameString();
			auto pJm = problemPos->pJm;
			auto project =
				pJm->getJavaProject(problemPos->layer_pos->proj);
			if (!project)
				return;
			JavaModel::NameLookup* namel_lookup_ = project->getNameLookup();
			wstring containPkgName = type->ContainingPackageName();
			boost::replace_all(containPkgName, L"/", L".");
			namel_lookup_->beginToOccupy();
			
			auto typeInJavaModel =  namel_lookup_->findType(type->sourceName('.'),containPkgName, false, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			if(!typeInJavaModel || typeInJavaModel->isBinary()){
				namel_lookup_->endToOccupy();
				return;
			}
		
			auto proj = typeInJavaModel->getJavaProject();
			if(!proj){
				namel_lookup_->endToOccupy();
				return;
			}

			auto projectName = proj->getElementName();
		

			 // 找到插入的位置

			auto declaringType = typeInJavaModel->getDeclaringType();
			if(!declaringType)
			{
				namel_lookup_->endToOccupy();
				return;
			}
			auto cunit = dynamic_cast< CompilationUnit* >(declaringType->getParent());
			if(!cunit){
				namel_lookup_->endToOccupy();
				return;
			}
			int left_line_no;
			int left_column_no;
			int right_line_no;
			int right_column_no;
			auto methods = typeInJavaModel->getMethods();
			wchar_t replaceChar;
			if(methods)
			{
				auto lastMethod = methods->operator[](methods->size() - 1);
				auto rng =  lastMethod->getSourceRange();
				left_line_no = rng->m_nEndLine;
				left_column_no = rng->m_nEndIndex + 1;
				right_line_no = rng->m_nEndLine;
				right_column_no = rng->m_nEndIndex + 1;
				
			}
			else
			{
				auto rng = typeInJavaModel->getSourceRange();
				left_line_no = rng->m_nEndLine;
				left_column_no = rng->m_nEndIndex;
				right_line_no = rng->m_nEndLine;
				right_column_no = rng->m_nEndIndex;
	
			}
			auto importEditCtrl = new ImportEditCtrl(cunit);
			auto newMethod = new NewMethodCompletionProposal(_fileName, projectName, containPkgName);

		
			auto tempMethod = dynamic_cast<AbstractMethodDeclaration*>(ScopeUtil::enclosingMethod(resolving->visitor->completeScope)) ;

			if(tempMethod && tempMethod->method_symbol){
				newMethod->access->SetFlags(tempMethod->method_symbol->Flags());
			}
			newMethod->path = cunit->getPath();
			newMethod->isInterface = type->ACC_INTERFACE();
			newMethod->selector = id_token->getName();
			newMethod->constructor = false;

			newMethod->left_line_no = left_line_no;
			newMethod->left_column_no = left_column_no;
			newMethod->right_line_no = right_line_no;
			newMethod->right_column_no = right_column_no;
			newMethod->icon_type = CorrectionProposal::CLASS_ICON;
			newMethod->fDisplayString = L"Create method " + newMethod->selector;
			AstArguments* args = method_call->arguments;
		
			for (auto i = 0; i < numArgumens; ++i){

				auto argment = args->Argument(i);

				if (!argment)
					return;

				newMethod->paramenterNames.push_back( getParameterNameFromParameterAst(argment, i) );
				TypeSymbol* arg_type = argment->Type();
			    if(!arg_type || arg_type->Bad()) {
					newMethod->formal_parameterTypes.push_back(L"Object");
			    }
				else{
					auto typeName = arg_type->getNameString();
					newMethod->formal_parameterTypes.push_back(typeName );
					wstring import_name = arg_type->getCompoundName();
					boost::replace_all(import_name, L"/", L".");
					if (importEditCtrl->AddImportEdit(import_name)) {
						auto proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
						proposal->fReplacementString = L"import " + import_name + L";\n";
						newMethod->proposals.push_back(proposal);

					}
				}
			}

			if(method_call->symbol && method_call->symbol->_kind == Symbol::METHOD)
			{
				auto methodSymbol = reinterpret_cast<MethodSymbol*>(method_call->symbol);
				auto retunrType = methodSymbol->Type();
				if(retunrType && !retunrType->Bad()){
					newMethod->returnTypeName = retunrType->getNameString();
					wstring import_name = retunrType->getCompoundName();
					boost::replace_all(import_name, L"/", L".");
					if (importEditCtrl->AddImportEdit(import_name)) {
						auto proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
						proposal->fReplacementString = L"import " + import_name + L";\n";
						newMethod->proposals.push_back(proposal);
					}
				}
				else{
					newMethod->returnTypeName = L"void";
				}

			}
			else{
				auto temp_binding = resolving->getMethodInvocatePossibleTypeBinding();
				if(temp_binding)
					newMethod->returnTypeName = temp_binding->getNameString();
			}
			proposals.push_back(newMethod);
			namel_lookup_->endToOccupy();

		}

		void ReportConstructorNotFound(ProblemPosition* problemPos, ASTResolving* resolving,
			std::vector<CorrectionProposal*>& proposals, Ast* ast, TypeSymbol* type)
		{

			if (!problemPos || !resolving  || !resolving->lexFile || !resolving->lexFile->semantic)
				return;

			auto semantic = resolving->lexFile->semantic;
			auto class_creation =AstClassCreationExpression::ClassCreationExpressionCast(ast);
			AstSuperCall* super_call = AstSuperCall::SuperCallCast(ast);
			AstArguments* args;
			if (class_creation){
				args = class_creation->arguments;
				if (class_creation->anonymous_type_opt)
					class_creation = NULL;
			}
			else if (super_call){
				args = super_call->arguments;
			}
			else{
				AstThisCall* this_call = AstThisCall::ThisCallCast(ast);
				assert(this_call);
				args = this_call->arguments;	
			}
			auto numArgumens = args->NumArguments();
			if (problemPos->problemId == IProblem::ParameterMismatch ||
				problemPos->problemId == IProblem::UndefinedConstructor){
				// Search for an accessible constructor with different arguments. Favor
				// the earliest ctor found with the smallest difference in parameter count.
				// Since the JVMS limits methods to 255 parameters, we initialize our
				// difference detection with 255.
				MethodSymbol* best_match = NULL;
				MethodSymbol* ctor;
				int difference = 255;
				auto typeScope = ScopeUtil::enclosingType(resolving->visitor->completeScope);
				if (!typeScope)
					return;

				auto thisType = dynamic_cast<TypeDeclaration*>(typeScope)->symbol;
				if (!thisType || thisType->Bad())
					return;
				for (ctor = type->FindMethodSymbol(semantic->lookup_env.nameEnv->init_name_symbol); ctor; ctor = ctor->next_method){
					if (semantic->ConstructorAccessCheck(ctor, !class_creation, thisType)) {
						int diff = numArgumens - ctor->NumFormalParameters();
						if (diff < 0)
							diff = -diff;
						if (diff < difference){
							best_match = ctor;
							difference = diff;
						}
					}
				}
				if (best_match) {
					AstArguments* arguments;
					if (class_creation) {
						arguments = class_creation->arguments;
					}
					else if (super_call) {
						arguments = super_call->arguments;
					}
					else {
						AstThisCall* this_call = AstThisCall::ThisCallCast(ast);
						arguments = this_call->arguments;
					}
					getMethodProposal(resolving, best_match, arguments, proposals);


				}

			}
			
			if (!type || !type->file_symbol)
				return;

			auto _fileName = type->file_symbol->getNameString();
			auto pJm = problemPos->pJm;
			if (!pJm)
				return;

			auto project =
				pJm->getJavaProject(problemPos->layer_pos->proj);
			if (!project)
				return;
			JavaModel::NameLookup* namel_lookup_ = project->getNameLookup();
			wstring containPkgName = type->ContainingPackageName();
			boost::replace_all(containPkgName, L"/", L".");
			namel_lookup_->beginToOccupy();

			auto typeInJavaModel = namel_lookup_->findType(type->sourceName('.'), containPkgName, false, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES);
			if (!typeInJavaModel || typeInJavaModel->isBinary()) {
				namel_lookup_->endToOccupy();
				return;
			}

			auto proj = typeInJavaModel->getJavaProject();
			if (!proj) {
				namel_lookup_->endToOccupy();
				return;
			}

			auto projectName = proj->getElementName();


			// 找到插入的位置

			auto declaringType = typeInJavaModel->getDeclaringType();
			if (!declaringType)
			{
				namel_lookup_->endToOccupy();
				return;
			}
			auto cunit = dynamic_cast< CompilationUnit* >(declaringType->getParent());
			if (!cunit) {
				namel_lookup_->endToOccupy();
				return;
			}
			int left_line_no;
			int left_column_no;
			int right_line_no;
			int right_column_no;
			auto methods = typeInJavaModel->getMethods();
			
			if (methods){
				auto lastMethod = methods->operator[](methods->size() - 1);
				if (!lastMethod)
					return;

				auto rng = lastMethod->getSourceRange();
				if (!rng)
					return;
				left_line_no = rng->m_nEndLine;
				left_column_no = rng->m_nEndIndex + 1;
				right_line_no = rng->m_nEndLine;
				right_column_no = rng->m_nEndIndex  + 1;
			}
			else{
				auto rng = typeInJavaModel->getSourceRange();
				if (!rng)
					return;
				left_line_no = rng->m_nEndLine;
				left_column_no = rng->m_nEndIndex;
				right_line_no = rng->m_nEndLine;
				right_column_no = rng->m_nEndIndex;
			}
			auto importEditCtrl = new ImportEditCtrl(cunit);
			auto newMethod = new NewMethodCompletionProposal(_fileName, projectName, containPkgName);
			newMethod->left_line_no = left_line_no;
			newMethod->left_column_no = left_column_no;
			newMethod->right_line_no = right_line_no;
			newMethod->right_column_no = right_column_no;
			newMethod->constructor = true;
			newMethod->selector = typeInJavaModel->getElementName();
			for (auto i = 0; i < numArgumens; ++i) {
				auto argment = args->Argument(i);

				if (!argment)
					continue;

				newMethod->paramenterNames.push_back(getParameterNameFromParameterAst(argment, i));
				TypeSymbol* arg_type = argment->Type();
				if (!arg_type || arg_type->Bad()) {
					newMethod->formal_parameterTypes.push_back(L"Object");
				}
				else {
					auto typeName = arg_type->getNameString();
					newMethod->formal_parameterTypes.push_back(typeName);
					wstring pkgName = arg_type->ContainingPackageName();
					boost::replace_all(pkgName, L"/", L".");
					if (importEditCtrl->IsNeedToImportEdit(pkgName)) {
						auto proposal = new ReplaceCorrectionProposal(importEditCtrl->packageLineNo, 1, importEditCtrl->packageLineNo, 0);
						proposal->fReplacementString = L"import " + pkgName + L";\n";
						newMethod->proposals.push_back(proposal);
					}
				}
			}	
			newMethod->path = cunit->getPath();
			newMethod->fDisplayString = L"Create constructor " + newMethod->selector;
			newMethod->icon_type = CorrectionProposal::CLASS_ICON;
			proposals.push_back(newMethod);
			namel_lookup_->endToOccupy();
		}

		

		void UnresolvedElementsSubProcessor::getMethodProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals)
		{
			if (!problemPos || !problemPos->cur_info || !problemPos->layer_pos)
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
				case CorrectionAssistASTVisitor::CompletionOnMessageSend:{
						resolving->BindingProcess();
						ReportMethodNotFound(problemPos,resolving, proposals);
				}
				break;

				
				case CorrectionAssistASTVisitor::CompletionOnClassCreationExpressionOnTypeName:{
					resolving->BindingProcess();
					auto createExpreNode = reinterpret_cast<AstClassCreationExpression*>(visitor->astNode);
					if(createExpreNode->class_type)
						ReportConstructorNotFound(problemPos,resolving, proposals,visitor->astNode, createExpreNode->class_type->symbol);
				}
				break;

				case CorrectionAssistASTVisitor::CompletionOnExplicitSuperConstructorCall:{
					resolving->BindingProcess();
					auto typeSymbol = ScopeUtil::enclosingSourceType(visitor->completeScope);
					ReportConstructorNotFound(problemPos,resolving, proposals,visitor->astNode, typeSymbol->super);
					
				}
				break;

				case CorrectionAssistASTVisitor::CompletionOnExplicitThisConstructorCall:{
					resolving->BindingProcess();
					auto typeSymbol =  ScopeUtil::enclosingSourceType(visitor->completeScope);
					ReportConstructorNotFound(problemPos, resolving, proposals, visitor->astNode,typeSymbol);
					
				}
				break;

				default:
				   break;

				}
			}
			delete resolving;
			
		
		}

	
	} // Close namespace Correction block

} // Close namespace Jikes block

