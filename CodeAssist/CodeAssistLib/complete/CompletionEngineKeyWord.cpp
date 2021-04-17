#include "stdafx.h"
#include "CompletionEngine.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"

#include <JCDT_Lib/internal/lex/Token.h>
#include "JCDT_Lib/internal/lex/LexStream.h"

#include <CodeAssistLib/impl/AstNoFound.h>
#include <JCDT_Lib/internal/parser/ParserUtil.h>
#include <JCDT_Lib/internal/parser/ParserUtil.h>

#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include  <boost/algorithm/string.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <CodeAssistLib/impl/AssistInfo.h>

#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/SourceField.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/Initializer.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/internal/lookup/SearchableEnvironmentRequestor.h>
#include "CompleteAssistASTVisitor.h"
#include <CodeAssistLib/impl/RelevanceConstants.h>
#include <JCDT_Lib/internal/ast/AstPackageDeclaration.h>
#include <JCDT_Lib/internal/ast/AstImportDeclaration.h>
#include <JCDT_Lib/internal/ast/ASTNode.h>
#include <CodeAssistLib/impl/ISearchableNameEnvironment.h>
#include <JCDT_Lib/internal/ast/ScopeUtil.h>
#include <JCDT_Lib/internal/lookup/BlockSymbol.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <CodeAssistLib/impl/CodeAssistUtil.h>
#include <JCDT_Lib/internal/util/stringex.h>
#include "ICompletionRequestor.h"
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/lookup/UnitFileSymbol.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include "CompleteLex.h"
#include "JCDT_Lib/internal/parser/diagnose/diagnose.h"
#include <JavaModelLib/internal/core/JavaModel.h>
#include <queue>
#include "ResultCollector.h"
#include "Proposal/PackageCompletionProposal.h"
#include "Proposal/IfCompletionProposal.h"
#include "Proposal/TryCompletionProposal.h"
#include "Proposal/BreakCompletionProposal.h"
#include "Proposal/WhileCompletionProposal.h"
#include "Proposal/DoCompletionProposal.h"
#include "Proposal/DefaultCompletionProposal.h"
#include "Proposal/ThrowCompletionProposal.h"
#include "Proposal/ElseCompletionProposal.h"
#include "Proposal/CaseCompletionProposal.h"
#include "Proposal/ForCompletionProposal.h"
#include "Proposal/SwitchCompletionProposal.h"
#include "Proposal/ContinueCompletionProposal.h"
#include "Proposal/SynchronizedCompletionProposalcpp.h"
#include "Proposal/ClassCompletionProposal.h"
#include "Proposal/InterfaceCompletionProposal.h"
#include "Proposal/MethodCompletionProposal.h"
#include <boost/unordered/unordered_set.hpp>
#include <JCDT_Lib/internal/util/spell.h>
using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

	namespace CodeAssist {

		std::vector<wstring>   m_leftJavaKeyword =
		{
			L"abstract",
			L"private",
			L"boolean",
			L"implements",
			L"protected",
			L"double",
			L"public",
			L"byte",
			L"instanceof",
			L"transient",
			L"extends",
			L"long",
			L"short",
			L"final",
			L"static",
			L"void",
			L"char",
			L"finally",
			L"long",
			L"strictfp",
			L"volatile",
			L"float",
			L"native",
			L"const",
			L"new",
			L"goto",
			L"package",
			L"int",
			L"null",
			L"throws",
			L"true",
			L"false"
		};
		void CompletionEngine::ProcessCompletionOnReturnStatement()
		{	
			auto  methodScope = dynamic_cast<AbstractMethodDeclaration*>(ScopeUtil::enclosingMethod(visitor->completeScope));
			if (methodScope && methodScope->method_symbol && !methodScope->method_symbol->IsConstructor())
			{
				auto type = methodScope->method_symbol->Type();

				if (!type || type == lookupEnvironment->no_type) {

				}
				else {
					vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
					vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
					vector<VariableSymbol*>  localsFound;
					findVariablesAndMethods(
						L"",
						visitor->completeScope,
						&FakeInvocationSite,
						visitor->completeScope,
						VarialeOnly,
						type,
						methodsFound,
						fieldsFound,
						localsFound,
						DO_NOT_CARE_CONST);
				}
			}
		}
		void CompletionEngine::ProcessCompletionOnCaseStatement()
		{
			if (visitor->completeScope->GetScopeKind() == AstNodeScope::BLOCK_SCOPE) {
				auto  scopeNode = dynamic_cast<AstBlock*>(visitor->completeScope);
				SwitchBlock* switchBlock = nullptr;
				while (scopeNode) {
					auto tag = scopeNode->Tag();
					if (tag == AstBlock::SWITCH)
					{
						switchBlock = reinterpret_cast<SwitchBlock*>(scopeNode);
						break;
					}
					scopeNode = dynamic_cast<AstBlock*>(scopeNode->parent_scope);
				}

				if (switchBlock) {
					auto switchStatement = reinterpret_cast<SwitchStatement*>(switchBlock->owner_ast);
					if (!switchStatement->expression) {
						return;
					}
					auto symbol = switchStatement->expression->symbol;
					if (!symbol) {
						return;
					}
					TypeSymbol* type = nullptr;
					if (symbol->_kind == Symbol::VARIABLE)
					{
						type = reinterpret_cast<VariableSymbol*>(symbol)->Type();
						if (!type || type == lookupEnvironment->no_type)
						{
							type = nullptr;
						}
					}
					else if (symbol->_kind == Symbol::TYPE && symbol != lookupEnvironment->no_type)
					{
						type = reinterpret_cast<TypeSymbol*>(symbol);
					}
					if (type)
					{
						if (!transitRequstor)
							transitRequstor = new ResultCollector();

						auto temp_requestor = (ResultCollector*)requestor;
						requestor = transitRequstor;
						vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
						vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
						vector<VariableSymbol*>  localsFound;
						findVariablesAndMethods(
							L"",
							visitor->completeScope,
							&FakeInvocationSite,
							visitor->completeScope,
							VarialeOnly,
							type,
							methodsFound,
							fieldsFound,
							localsFound,
							ONLY_CONST);

						transitRequstor->ClearAndDelete();
						requestor = temp_requestor;

						auto fieldsFoundData = fieldsFound.data();
						auto fieldsFoundDataSize = fieldsFound.size();

						auto localsFoundSize = localsFound.size();
						auto localsFoundData = localsFound.data();

						boost::unordered_set<VariableSymbol*> excludeVariables;

						unsigned  numSwitchBlocks = switchStatement->NumBlocks();
						for (unsigned i = 0; i < numSwitchBlocks; ++i)
						{
							AstSwitchBlockStatement* tempBlock = switchStatement->GetBlock(i);
							auto numLabel = tempBlock->NumSwitchLabels();
							for (unsigned k = 0; k < numLabel; ++k)
							{
								auto lablel = tempBlock->SwitchLabel(k);
								if (lablel->expression_opt && lablel->expression_opt->value)
								{
									LiteralValue* value = lablel->expression_opt->value;
									for (auto varIdx = 0; varIdx < localsFoundSize; ++varIdx)
									{
										auto  variable = localsFoundData[varIdx];
										if (!variable)
											return;
										if (variable->initial_value == value) {
											excludeVariables.insert(variable);
										}
									}
									for (auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
									{
										auto pairItem = fieldsFoundData[varIdx];
										
										auto variable = pairItem.first;
										if (variable->initial_value == value) {
											excludeVariables.insert(variable);
										}
									}
								}

							}
						}

						if (!excludeVariables.empty()) {
							auto end = excludeVariables.end();
							for (auto varIdx = 0; varIdx < localsFoundSize; ++varIdx)
							{
								auto  local = localsFoundData[varIdx];
								if (!local)
									continue;
								if (excludeVariables.find(local) == end)
								{
									double  relevance = R_DEFAULT;
									auto localType = local->Type();
									relevance += computeRelevanceForExpectingType(local->Type());
									relevance += computeRelevanceForFinal(assistNodeIsInsideCase, local->ACC_FINAL());
									wstring typePackageName;
									wstring typeName;
									if (localType) {
										typePackageName = localType->ContainingPackageName();
										boost::replace_all(typePackageName, L"/", L".");
										typeName = localType->qualifiedSourceName('.');
									}

									requestor->acceptLocalVariable(
										local->getNameString(),
										typePackageName,
										typeName,
										local->Flags(),
										completionRang,
										relevance);
								}

							}
							for (auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
							{
								auto pairItem = fieldsFoundData[varIdx];
								auto field = pairItem.first;
								if (excludeVariables.find(field) == end)
								{
									double  relevance = computeBaseRelevance();
									relevance += computeRelevanceForResolution();

									relevance += computeRelevanceForExpectingType(field->Type());
									relevance += computeRelevanceForEnumConstant(field->Type());
									relevance += computeRelevanceForStatic(DO_NOT_CARE_STATIC, field->ACC_STATIC());
									relevance += computeRelevanceForFinal(assistNodeIsInsideCase, field->ACC_FINAL());
								

									auto declaringClass = (TypeSymbol*)field->owner;
									if (!declaringClass)
										return;

									wstring declaringTypePackageName = declaringClass->ContainingPackageName();
									boost::replace_all(declaringTypePackageName, L"/", L".");
									auto fieldType = field->Type();
									requestor
										->acceptField(declaringTypePackageName,
											declaringClass->qualifiedSourceName('.'),
											field->getNameString(),
											fieldType->ContainingPackageName(),
											fieldType->qualifiedSourceName('.'),
											field->getNameString(),
											// may include some qualification to resolve ambiguities
											field->Flags(),
											completionRang,
											relevance);
								}
							}
						}
						else
						{
							for (auto varIdx = 0; varIdx < localsFoundSize; ++varIdx)
							{
								auto  local = localsFoundData[varIdx];		
								double  relevance = R_DEFAULT;
								auto localType = local->Type();
								relevance += computeRelevanceForExpectingType(local->Type());
								relevance += computeRelevanceForFinal(assistNodeIsInsideCase, local->ACC_FINAL());
								wstring typePackageName;
								wstring typeName;
								if (localType) {
									typePackageName = localType->ContainingPackageName();
									boost::replace_all(typePackageName, L"/", L".");
									typeName = localType->qualifiedSourceName('.');
								}

								requestor->acceptLocalVariable(
									local->getNameString(),
									typePackageName,
									typeName,
									local->Flags(),
									completionRang,
									relevance);
							}
							for (auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
							{
								auto pairItem = fieldsFoundData[varIdx];
								auto field = pairItem.first;
								double  relevance = computeBaseRelevance();
								relevance += computeRelevanceForResolution();

								relevance += computeRelevanceForExpectingType(field->Type());
								relevance += computeRelevanceForEnumConstant(field->Type());
								relevance += computeRelevanceForStatic(DO_NOT_CARE_STATIC, field->ACC_STATIC());
								relevance += computeRelevanceForFinal(assistNodeIsInsideCase, field->ACC_FINAL());
							

								auto declaringClass = (TypeSymbol*)field->owner;
								if(!declaringClass)
								{
									return;
								}
								wstring declaringTypePackageName = declaringClass->ContainingPackageName();
								boost::replace_all(declaringTypePackageName, L"/", L".");
								auto fieldType = field->Type();
								if (!fieldType)
								{
									return;
								}
								requestor
									->acceptField(declaringTypePackageName,
										declaringClass->qualifiedSourceName('.'),
										field->getNameString(),
										fieldType->ContainingPackageName(),
										fieldType->qualifiedSourceName('.'),
										field->getNameString(),
										// may include some qualification to resolve ambiguities
										field->Flags(),
										completionRang,
										relevance);
							}
						}
					}
				}
			}
		}
	void CompletionEngine::ProccessKeyWord(const wstring identiferForKeyWord, CompleteAssistASTVisitor* visitor, unsigned int preTokenKind)
		{

			if (!isForCodeComplete || !visitor)
				return;

			if(preTokenKind == TK_DOT){
				return;
			}
			auto  identiferForKeyWordData = identiferForKeyWord.data();
			auto identifierLen = identiferForKeyWord.size();
			ResultCollector* collector = (ResultCollector*)(requestor);

			int resultCount = collector->items.size();
			bool extraComplete = true;
			if (identifierLen == 1){
				if (resultCount > 12) {
					extraComplete = false;
				}
			}
			if (resultCount > 3) {
				if (assistNodeIsInsideCase)
					return;
			}

			auto scopeKInd = visitor->completeScope->GetScopeKind();
			auto rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_return);
			if (rate)	
			{
				auto  methodScope = dynamic_cast<AbstractMethodDeclaration*>(ScopeUtil::enclosingMethod(visitor->completeScope));
				if (methodScope && methodScope->method_symbol && !methodScope->method_symbol->IsConstructor())
				{

					auto temp_requestor = (ResultCollector*)requestor;
					double  relevance = computeBaseRelevance();

					wstring _string = L"return ;";
					 
					relevance += rate;
					if (keyWordWeightIncrease)
						relevance = relevance / R_INCREASE_WEIGHT;
					auto item = new  JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal,
						0, relevance, completionRang->Clone());
					item->fDisplayString = (_string);
					item->needToCheckMatch = false;
					item->cursorLenth = _string.size() - 1;
					auto display = item->GetDisplayString();
					if (!temp_requestor->allDisplayString.insert(display).second) {
						delete item;
					}
					else {
						temp_requestor->items.emplace_back(item);
					}
	
					auto type = methodScope->method_symbol->Type();
					if (!type || type == lookupEnvironment->no_type){
					}
					else{
						if (!transitRequstor)
							transitRequstor = new ResultCollector();

					
						requestor = transitRequstor;


						vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
						vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
						vector<VariableSymbol*>  localsFound;

						findVariablesAndMethods(
							L"",
							visitor->completeScope,
							&FakeInvocationSite,
							visitor->completeScope,
							VarialeOnly,
							type,
							methodsFound,
							fieldsFound,
							localsFound,
							DO_NOT_CARE_CONST);

						temp_requestor->items.reserve(temp_requestor->items.size() + transitRequstor->items.size());
						for (auto it : transitRequstor->items) {
							_string = L"return " + it->GetReplacementString() + L";";
							temp_requestor->acceptKeyword(_string, completionRang, relevance,false);
						}
						transitRequstor->ClearAndDelete();
						requestor = temp_requestor;
					}
				}
			}

			if (CharOperation::prefixEquals(identiferForKeyWord, StringConstant::US_if, false))
			{
				double  relevance = computeBaseRelevance();

				if(scopeKInd != AstNodeScope::BLOCK_SCOPE){
					relevance = 1;
				}
				rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_if);
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new IfCompletionProposal(0, relevance,completionRang->Clone());
				proposal->otheTag = IfCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if(extraComplete)
				{
					proposal = new IfCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = IfCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new IfCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = IfCompletionProposal::WITH_ELSE;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				

			}
			if (CharOperation::prefixEquals(identiferForKeyWord, StringConstant::US_try, false))
			{
				double  relevance = computeBaseRelevance();

				if (scopeKInd != AstNodeScope::BLOCK_SCOPE) {
					relevance = 1;
				}
				rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_try);
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new TryCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = TryCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new TryCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = TryCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
		

			}
			

			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_break);
			if (rate)
			{
				double  relevance = computeBaseRelevance();

				if (scopeKInd != AstNodeScope::BLOCK_SCOPE) {
					relevance = 1;
				}
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new BreakCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = BreakCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new BreakCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = BreakCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
			
			}

	
			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_while);
			if (rate)
			{
			
				double  relevance = computeBaseRelevance();

				if (scopeKInd != AstNodeScope::BLOCK_SCOPE) {
					relevance = 1;
				}
			
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new WhileCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = WhileCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new WhileCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = WhileCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new WhileCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = WhileCompletionProposal::DO_WHILE;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}
			if (CharOperation::prefixEquals(identiferForKeyWord, StringConstant::US_do, false))
			{
				double  relevance = computeBaseRelevance();

				if (scopeKInd != AstNodeScope::BLOCK_SCOPE) {
					relevance = 1;
				}
			
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				else
				{
					rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_do);
					relevance += rate;
				}
				auto proposal = new DoCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = DoCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new DoCompletionProposal(0, 50, completionRang->Clone());
					proposal->otheTag = DoCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
			
			}

		
			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_default);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new DefaultCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = DefaultCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new DefaultCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = DefaultCompletionProposal::WITH_RETURN;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new DefaultCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = DefaultCompletionProposal::WITH_BREAK;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}
			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_this);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto ctor = dynamic_cast<ConstructorDeclaration*> (ScopeUtil::enclosingMethod(visitor->completeScope));
				if (ctor) {
					auto type = ScopeUtil::enclosingSourceType(visitor->completeScope);

					if (type) {
						vector<TypeSymbol*>  argTypes;
						if (!transitRequstor)
							transitRequstor = new ResultCollector();
						auto temp_requestor = (ResultCollector*)requestor;
						requestor = transitRequstor;
						findConstructors(type, argTypes, visitor->completeScope, &FakeInvocationSite, false, ctor->method_symbol);
						auto  tempSize = transitRequstor->items.size();
						if (!tempSize)
						{
							requestor = temp_requestor;
							requestor->acceptKeyword(L"this", completionRang, relevance,false);
						}
						else
						{
							temp_requestor->items.reserve(temp_requestor->items.size() + tempSize);
							for (auto it : transitRequstor->items) {
								if (it->iType == IJavaCompletionProposal::MethodProposal) {
									temp_requestor->items.emplace_back(it);
									it->relevance += R_CASE;
									reinterpret_cast<MethodCompletionProposal*>(it)->selector = L"this";
									it->needToCheckMatch = false;
								}
								else
									delete it;
							}
							transitRequstor->items.clear();
							requestor = temp_requestor;
						}
					}
				}
				else
				{
					requestor->acceptKeyword(L"this", completionRang, relevance,false);
				}
			}


			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_throw);
			if (rate)
			{
				double  relevance = computeBaseRelevance(); 
				relevance += rate; 
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new ThrowCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = ThrowCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new ThrowCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ThrowCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
			
			}

			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_else);
			if (rate)
			{
				double  relevance = computeBaseRelevance();

				if (scopeKInd != AstNodeScope::BLOCK_SCOPE) {
					relevance = 1;
				}
				
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new ElseCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = ElseCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new ElseCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ElseCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new ElseCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ElseCompletionProposal::WITH_IF;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}

			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_case);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				bool inSwithBlock = false;;
				if (visitor->completeScope->GetScopeKind() == AstNodeScope::BLOCK_SCOPE) {
					auto  scopeNode = dynamic_cast<AstBlock*>(visitor->completeScope);
					SwitchBlock* switchBlock = nullptr;
					while (scopeNode) {
						auto tag = scopeNode->Tag();
						if (tag == AstBlock::SWITCH)
						{
							switchBlock = reinterpret_cast<SwitchBlock*>(scopeNode);
							break;
						}
						scopeNode = dynamic_cast<AstBlock*>(scopeNode->parent_scope);
					}

					if (switchBlock) {
						inSwithBlock = true;
						relevance += R_CASE_IN_SWITCH;
						auto proposal = new CaseCompletionProposal(0, relevance + 1, completionRang->Clone());
						proposal->otheTag = CaseCompletionProposal::NORMAL;
						proposal->needToCheckMatch = false;
						collector->items.emplace_back(proposal);

	
						proposal = new CaseCompletionProposal(0, relevance + 1, completionRang->Clone());
						proposal->otheTag = CaseCompletionProposal::WITH_BRACKETS;
						proposal->needToCheckMatch = false;
						collector->items.emplace_back(proposal);

							
						auto switchStatement = reinterpret_cast<SwitchStatement*>(switchBlock->owner_ast);
						if (!switchStatement || !switchStatement->expression)
							return;

						auto symbol = switchStatement->expression->symbol;
						if (!symbol) {
							return;
						}
						TypeSymbol* type = nullptr;
						if (symbol->_kind == Symbol::VARIABLE)
						{
							type = reinterpret_cast<VariableSymbol*>(symbol)->Type();
							if (!type || type == lookupEnvironment->no_type)
							{
								type = nullptr;
							}
						}
						else if (symbol->_kind == Symbol::TYPE && symbol != lookupEnvironment->no_type)
						{
							type = reinterpret_cast<TypeSymbol*>(symbol);
						}
						if (type)
						{
							if (!transitRequstor)
								transitRequstor = new ResultCollector();

							auto temp_requestor = (ResultCollector*)requestor;
							requestor = transitRequstor;
							vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
							vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
							vector<VariableSymbol*>  localsFound;
							findVariablesAndMethods(
								L"",
								visitor->completeScope,
								&FakeInvocationSite,
								visitor->completeScope,
								VarialeOnly,
								type,
								methodsFound,
								fieldsFound,
								localsFound,
								ONLY_CONST);
								
							transitRequstor->ClearAndDelete();
							requestor = temp_requestor;

							auto fieldsFoundData = fieldsFound.data();
							auto fieldsFoundDataSize = fieldsFound.size();

							auto localsFoundSize = localsFound.size();
							auto localsFoundData = localsFound.data();

							boost::unordered_set<VariableSymbol*> excludeVariables;

							unsigned  numSwitchBlocks = switchStatement->NumBlocks();
							for(unsigned i = 0 ; i < numSwitchBlocks; ++i)
							{
								AstSwitchBlockStatement* tempBlock = switchStatement->GetBlock(i);
								auto numLabel = tempBlock->NumSwitchLabels();
								for(unsigned k = 0 ;  k < numLabel; ++k)
								{
									auto lablel = tempBlock->SwitchLabel(k);
									if(lablel->expression_opt && lablel->expression_opt->value)
									{
										LiteralValue* value = lablel->expression_opt->value;
										for(auto varIdx = 0 ; varIdx < localsFoundSize; ++varIdx)
										{
											auto  variable = localsFoundData[varIdx];
											if(variable && variable->initial_value == value){
												excludeVariables.insert(variable);
											}
										}
										for(auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
										{
											auto pairItem = fieldsFoundData[varIdx];
											auto variable = pairItem.first;
											if (variable && variable->initial_value == value) {
												excludeVariables.insert(variable);
											}
										}
									}

								}
							}
								
							if(!excludeVariables.empty()){
								auto end = excludeVariables.end();
								for (auto varIdx = 0; varIdx < localsFoundSize; ++varIdx)
								{
									auto  variable = localsFoundData[varIdx];
									if (variable && excludeVariables.find(variable) == end)
									{
										auto _string = L"case " + variable->getNameString() + L":";
										requestor->acceptKeyword(_string, completionRang, relevance,false);
									}

								}
								for (auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
								{
									auto pairItem = fieldsFoundData[varIdx];
									auto variable = pairItem.first;
									if (variable && excludeVariables.find(variable) == end)
									{
										auto _string = L"case " + variable->getNameString() + L":";
										requestor->acceptKeyword(_string, completionRang, relevance,false);
									}
								}
							}
							else
							{
								for (auto varIdx = 0; varIdx < localsFoundSize; ++varIdx)
								{
									auto  variable = localsFoundData[varIdx];

									if (!variable)
										continue;

									auto _string = L"case " + variable->getNameString() + L":";
									requestor->acceptKeyword(_string, completionRang, relevance,false);
								}
								for (auto varIdx = 0; varIdx < fieldsFoundDataSize; ++varIdx)
								{
									auto pairItem = fieldsFoundData[varIdx];
									auto variable = pairItem.first;

									if (!variable)
										continue;

									auto _string = L"case " + variable->getNameString() + L":";
									requestor->acceptKeyword(_string, completionRang, relevance, false);
								}
							}
						}
					}
				}
			
				if (extraComplete && !inSwithBlock)
				{
				
					relevance += rate;
					if (keyWordWeightIncrease)
						relevance = relevance / R_INCREASE_WEIGHT;
					auto proposal = new CaseCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = CaseCompletionProposal::NORMAL;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new CaseCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = CaseCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}

			}
	
			
			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_super);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto ctor = dynamic_cast<ConstructorDeclaration*> (ScopeUtil::enclosingMethod(visitor->completeScope));
				if (ctor) {
					auto type = ScopeUtil::enclosingSourceType(visitor->completeScope);

					if(type && type->super){
						vector<TypeSymbol*>  argTypes;
						if (!transitRequstor)
							transitRequstor = new ResultCollector();
						auto temp_requestor = (ResultCollector*)requestor;
						requestor = transitRequstor;
						findConstructors(type->super, argTypes, visitor->completeScope, &FakeInvocationSite, false,NO_EXCLUDE_CONSTRUCTOR);
						temp_requestor->items.reserve(temp_requestor->items.size() + transitRequstor->items.size());
						for (auto it : transitRequstor->items) {
							if(!it)
								continue;

							if (it->iType == IJavaCompletionProposal::MethodProposal){
								it->relevance += R_CASE;
								temp_requestor->items.emplace_back(it);
								reinterpret_cast<MethodCompletionProposal*>(it)->selector = L"super";
								it->needToCheckMatch = false;
							}
							else
								delete it;
						}
						transitRequstor->items.clear();
						requestor = temp_requestor;
					}
					else
					{
						requestor->acceptKeyword(L"super", completionRang, relevance, false);
					}
				}
				else
				{
					requestor->acceptKeyword(L"super", completionRang, relevance, false);
				}

			}

				rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_for);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;

				auto proposal = new ForCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = ForCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new ForCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ForCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}

			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_switch);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				
				relevance += rate;

				auto proposal = new SwitchCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = SwitchCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new SwitchCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = SwitchCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}



			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_continue);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new ContinueCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = ContinueCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new ContinueCompletionProposal(0, 50, completionRang->Clone());
					proposal->otheTag = ContinueCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}

				
			}


			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_synchronized);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new SynchronizedCompletionProposalcpp(0, relevance, completionRang->Clone());
				proposal->otheTag = SynchronizedCompletionProposalcpp::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if (extraComplete)
				{
					proposal = new SynchronizedCompletionProposalcpp(0, relevance, completionRang->Clone());
					proposal->otheTag = SynchronizedCompletionProposalcpp::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}
				
			}


				rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_class);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
			
				if (scopeKInd == AstNodeScope::COMPILATION_UNIT_SCOPE) {
					relevance += R_CASE;
				}
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new ClassCompletionProposal(0, relevance, completionRang->Clone());
				proposal->otheTag = ClassCompletionProposal::NORMAL;
				proposal->needToCheckMatch = false;
				collector->items.emplace_back(proposal);
				if(extraComplete)
				{
					proposal = new ClassCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ClassCompletionProposal::WITH_BRACKETS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);

					proposal = new ClassCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = ClassCompletionProposal::WITH_EXTENDS;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
				}

			}

			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_import);
			if (rate)
			{
				double  relevance = computeBaseRelevance();
				if (scopeKInd == AstNodeScope::COMPILATION_UNIT_SCOPE) {
					relevance += R_CASE;
				}
				relevance += rate;
				if (keyWordWeightIncrease)
					relevance = relevance / R_INCREASE_WEIGHT;
				auto proposal = new JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal, 0, relevance, completionRang->Clone());
				collector->items.emplace_back(proposal);
				proposal->fDisplayString = proposal->fReplacementString = L"import ";
				proposal->needToCheckMatch = false;
				if (extraComplete)
				{
					proposal = new JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal, 0, relevance, completionRang->Clone());
					collector->items.emplace_back(proposal);
					proposal->needToCheckMatch = false;
					proposal->fDisplayString = proposal->fReplacementString = L"import javacard.framework.*;";


					proposal = new JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal, 0, relevance, completionRang->Clone());
					collector->items.emplace_back(proposal);
					proposal->fDisplayString = proposal->fReplacementString = L"import javacard.security.*;";
					proposal->needToCheckMatch = false;

					proposal = new JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal, 0, relevance, completionRang->Clone());
					collector->items.emplace_back(proposal);
					proposal->fDisplayString = proposal->fReplacementString = L"import javacardx.crypto.*;";
					proposal->needToCheckMatch = false;

					proposal = new JavaCompletionProposal(IJavaCompletionProposal::KeywordProposal, 0, relevance, completionRang->Clone());
					collector->items.emplace_back(proposal);
					proposal->fDisplayString = proposal->fReplacementString = L"import javacardx.framework.tlv.*;";
					proposal->needToCheckMatch = false;
				}	
				
			}


			rate = computeRelevanceForCaseMatching(identiferForKeyWordData, StringConstant::US_interface);
			if (rate)
			{
					double  relevance = computeBaseRelevance();
					if (scopeKInd == AstNodeScope::COMPILATION_UNIT_SCOPE) {
						relevance += R_CASE;
					}
					relevance += rate;
					if (keyWordWeightIncrease)
						relevance = relevance / R_INCREASE_WEIGHT;
					auto proposal = new InterfaceCompletionProposal(0, relevance, completionRang->Clone());
					proposal->otheTag = InterfaceCompletionProposal::NORMAL;
					proposal->needToCheckMatch = false;
					collector->items.emplace_back(proposal);
					if (extraComplete)
					{
						proposal = new InterfaceCompletionProposal(0, relevance, completionRang->Clone());
						proposal->otheTag = InterfaceCompletionProposal::WITH_BRACKETS;
						proposal->needToCheckMatch = false;
						collector->items.emplace_back(proposal);

						proposal = new InterfaceCompletionProposal(0, relevance, completionRang->Clone());
						proposal->otheTag = InterfaceCompletionProposal::WITH_EXTENDS;
						proposal->needToCheckMatch = false;
						collector->items.emplace_back(proposal);
					}
			}
					
			findKeywords(identiferForKeyWord, m_leftJavaKeyword, nullptr);
		}


	void CompletionEngine::findKeywords(const wstring& keyword, vector<wstring>& choices, AstNodeScope* scope) {
		auto  resultSize = requestor->ResultSize();
		int length = keyword.length();
		if (length > 0) {
			auto choiceData = choices.data();
			for (int i = 0, size = choices.size(); i < size; i++) {
				auto  choiceRef = choiceData[i];
				double rate = Spell::IsLeftStringOrderInRightString(keyword.c_str(), choiceRef.c_str());
				if (length <= choiceRef.length()
					&& (CharOperation::prefixEquals(keyword, choiceRef, false)
						|| rate )
					)
				{
					double  relevance = R_DEFAULT;
					auto  temp = computeRelevanceForCaseMatching(keyword, choiceRef);
					if (!temp){
						relevance += rate;
					}
					else{
						relevance += temp;
					}
					if (keyWordWeightIncrease)
						relevance = relevance / R_INCREASE_WEIGHT;
					requestor->acceptKeyword(choiceRef, completionRang, relevance, false);
				}
			}
		}
	}

} // Close namespace Jikes block

} // Close namespace CodeAssist block

