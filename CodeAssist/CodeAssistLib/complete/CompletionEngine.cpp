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
#include "Proposal/TypeCompletionProposal.h"
#include <JCDT_Lib/internal/util/spell.h>

#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JCDT_Lib/internal/ast/InvocationSite.h>

using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		 vector<wstring> CompletionEngine::baseTypes = {
			 L"bool", //$NON-NLS-1$
			 L"byte", //$NON-NLS-1$
			 L"char", //$NON-NLS-1$
			 L"double", //$NON-NLS-1$
			 L"float", //$NON-NLS-1$
			 L"int", //$NON-NLS-1$
			 L"long", //$NON-NLS-1$
			 L"short", //$NON-NLS-1$
			 L"void", //$NON-NLS-1$
		 };

		 wstring CompletionEngine::classField(L"class");
		 wstring CompletionEngine::lengthField(L"length");
		 wstring CompletionEngine::_THIS(L"this");
		 wstring CompletionEngine::THROWS(L"throws");

		 TypeSymbol* CompletionEngine::NO_TYPE_SYMBOL = nullptr;
		 MethodSymbol* CompletionEngine::NO_EXCLUDE_CONSTRUCTOR = nullptr;
		 wstring CompletionEngine::EMPTY_TOKEN;
		 wstring CompletionEngine::NoChar;

		 
		 CompletionEngine::__InvocationSite  CompletionEngine::FakeInvocationSite;


		 wstring computePrefix(TypeSymbol* declarationType, TypeSymbol* invocationType, bool isStatic) {

			 wstring  completion;
			 if (!declarationType || !invocationType)
				 return completion;
			

			 if (isStatic) {
				 completion.append(declarationType->sourceName('.'));

			 }
			 else if (declarationType == invocationType) {
				 completion.append(CompletionEngine::_THIS);

			 }
			 else {

				 if (!declarationType->IsNested()) {

					 completion.append(declarationType->sourceName('.'));
					 completion.push_back('.');
					 completion.append(CompletionEngine::_THIS);

				 }
				 else if (!declarationType->Anonymous()) {

					 completion.append(declarationType->sourceName('.'));
					 completion.push_back('.');
					 completion.append(CompletionEngine::_THIS);

				 }
			 }

			 return completion;
		 }


		  bool CompletionEngine::mustQualifyType(
			 const wstring& packageName,
			  const wstring& typeName)
		  {

			 // if (!cunit || cunit->compilation_unit)
				//  return true;

			 // auto compilation_unit = cunit->compilation_unit;
			 //// If there are no types defined into the current CU yet.
			 // vector<wstring> compoundPackageName;
			 // boost::split(compoundPackageName, packageName, boost::is_any_of(L"."));

			 // wstring  readableTypeName = packageName + L"." + typeName;
			 // wstring unitCompoundName;

			 // if(compilation_unit->package_declaration_opt){
				// auto temp = compilation_unit->package_declaration_opt->getImportName();
				// unitCompoundName = boost::join(temp, L".");
			 // }
			 //      
			 // if(unitCompoundName == packageName)
				// return false;

			 // auto size = compilation_unit->NumImportDeclarations();
			 // for(auto i = 0 ; i < size; ++ i)
			 // {
				//  auto importDecl = compilation_unit->ImportDeclaration(i);
				//  if (importDecl->onDemand())
				//  {
				//	  auto importReadableName = importDecl->getImportName('.');
				//	  if (importReadableName == packageName)
				//	  {
				//		  for (int j = 0; j < imports.length; j++) {
				//			  if (i != j) {
				//				  if (imports[j].onDemand) {
				//					  if (nameEnvironment.findType(typeName, imports[j].compoundName) != null) {
				//						  return true;
				//					  }
				//				  }
				//				  else {
				//					  if (CharOperation.equals(CharOperation.lastSegment(imports[j].readableName(), '.'), typeName)) {
				//						  return true;
				//					  }
				//				  }
				//			  }
				//		  }
				//		  return false; // how do you match p1.p2.A.* ?
				//	  }

				//  }
				//  else
				//  {
				//	  auto importReadableName = importDecl->getImportName('.');
				//	  if (importReadableName == readableTypeName){
				//		  return false;
				//	  }
				//  }
				//	 
			 // }
	
			 return true;
		 }

		
		void CompletionEngine::findPackages(const wstring&  prefex) {
			GetNameLookUp();
			auto lastDotIdx = (prefex.rfind(L"."));
			if(lastDotIdx == wstring::npos)
			{

			}
			else
			{
				pakcageAssistWhenDot = prefex.substr(0, lastDotIdx + 1);
			}
			
			namel_lookup_->seekPackageFragments(prefex, true, nameLookupRequestor);
		}

		void CompletionEngine::findTypes(const wstring& prefex)
		{
			

			GetNameLookUp();
			if (!isForCodeComplete)
			{
				namel_lookup_->seekTypes(prefex, nullptr, true, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES
					, nameLookupRequestor);
				return;
			}
			set<wstring> packageNames;
			auto lastDotIndex = prefex.rfind('.');
		
			resolveImportNames();
			wstring prefexTypeName;
			if (lastDotIndex == wstring::npos) {
				prefexTypeName = prefex;
				for (auto it : *importPackageNames)
				{
					auto& names = it.first;
					auto size = names.size();
					if(it.second)//on demand
					{
						packageNames.insert(boost::join(names, L"."));
					}
					else {
						if (size > 1) 
						{
							if (CharOperation::prefixEquals(prefexTypeName, names[size - 1], false)) {
								auto _begin = names.begin();
								auto _end = _begin + size - 1;
								vector<wstring> package(_begin, _end);
								packageNames.insert(boost::join(package, L"."));
							}
						}
						else if (size == 1) {
							if (CharOperation::prefixEquals(prefexTypeName, names[0], false))
								packageNames.insert(EMPTY_TOKEN);
						}
					}	
				}
			}
			else{

				prefexTypeName = prefex.substr(lastDotIndex + 1, prefex.size());

				auto prefexPacakge = prefex.substr(0, lastDotIndex);


				vector<wstring> prefexPacakgeNames;
				boost::split(prefexPacakgeNames, prefexPacakge, boost::is_any_of(L"."));

				packageNames.insert(prefexPacakge);

				for (auto it : *importPackageNames)
				{	
					auto& names = it.first;
					auto size = names.size();
					if (it.second)//on demand 
					{
						if (CharOperation::equals(prefexPacakgeNames, names, false))
						{
							packageNames.insert(boost::join(names, L"."));
						}
					}
					else
					{
						// 这些是限定引入
						if (CharOperation::prefixEquals(prefexTypeName, names[size - 1], false)) {
							auto _begin = names.begin();
							auto _end = _begin + size - 1;
							vector<wstring> package(_begin, _end);
							if (CharOperation::equals(prefexPacakgeNames, package, false)) {
								packageNames.insert(boost::join(names, L"."));
							}
						}

					}	
				}	
			}



			if(packageNames.size()){
				JavaElementRequestor* elementRequestor = new JavaElementRequestor();
				for (auto packageName : packageNames) {
					namel_lookup_->seekPackageFragments(packageName, false, elementRequestor);
					auto fragments = elementRequestor->getPackageFragments();
					auto fragmentSize = fragments.size();
					if (fragmentSize) {
						for (int i = 0, length = fragmentSize; i < length; i++)
							if (fragments[i] != nullptr){			
								namel_lookup_->seekTypes(prefexTypeName, fragments[i], true,
									NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES, nameLookupRequestor);		
							}
								
					}
					fragments.clear();
				}
				delete elementRequestor;
			}
			else
			{
				namel_lookup_->seekTypes(prefex, nullptr, true, NameLookup::ACCEPT_CLASSES | NameLookup::ACCEPT_INTERFACES
				, nameLookupRequestor);
			}

			
		}

		void CompletionEngine::findPackages(AstPackageDeclaration* packageStatement, AstNodeScope* scope) {

			 AstName* name = packageStatement->name;
			 while (name){

				 if (!name->identifier_token)
					 return;

				 if (name->identifier_token->IsInRange(actualCompletionPosition)){
					 break;
				 }
				 name = name->base_opt; 
			 }
			 if (!name || !name->identifier_token){
				 return;
			 }
			auto identifier_token = name->identifier_token;
	
			auto	token = identifier_token->getName();
			if (identifier_token->start_location < actualCompletionPosition){
				token = token.substr(0, actualCompletionPosition - identifier_token->start_location);
			}
			if (tokenKind == TK_DOT) {
				token += L".";
			}
			else
			{
				requestor->SetTokenForMatch(token);
				token = L".";
			}


			if (name->base_opt){
				vector<wstring> all_names;	
				GetAstNameAllNames(name->base_opt, all_names);
				all_names.push_back(token);
				token = boost::join(all_names, L".");
			}
		
			
			auto lastDotIndex = token.rfind('.');
			if (lastDotIndex != wstring::npos)
				pakcageAssistWhenDot = (token.substr(0, lastDotIndex));

			keyWordWeightIncrease = true;
			boost::to_lower(token);
			findPackages(token);
			 
	
		}
		  void CompletionEngine::findImports(AstImportDeclaration* importReference, AstNodeScope* scope)
		  {
			
			  
			  AstName* name = importReference->name;
			  while (name) {
				  if (!name->identifier_token)
					  return;
				  if (name->identifier_token->IsInRange(actualCompletionPosition)) {
					  break;
				  }
				  name = name->base_opt;
			  }
			  if (!name || !name->identifier_token)
				  return;
			  
			auto identifier_token = name->identifier_token;
			auto	token = identifier_token->getName();
			if (identifier_token->start_location < actualCompletionPosition)
			{
				token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
			}
			
			wstring  preToken ;
			if(name->base_opt){	
				vector<wstring> all_names;
				GetAstNameAllNames(name->base_opt, all_names);	
				preToken = boost::join(all_names, L".");
			}
			
			if(hasImportWord)
			{
				if (tokenKind == TK_DOT) {
					token += L".";
					if (!preToken.empty()) {
						token = preToken + L"." + token;
					}
				}
				else{
					requestor->SetTokenForMatch(token);
					if (!preToken.empty()) {
						token = preToken + L".";
					}
					else
					{
						token.clear();
					}
				}

				keyWordWeightIncrease = true;
				if(isForCodeComplete)
				{
					if (!transitRequstor) {
						transitRequstor = new ResultCollector();
					}
					auto temp = (ResultCollector*)requestor;
					requestor = transitRequstor;
					findPackages(token);

					temp->items.reserve(transitRequstor->items.size() * 2);
					for (auto it : transitRequstor->items) {
						auto pkgProposal = reinterpret_cast<PackageCompletionProposal*>(it);
						if (!pkgProposal || !pkgProposal->completionRang)
							continue;
						
						auto cloneOne = new PackageCompletionProposal(
							pkgProposal->fReplacementString,
							pkgProposal->access, pkgProposal->relevance, pkgProposal->completionRang->Clone(), pkgProposal->packageName
						);
						temp->items.push_back(cloneOne);

						cloneOne->fReplacementString.append(L".*;");

						temp->items.emplace_back(it);
					}
					transitRequstor->JustClearNoDelete();

					requestor = temp;
				}
				else
					findPackages(token);


				if (importReference->name->IsQualifiedName() || tokenKind == TK_DOT)
					findTypes(token);
			}
			else
			{
				if (tokenKind == TK_DOT) {
					token += L".";
					if (!preToken.empty()) {
						token = preToken + L"." + token;
					}
				}
				else {
					requestor->SetTokenForMatch(token);
					if (!preToken.empty()) {
						token = preToken + L".";
					}
					else
					{
						if(!Spell::IsLeftStringOrderInRightString(token.c_str(),StringConstant::US_import))
						{
							token.clear();
						}
					}
				}
				findPackages(token);
				if (importReference->name->IsQualifiedName() || tokenKind == TK_DOT)
					findTypes(token);

			}  
		 }


		  void CompletionEngine::findNestedTypes(
			  const wstring& typeName,
			  TypeSymbol* currentType,
			  AstNodeScope* scope) 
		  {
			  if (typeName.empty())
				  return;

			  int typeLength = typeName.length();

			  while (scope != nullptr) { // done when a COMPILATION_UNIT_SCOPE is found

				  switch (scope->GetScopeKind()) {

				  case AstNodeScope::METHOD_SCOPE:
				  case AstNodeScope::BLOCK_SCOPE:
						break;
				  case AstNodeScope::CLASS_SCOPE: {
					  auto enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
					  findMemberTypes(typeName, enclosingSourceType, scope, currentType);
					  if (typeLength == 0)
						  return; // do not search outside the class scope if no prefix was provided
				  }
					
					  break;

				  case AstNodeScope::COMPILATION_UNIT_SCOPE:
					  return;
				  }
				  scope = scope->parent_scope;
			  }
		  }


		  void CompletionEngine::findTypesAndPackages(const wstring& token, AstNodeScope* scope)
		{
		
			auto enclosingSourceType = ScopeUtil::enclosingSourceType(scope);

			if (enclosingSourceType != nullptr)
				findNestedTypes(token, enclosingSourceType, scope);

			if (cunit != nullptr) {
				int typeLength = token.length();
				vector<TypeSymbol*>& types = cunit->types;// topLevelTypes
				
				for (int i = 0, length = types.size(); i < length; i++) {
					auto sourceType = types[i];

					if (!sourceType)
						continue;

					auto typeSourceName = sourceType->sourceName('.');

					if (typeLength > typeSourceName.length())	
						continue;

					if (!CharOperation::prefixEquals(token, typeSourceName, false))
						continue;

					if (assistNodeIsExtendedType && sourceType->ACC_FINAL())
						continue;

					double  relevance = R_DEFAULT;
					
					relevance += computeRelevanceForExpectingType(sourceType);

					wstring compoundPacakgeName = sourceType->ContainingPackageName();
					boost::replace_all(compoundPacakgeName, L"/", L".");

					if (!sourceType->ACC_INTERFACE()) {
						///relevance += computeRelevanceForClass();
						requestor->acceptClass(
							compoundPacakgeName,
							typeSourceName,
							typeSourceName,
							sourceType->Flags(),
							completionRang,
							relevance);
					}
					else {
						//relevance += computeRelevanceForInterface();
						requestor->acceptInterface(
							compoundPacakgeName,
							typeSourceName,
							typeSourceName,
							sourceType->Flags(),
							completionRang,
							relevance);
					}
				}
			}

			findTypes(token);
			if (token.length() == 0)
				return;
			findPackages(token);
		}


		void CompletionEngine::ProcessCompletionOnFieldType(AstType* type, AstNodeScope* scope)
		{
			if (!type)
				return;

			if(type->kind == Ast::TYPE){
				ProcessCompletionOnTypeReference(reinterpret_cast<AstTypeName*>(type), scope);
			}
			else if(Ast::ARRAY  == type->kind)
			{
				auto leftType = reinterpret_cast<AstArrayType*>(type)->type;
				if(leftType && leftType->kind == Ast::TYPE)
					ProcessCompletionOnTypeReference(reinterpret_cast<AstTypeName*>(leftType), scope);
			}
		}
		

		void CompletionEngine::acceptClass(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			double  relevance = computeBaseRelevance();
		
			relevance += computeRelevanceForClass();
		
			requestor->acceptClass(
				packageName,
				typeName,
				typeName,
				modifiers,
				completionRang,
				relevance);
		}

		void CompletionEngine::acceptInterface(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			double  relevance = computeBaseRelevance();
		
			relevance += computeRelevanceForInterface();
		
			requestor->acceptInterface(
				packageName,
				typeName,
				typeName,
				modifiers,
				completionRang,
				relevance);
		}

		void CompletionEngine::acceptPackage(const std::wstring& packageName)
		{
			wstring completeName;

	        if(!pakcageAssistWhenDot.empty())
	        {
				if(CharOperation::prefixEquals(pakcageAssistWhenDot,packageName,false))
				{
					completeName = packageName.substr(pakcageAssistWhenDot.size(), packageName.size());
				}
				else
					completeName = packageName;
	        }
			else
			{
				completeName = packageName;
			}
			double  relevance = computeBaseRelevance();
			relevance += computeBaseRelevancePackageRelevance();
			
			requestor->acceptPackage(
				packageName,
				completeName,
				completionRang,
				relevance);
		}

		void CompletionEngine::acceptType(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			AccessFlags acces(modifiers);
			if ( assistNodeIsExtendedType &&  acces.ACC_FINAL()) 
				return;

			double  relevance = computeBaseRelevance();
			requestor->acceptType(
				packageName,
				typeName,
				typeName,
				completionRang,
				relevance);
		}

		void CompletionEngine::resolveImportNames()
		{
			if(importPackageNames)
			{
				return;
			}
	
			importPackageNames = new vector<pair< vector<wstring >, bool> >();
			 auto num = cunit->compilation_unit->NumImportDeclarations();

			 importPackageNames->emplace_back( make_pair( vector<wstring >{ L"java", L"lang", L"Object"} , false ) );
			 for (auto i = 0; i < num; ++i)
			 {
				 auto importDecl = cunit->compilation_unit->ImportDeclaration(i);
				 if (!importDecl)
					 continue;

				 if(importDecl->onDemand())
					 importPackageNames->emplace_back(make_pair( importDecl->getNames() , true ) );
				 else
					 importPackageNames->emplace_back(make_pair(importDecl->getNames(), false));
			 }
			 if (layerPos)
			 {
				 vector<wstring > temp;
				 boost::split(temp,layerPos->projectPos.pkg, boost::is_any_of(L"."));
				 importPackageNames->emplace_back(make_pair (temp , true));
			 }
		}

	

		void CompletionEngine::GetCallTips(CompleteAssistASTVisitor* visitor)
		 {
			if (!visitor)
				return;

			 if (!visitor->lastInRangeMethodCallNode)
				 return;
			 
			 if (!transitRequstor)
				 transitRequstor = new ResultCollector();

			 auto temp_requstor = (ResultCollector*)requestor;
			 requestor = transitRequstor;

			 switch (visitor->lastInRangeMethodCallNode->kind)
			 {
				 
			 case Ast::CALL:
			 {
				 TypeSymbol* _typeSymbol = nullptr;
				 auto methodInvocate = reinterpret_cast<AstMethodInvocation*>(visitor->lastInRangeMethodCallNode);
				 if (methodInvocate->symbol )
				 {
					 if (methodInvocate->symbol->_kind == Symbol::METHOD)
					 {
						 if(methodInvocate->base_opt && methodInvocate->base_opt->symbol && methodInvocate->base_opt->symbol->Kind() == Symbol::TYPE)
						 {
							 _typeSymbol =  reinterpret_cast<TypeSymbol*>(methodInvocate->base_opt->symbol );
						 }
						 else
						 {
							 auto method = reinterpret_cast<MethodSymbol*>(methodInvocate->symbol);

							 int length = method->NumFormalParameters();
							 vector<wstring>  parameterPackageNames(length);
							 vector<wstring>  parameterTypeNames(length);


							 for (int i = 0; i < length; i++) 
							 {
								 auto  type = method->FormalParameter(i)->Type();
								 if (!type)
									 continue;

								 wstring temp = type->ContainingPackageName();
								 parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
								 parameterTypeNames[i] = type->qualifiedSourceName('.');
							 }

							 vector<wstring> parameterNames;

							 findMethodParameterNames(method, parameterTypeNames, parameterNames);
							 auto declaringClass = method->containing_type;
							 if (!declaringClass)
								 return;

							 wstring declaringTypePackageName = declaringClass->ContainingPackageName();
							 boost::replace_all(declaringTypePackageName, L"/", L".");

							 auto returnType = method->Type();
							 if (!returnType)
								 return;
							 wstring returnTypePackageName = returnType->ContainingPackageName();
							 boost::replace_all(returnTypePackageName, L"/", L".");
							 requestor->acceptMethod(
							 declaringTypePackageName,
							 declaringClass->qualifiedSourceName('.'),
							 method->getNameString(),
							 parameterPackageNames,
							 parameterTypeNames,
							 parameterNames,
							 returnTypePackageName,
							 returnType->qualifiedSourceName('.'),
								 EMPTY_TOKEN,
							 method->Flags(),
							 completionRang,
							 100);
						 }
						
					 }
					 else if (methodInvocate->symbol->_kind == Symbol::TYPE && methodInvocate->symbol != lookupEnvironment->no_type)
					 {
						  _typeSymbol = reinterpret_cast<TypeSymbol*>(methodInvocate->symbol);

					 }
					 else if(methodInvocate->base_opt
						  && methodInvocate->base_opt->symbol 
						 )
					 {
						 if ( methodInvocate->base_opt->symbol->_kind == Symbol::TYPE
							 && methodInvocate->base_opt->symbol != lookupEnvironment->no_type)
						 {
							 _typeSymbol = reinterpret_cast<TypeSymbol*>(methodInvocate->base_opt->symbol);
						 }
						 else if (methodInvocate->base_opt->symbol->_kind == Symbol::VARIABLE)
						 {
							 auto variable = reinterpret_cast<VariableSymbol*>(methodInvocate->base_opt->symbol);
							 _typeSymbol = variable->Type();
						 }
					 }

					 if(_typeSymbol)
					 {	
						 if (!methodInvocate->identifier_token)
							 return;
						 if (CharOperation::prefixEquals(methodInvocate->identifier_token->getName(), _typeSymbol->getNameString(), false))
						 {
							 vector<TypeSymbol*>  argTypes;
							 findConstructors(_typeSymbol,
								 argTypes, 
								 visitor->completeScope,
								 &FakeInvocationSite,
								 false,
								 NO_EXCLUDE_CONSTRUCTOR);
						 }
						 else
						 {
							 vector<pair<MethodSymbol*, TypeSymbol*>> methodsFound;

							 StaticAccess onlyStaticMethods = DO_NOT_CARE_STATIC;
							 bool exactMatch = false;
							 bool isCompletingDeclaration = false;

							 bool implicitCall = false;
							 vector<TypeSymbol*>  argTypes;
							 findMethods(methodInvocate->identifier_token->getName(),
								 argTypes, _typeSymbol, visitor->completeScope, methodsFound, onlyStaticMethods, exactMatch, isCompletingDeclaration,
								 &FakeInvocationSite, visitor->completeScope, implicitCall,nullptr);
						 }
					 }
					 else if(!methodInvocate->base_opt)
					 {
						 ProcessCompletionOnMessageSend(methodInvocate,
							 visitor->completeScope);
					 }
				 }
				 else
				 {
					 ProcessCompletionOnMessageSend(methodInvocate,
						 visitor->completeScope);
				 }
			 }
			 break;
			 case Ast::CLASS_CREATION:
			 {
				 auto classCreateAst = reinterpret_cast<AstClassCreationExpression*>(visitor->lastInRangeMethodCallNode);
				 if (classCreateAst->class_type && classCreateAst->class_type->symbol && classCreateAst->class_type->symbol->_kind == Symbol::TYPE)
				 {
					 auto _typeSymbol = reinterpret_cast<TypeSymbol*>(classCreateAst->class_type->symbol);
					 if (_typeSymbol != lookupEnvironment->no_type)
					 {
						 vector<TypeSymbol*>  argTypes;
						 findConstructors(_typeSymbol, 
							 argTypes,
							 visitor->completeScope,
							 &FakeInvocationSite,
							 false,
							 NO_EXCLUDE_CONSTRUCTOR);
					 }
				 }

			 }
			 break;
			 case Ast::SUPER_CALL:
			 {
				 auto supperCall = reinterpret_cast<AstSuperCall*>(visitor->lastInRangeMethodCallNode);
				 if (supperCall->symbol)
				 {
					 auto method = reinterpret_cast<MethodSymbol*>(supperCall->symbol);
					 if (!method)
						 return;

					 int length = method->NumFormalParameters();
					 vector<wstring>  parameterPackageNames(length);
					 vector<wstring>  parameterTypeNames(length);


					 for (int i = 0; i < length; i++) {
						 auto temp_p = method->FormalParameter(i);

						 if (!temp_p)
							 continue;
						 auto  type = temp_p->Type();

						 if (!type)
							 continue;

						 wstring temp = type->ContainingPackageName();
						 parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
						 parameterTypeNames[i] = type->qualifiedSourceName('.');
					 }

					 vector<wstring> parameterNames;

					 findMethodParameterNames(method, parameterTypeNames, parameterNames);
					 auto declaringClass = method->containing_type;
					 if (!declaringClass)
						 return;

					 wstring declaringTypePackageName = declaringClass->ContainingPackageName();
					 boost::replace_all(declaringTypePackageName, L"/", L".");

					 auto returnType = method->Type();

					 if (!returnType)
						 return;

					 wstring returnTypePackageName = returnType->ContainingPackageName();
					 boost::replace_all(returnTypePackageName, L"/", L".");
					 requestor->acceptMethod(
						 declaringTypePackageName,
						 declaringClass->qualifiedSourceName('.'),
						 method->getNameString(),
						 parameterPackageNames,
						 parameterTypeNames,
						 parameterNames,
						 returnTypePackageName,
						 returnType->qualifiedSourceName('.'),
						 EMPTY_TOKEN,
						 method->Flags(),
						 completionRang,
						 100);
				 }
				 else if (supperCall->base_opt
					 && supperCall->base_opt->symbol)
				 {
					 if (supperCall->base_opt->symbol->_kind == Symbol::TYPE
						 && supperCall->base_opt->symbol != lookupEnvironment->no_type)
					 {
						 auto _typeSymbol = reinterpret_cast<TypeSymbol*>(supperCall->base_opt->symbol);
						 if (!_typeSymbol->super || _typeSymbol->super != lookupEnvironment->no_type) {
							 return;
						 }

						 vector<TypeSymbol*>  argTypes;
						 findConstructors(_typeSymbol->super,
							 argTypes, 
							 visitor->completeScope,
							 &FakeInvocationSite,
							 false,
							 NO_EXCLUDE_CONSTRUCTOR);
					 }
					 else if(supperCall->base_opt->symbol->_kind == Symbol::VARIABLE)
					 {
						 auto _typeSymbol = reinterpret_cast<VariableSymbol*>(supperCall->base_opt->symbol)->Type();
						 if (!_typeSymbol  || !_typeSymbol->super || _typeSymbol->super != lookupEnvironment->no_type) {
							 return;
						 }
						 vector<TypeSymbol*>  argTypes;
						 findConstructors(_typeSymbol->super, 
							 argTypes,
							 visitor->completeScope,
							 &FakeInvocationSite,
							 false,
							 NO_EXCLUDE_CONSTRUCTOR
						 );
					 }
					
				 }
				 else if(!supperCall->base_opt)
				 {
					 auto _typeSymbol = ScopeUtil::enclosingSourceType(visitor->completeScope);
					 if(_typeSymbol && _typeSymbol->super && _typeSymbol->super != lookupEnvironment->no_type)
					 {
						 vector<TypeSymbol*>  argTypes;
						 findConstructors(_typeSymbol->super,
							 argTypes,
							 visitor->completeScope,
							 &FakeInvocationSite, 
							 false,
							 NO_EXCLUDE_CONSTRUCTOR
						 );
					 }
				 }
			 }
			 break;

			 case Ast::THIS_CALL:
			 {
				 auto thisCall = reinterpret_cast<AstThisCall*>(visitor->lastInRangeMethodCallNode);
				 if (thisCall->symbol)
				 {
					 auto method = reinterpret_cast<MethodSymbol*>(thisCall->symbol);

					 int length = method->NumFormalParameters();
					 vector<wstring>  parameterPackageNames(length);
					 vector<wstring>  parameterTypeNames(length);


					 for (int i = 0; i < length; i++) {
						 auto temp_p = method->FormalParameter(i);
						 if (!temp_p)
							 continue;
						 auto  type = temp_p->Type();
						 if (!type)
							 continue;

						 wstring temp = type->ContainingPackageName();
						 parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
						 parameterTypeNames[i] = type->qualifiedSourceName('.');
					 }

					 vector<wstring> parameterNames;

					 findMethodParameterNames(method, parameterTypeNames, parameterNames);
					 auto declaringClass = method->containing_type;
					 if (!declaringClass)
						 return;

					 wstring declaringTypePackageName = declaringClass->ContainingPackageName();
					 boost::replace_all(declaringTypePackageName, L"/", L".");

					 auto returnType = method->Type();
					 if (!returnType)
						 return;

					 wstring returnTypePackageName = returnType->ContainingPackageName();
					 boost::replace_all(returnTypePackageName, L"/", L".");
					 requestor->acceptMethod(
						 declaringTypePackageName,
						 declaringClass->qualifiedSourceName('.'),
						 method->getNameString(),
						 parameterPackageNames,
						 parameterTypeNames,
						 parameterNames,
						 returnTypePackageName,
						 returnType->qualifiedSourceName('.'),
						 EMPTY_TOKEN,
						 method->Flags(),
						 completionRang,
						 100);
				 }
				 else {
					 auto _typeSymbol = ScopeUtil::enclosingSourceType(visitor->completeScope);
					 if (_typeSymbol && _typeSymbol != lookupEnvironment->no_type){
						 vector<TypeSymbol*>  argTypes;
						 findConstructors(_typeSymbol,
							 argTypes, 
							 visitor->completeScope,
							 &FakeInvocationSite, 
							 false,
							 NO_EXCLUDE_CONSTRUCTOR
						 );
					 }
				 }
			 }
				 break;
			 default:
				 break;
			 }

			 if(!transitRequstor->items.empty())
			 {

				 for(auto itTip : transitRequstor->items)
				 {
					 temp_requstor->tips.emplace_back(itTip);
				 }
				 transitRequstor->JustClearNoDelete();

			 }	
			 requestor = temp_requstor;
		}
		 void CompletionEngine::GetResult()
		 {
			 if (visitor->astNode)
			 {

				 if(isForCodeComplete)
					GetCallTips(visitor);

				 switch (visitor->completNodeType)
				 {
				 case CompleteAssistASTVisitor::CompletionOnMemberAccess:

					 ProcessCompletionOnMemberAccess(reinterpret_cast<FieldReference*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnMessageSend:
					 ProcessCompletionOnMessageSend(reinterpret_cast<AstMethodInvocation*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnExplicitConstructorCall:
					 ProcessCompletionOnExplicitConstructorCall(visitor->astNode, visitor->completeScope);
					 break;



				 case CompleteAssistASTVisitor::CompletionOnAstTypeName:
					 ProcessCompletionOnAstTypeName(reinterpret_cast<AstTypeName*>(visitor->astNode), visitor->completeScope);

					 break;
				 case CompleteAssistASTVisitor::CompletionOnImportReference:
					 findImports(reinterpret_cast<AstImportDeclaration*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnPackageReference:
					 findPackages(reinterpret_cast<AstPackageDeclaration*>(visitor->astNode), visitor->completeScope);
					 break;

				 case CompleteAssistASTVisitor::CompletionOnClassCreationExpressionOnTypeName:
					 ProcessCompletionOnClassCreationExpressionOnTypeName(reinterpret_cast<AstClassCreationExpression*>(visitor->astNode)
						 , visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnClassCreationExpressionDotSymbolInArguent:
					 break;
				 case CompleteAssistASTVisitor::CompletionOnMethodName:
					 ProcessCompletionOnMethodName(reinterpret_cast<MethodDeclaration*>(visitor->astNode)
						 , visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnMethoDotSymbolInArguent:
					 break;
				 case CompleteAssistASTVisitor::CompletionOnMethodReturnType:
					 ProcessCompletionOnMethodReturnType(reinterpret_cast<MethodDeclaration*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnClassReference:
				 case CompleteAssistASTVisitor::CompletionOnInterfaceReference:
				 case CompleteAssistASTVisitor::CompletionOnExceptionReference:
					 ProcessCompletionOnTypeReference(reinterpret_cast<AstTypeName*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnFieldName:
					 ProcessCompletionOnFieldName(reinterpret_cast<AstVariableDeclarator*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnLocalFieldName:
					 ProcessCompletionOnLocalFieldName(reinterpret_cast<AstVariableDeclarator*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnFieldType:
					 ProcessCompletionOnFieldType(reinterpret_cast<FieldDeclaration*>(visitor->astNode)->type, visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnLocalFieldType:
					 ProcessCompletionOnFieldType(reinterpret_cast<LocalDeclaration*>(visitor->astNode)->type, visitor->completeScope);
					 break;

				 case CompleteAssistASTVisitor::CompletionOnNameReference:
					 ProcessCompletionOnNameReference(reinterpret_cast<AstName*>(visitor->astNode), visitor->completeScope);
					 break;
				 case CompleteAssistASTVisitor::CompletionOnModifiers:
					 break;

				 case CompleteAssistASTVisitor::CompletionOnExplicittThisExpr:
				 case CompleteAssistASTVisitor::CompletionOnExplicittSuperExpr:
				 case CompleteAssistASTVisitor::CompletionOnArrayReference:
				 case  CompleteAssistASTVisitor::CompletionCastExpression:
				 case CompleteAssistASTVisitor::CompletionParenthesizedExpression:
					 ProcessCompletionOnExpression(reinterpret_cast<Expression*>(visitor->astNode), visitor->completeScope);
					 break;

				 case CompleteAssistASTVisitor::CompletionOnAssignmentExpressionEqualOpt:
				 case CompleteAssistASTVisitor::CompletionOnAssignmentExpressionBLANK:
					 ProcessCompletionOnAssignmentExpression(reinterpret_cast<AstAssignmentExpression*>(visitor->astNode), visitor->completeScope);
					 break;

				 case CompleteAssistASTVisitor::CompletionOnReturnStateMent:
					 keyWordWeightIncrease = true;
					 ProcessCompletionOnReturnStatement();
						 break;

				 case CompleteAssistASTVisitor::CompletionOnCaseStatement:
					 keyWordWeightIncrease = true;
					 ProcessCompletionOnCaseStatement();
						 break;
				 default:
					 break;

				 }
			 }

		 }
		 void CompletionEngine::ProcessCompletionOnAssignmentExpression(AstAssignmentExpression*expression, AstNodeScope* scope)
		 {
			 Expression* left_hand_side = expression->left_hand_side;

			 if (!left_hand_side)
				 return;

			 auto symbol = left_hand_side->symbol;
			 if (!symbol)
				 return;

			 if (symbol->_kind != Symbol::VARIABLE)
				 return;

			 auto variable = reinterpret_cast<VariableSymbol*>(symbol);
			 auto type = variable->Type();
			 if (!type || type == lookupEnvironment->no_type || type == lookupEnvironment->void_type)
				 return;

			 vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
			 vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
			 vector<VariableSymbol*>  localsFound;
			 if (variable->IsLocal())
				 localsFound.emplace_back(variable);
			 else
				 fieldsFound.emplace_back(make_pair(variable, type));

			 keyWordWeightIncrease = true;
			 findVariablesAndMethods(EMPTY_TOKEN,
				 scope, &FakeInvocationSite,
				 scope,
				 VarialeAndMethodBoth, 
				 type, 
				 methodsFound, 
				 fieldsFound,
				 localsFound,
				 DO_NOT_CARE_CONST);

		 }
		void CompletionEngine::complete()
		{
			wstring& file = _info.doc_path;
			int _line = _info.line;
			int column = _info.column;
			set<wstring> file_to_compiled;
			for(auto it : error_files){
				file_to_compiled.insert(it);
			}
			file_to_compiled.insert(file);
			auto name_symbol = lookupEnvironment->nameEnv->FindOrInsertName(file.c_str(), file.size());
			string utf_8_name = name_symbol->Utf8Name();
			cunit = new UnitFileSymbol(name_symbol, utf_8_name);
			
			CompleteLex* lex = new CompleteLex(option, cunit);
			lex->ReadInput();
			

			scanner->Scan(lex);
			if (!lex->InputBuffer()) {
				delete cunit;
				cunit = nullptr;
				return;
			}

			actualCompletionPosition = cunit->lex_stream->FindPosition(_line, column);
			

			int endIndex = lex->FindGoodTokenIndexInOrigianlTokenStream(actualCompletionPosition);
			unsigned int preTokenKind = 0;
			wstring identiferForKeyWord;
			if (endIndex > 0) {
				auto token = lex->token_stream[endIndex];
				tokenKind = token->_kind;

				int nStartIndex = lex->ColumnFromPos(token->start_location);
				int nStartLine= lex->LineFromPos(token->start_location);
				completionRang = new ISourceRange(nStartIndex, nStartLine, column, _line);
				switch (tokenKind)
				{
				case TK_Identifier:{
					identiferForKeyWord = token->getName();
					lenEntered = actualCompletionPosition - token->start_location + 1;
					if (lenEntered) {
						identiferForKeyWord = identiferForKeyWord.substr(0, lenEntered);
					}
					preTokenKind = lex->token_stream[endIndex - 1]->_kind;
				}
				break;
				case TK_DOT:
					actualCompletionPosition -= 1;
					nStartIndex = lex->ColumnFromPos(token->start_location + 1);
					completionRang->m_nStartIndex = nStartIndex;
					break;
				case TK_EQUAL:
					nStartIndex = lex->ColumnFromPos(token->start_location +1);
					 completionRang->m_nStartIndex = nStartIndex;
					break;
				default:
					identiferForKeyWord = token->getName();
					break;
				}
			}
			else
			{
				completionRang = new ISourceRange(column, _line, column, _line);
			}

			auto _info = HeadParse(cunit);
			if (_info)
			{
				auto compilation_unit = cunit->compilation_unit;

				if (cunit->compilation_unit->type_declarations) {
					AstArray<TypeDeclaration*>& types = *(cunit->compilation_unit->type_declarations);
					for (int i = 0, len = types.Length(); i < len; i++) {
						auto type = types[i];

						ExcludeUselessMethod(type, actualCompletionPosition);
					}
				}
				assert(compilation_unit);
				if (compilation_unit->state == CompilationUnitDeclaration::HEAD_PARSE) {
					for (unsigned k = 0; k < compilation_unit->NumTypeDeclarations(); k++) {
						TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
						if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration)) {
							ParserUtil::CheckNestedMembers(cunit->lex_stream, declaration->class_body, parser);
						}
					}
					compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;
				}
			}
			if (!cunit->compilation_unit)
				return;
		



			set<FileSymbol*> _set= { cunit };
			lookupEnvironment->RecordcompilationUnit(_set);
			

			visitor = new CompleteAssistASTVisitor(actualCompletionPosition);
			visitor->tokenKind = tokenKind;
			cunit->compilation_unit->traverse(visitor, nullptr);
			auto  homeMakeAst = GetHomeMakeAst(visitor, actualCompletionPosition, lookupEnvironment->nameEnv, cunit->compilation_unit, lex, parser);
			lookupEnvironment->ProcessFileHeaders(cunit);
			switch (visitor->completNodeType)
			{
			case CompleteAssistASTVisitor::CompletionOnImportReference:
			case CompleteAssistASTVisitor::CompletionOnPackageReference:
			case CompleteAssistASTVisitor::CompletionOnModifiers:
				break;
			default:
				lookupEnvironment->ProcessFileBodys();
				
				break;
			}

			assistNodeIsClass = visitor->nextTypeReferenceIsClass;
			assistNodeIsInterface = visitor->nextTypeReferenceIsInterface;
			assistNodeIsException = visitor->nextTypeReferenceIsException;
			assistNodeIsInsideCase = visitor->assistNodeIsInsideCase;
			bool isMatchNameSuggest = true;
			switch (visitor->completNodeType)
			{
			case CompleteAssistASTVisitor::CompletionOnMethodName:
			case CompleteAssistASTVisitor::CompletionOnFieldName:
			case CompleteAssistASTVisitor::CompletionOnLocalFieldName:
				break;
			default:
				isMatchNameSuggest = false;
				break;
			}

			GetResult();
			if(!isMatchNameSuggest){
				for (auto itAst : homeMakeAst)
				{
					if (!itAst)
						continue;

					visitor->ResetForHomeMakeAstTraverse();
					itAst->traverse(visitor, visitor->completeScope);
					GetResult();
				}

			}
	
			if (!identiferForKeyWord.empty() && !isMatchNameSuggest) {
				ProccessKeyWord(identiferForKeyWord, visitor, preTokenKind);
			}

			

		}
		

		void CompletionEngine::findMethodParameterNames(MethodSymbol* method, vector<wstring>& parameterTypeNames, vector<wstring>& parameterNames) {
			if (!method)
				return;

			int length = parameterTypeNames.size();
			if (length == 0) {
				return ;
			}
			// look into the corresponding unit if it is available
			if (method->declaration) {
				auto  methodDecl = (AbstractMethodDeclaration*)(method->declaration);
				methodDecl->getParameterNames(parameterNames);
			}
			else{
				
				GetNameLookUp();
	
				auto type_symbol = method->containing_type;
				if (!type_symbol)
					return;
				auto package_symbol = type_symbol->ContainingPackage();
				if (!package_symbol)
					return ;

				wstring declaringTypePackageName;
				wstring declaringTypeName;

				vector<wstring> parameterPackageNames;
				vector<wstring> parameterTypeNames;
				const wchar_t* temp = package_symbol->PackageName();
				if (temp)
					declaringTypePackageName.assign(temp, temp + package_symbol->PackageNameLength());

				temp = type_symbol->Name();
				if (temp)
					declaringTypeName.assign(temp, temp + type_symbol->NameLength());

				method->GetParameterNames(parameterPackageNames, parameterTypeNames);

				boost::replace_all(declaringTypePackageName, L"/", L".");

				auto methods = selecttonRequestor->getMethod(declaringTypePackageName, declaringTypeName, method->getNameString(),
					parameterPackageNames, parameterTypeNames, method->IsConstructor());

				if(methods.empty())
					return;
				auto methodElement = methods[0];
				if (!methodElement)
					return;

				methodElement->getNameRange();
				parameterNames = methodElement->getParameterNames();

			}
		
		}


		NameLookup* CompletionEngine::GetNameLookUp()
		{
			if(!namel_lookup_){
				namel_lookup_ = project->getNameLookup();
				selecttonRequestor = new JavaModel::SelectionRequestor(project, layerPos);
				nameLookupRequestor = new SearchableEnvironmentRequestor(this, nullptr);
			}
			return  namel_lookup_;
		}

		CompletionEngine::CompletionEngine(
			boost::shared_ptr<JavaModel::OriginalJavaProject> proj,
			INameEnvironment* env, Option* opt, AssistInfo& info,
			JavaModel::LayerPosInJavaModel* _layerPos, 
			ICompletionRequestor* _requestor)
			:Engine(env, opt),
			visitor(nullptr),
			assistNodeIsClass(false), 
			assistNodeIsEnum(false),
			assistNodeIsException(false), assistNodeIsInterface(false), 
			insideQualifiedReference(false), 
			assistNodeIsExtendedType(false),
			assistNodeIsInsideCase(false),
			startPosition(0),
			 endPosition(0),
			 offset(0),
			actualCompletionPosition(0),
			lenEntered(0),
			tokenKind(TK_Identifier),
			 namel_lookup_(nullptr),
			selecttonRequestor(nullptr),
			hasImportWord(false),
			project(proj),
			nameLookupRequestor(nullptr),
			completionRang(nullptr),
			transitRequstor(nullptr),
			requestor(_requestor),
			importPackageNames(nullptr),
			file_type(JavaModel::_A_EDIT_FILE),
			_info(info),
			layerPos(_layerPos),
			keyWordWeightIncrease(false), isForCodeComplete(true)
		{	
			
		}

		CompletionEngine::~CompletionEngine()
		 {
			 if(namel_lookup_)
			 {
				 namel_lookup_->endToOccupy();
			 }
			 delete importPackageNames;
			 delete selecttonRequestor;
			 delete nameLookupRequestor;
			 delete transitRequstor;
			 delete completionRang;
			 delete visitor;
		 }


		void  CompletionEngine::findFieldsLoop(
			const wstring& fieldName,
			TypeSymbol* receiverType,
			TypeSymbol* currentType,
			AstNodeScope* scope,
			vector< pair< VariableSymbol* , TypeSymbol*>  >& fieldsFound,
			vector<VariableSymbol*>& localsFound,
			StaticAccess onlyStaticFields,
			InvocationSite* invocationSite,
			AstNodeScope*  invocationScope,
			bool implicitCall,
			set<TypeSymbol*>& visistedType, 
			TypeSymbol* variableType,
			ConstAccess constAccess
		)
		{
			if (visistedType.find(currentType) == visistedType.end())
			{
				visistedType.insert(currentType);
			}
			else
			{
				return;
			}

			if (!currentType)
				return;

			vector<VariableSymbol*> fields;
			currentType->availableFields(fields);
			if(fields.size())
			{
				findFields(
					fieldName,
					fields,
					scope,
					fieldsFound,
					localsFound,
					onlyStaticFields,
					receiverType,
					invocationSite,
					invocationScope,
					implicitCall ,
					variableType,
					constAccess);
			}
			
			if (currentType->super)
			{
				findFieldsLoop(fieldName,
					receiverType,
					currentType->super,
					scope,
					fieldsFound,
					localsFound,
					onlyStaticFields,
					invocationSite,
					invocationScope,
					implicitCall,
					visistedType,
					variableType,
					constAccess);
			}

			if (currentType->interfaces)
			{
				for (auto it : *(currentType->interfaces))
				{
					findFieldsLoop(fieldName,
						receiverType,
						it,
						scope,
						fieldsFound,
						localsFound,
						onlyStaticFields,
						invocationSite,
						invocationScope,
						implicitCall,
						visistedType,
						variableType,
						constAccess
					);
				}
			}

		}


		void CompletionEngine::findFields(
			const wstring&  fieldName,
			vector<VariableSymbol*>& fields,
			AstNodeScope* scope,
			vector< pair< VariableSymbol* , TypeSymbol* > >& fieldsFound,
			vector<  VariableSymbol* >& localsFound,
			StaticAccess onlyStaticFields,
			TypeSymbol* receiverType,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall ,
			TypeSymbol* variableType,
			ConstAccess constAccess)
		{
			// Inherited fields which are hidden by subclasses are filtered out
			// No visibility checks can be performed without the scope & invocationSite
			 int fieldLength = fieldName.length();
			 for (int k = fields.size(); --k >= 0;) {
				auto field = fields[k];

				if (!field)
					continue;

				if (field->ACC_SYNTHETIC())	
					continue ;
				if (onlyStaticFields == ONLY_STATIC && !field->ACC_STATIC()) 
					continue ;
				if(constAccess == ONLY_CONST || assistNodeIsInsideCase){
					if (!field->ACC_FINAL() || !field->initial_value)
						continue;
				}

				if (fieldLength > field->NameLength()) 
					continue ;
				auto fieldNameString = field->getNameString();
				if (!CharOperation::prefixEquals(fieldName, fieldNameString, false /* ignore case */))
					continue ;

				if (options.checkVisibility
					&& !field->canBeSeenBy(receiverType, invocationSite, scope))
					continue ;
				bool continue_next = false;
				bool prefixRequired = false;

				for (int i = fieldsFound.size(); --i >= 0;) {
				
					auto otherField = fieldsFound[i].first;
					auto otherReceiverType = fieldsFound[i].second;

					if (!otherField || !otherReceiverType)
						continue;

					if (field == otherField && receiverType == otherReceiverType)
					{
						continue_next = true;
						break;
					}
					
					if (CharOperation::equals(field->getNameString(), otherField->getNameString(), true)) {

						auto field_declaringClass = (TypeSymbol*)(field->owner);
						auto otherField_declaringClass = (TypeSymbol*)(otherField->owner);
						if(!otherField_declaringClass || !field_declaringClass)
						{
							continue_next = true;
							continue;
						}
						if (otherField_declaringClass->IsSubclass(field_declaringClass))
						{
							continue_next = true;
							break;
						}
						if (otherField_declaringClass->ACC_INTERFACE())

							if (field_declaringClass->Implements(otherField_declaringClass))
							{
								continue_next = true;
								break;
							}
						if (field_declaringClass->ACC_INTERFACE())
							if (otherField_declaringClass->Implements(field_declaringClass))
							{
								continue_next = true;
								break;
							}
						prefixRequired = true;
					}
				}

				if (continue_next)
					continue;
				if (fieldNameString == RecoveryScanner::FAKE_IDENTIFIER)
					continue;
				for (int l = localsFound.size(); --l >= 0;) {
					auto local = localsFound[l];

					if (CharOperation::equals(field->getNameString(), local->getNameString(), true)) {

						auto  declarationType = ScopeUtil::enclosingSourceType(scope);
						if (declarationType->Anonymous() && declarationType != ScopeUtil::enclosingSourceType(invocationScope)) {
							continue_next = true;
							break;
						}
						prefixRequired = true;
						break;
					}
				}

				if (continue_next)
					continue;


				if(variableType && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(variableType, field->Type()))
					continue;

				fieldsFound.push_back( std::make_pair( field, receiverType ));

				auto completion = field->getNameString();

				if (prefixRequired || options.forceImplicitQualification) {

					auto prefix = computePrefix( 
						ScopeUtil::enclosingSourceType(scope),
						ScopeUtil::enclosingSourceType(invocationScope) 
						, field->ACC_STATIC()
					);
					completion = prefix + L"." +  completion;
				}

				
			

				double  relevance = computeBaseRelevance();
				
				relevance += computeRelevanceForExpectingType(field->Type());
				relevance += computeRelevanceForEnumConstant(field->Type());
				relevance += computeRelevanceForStatic(onlyStaticFields, field->ACC_STATIC());
				relevance += computeRelevanceForFinal(assistNodeIsInsideCase, field->ACC_FINAL());
			
				if (onlyStaticFields && insideQualifiedReference) {
					relevance += computeRelevanceForInheritance(receiverType, field->ContainingType());
				}
				
				auto declaringClass = (TypeSymbol*)field->owner;

				if (!declaringClass)
					return;

				wstring declaringTypePackageName =declaringClass->ContainingPackageName();
				boost::replace_all(declaringTypePackageName, L"/", L".");
				auto fieldType = field->Type();
				if (!fieldType)
					return;

				requestor
					->acceptField(declaringTypePackageName,
						declaringClass->qualifiedSourceName('.'),
						field->getNameString(),
						fieldType->ContainingPackageName(),
						fieldType->qualifiedSourceName('.'),
						completion,
						// may include some qualification to resolve ambiguities
						field->Flags(),
						completionRang,
						relevance);
			}
		}

		void CompletionEngine::findFieldsAndMethods(
			const wstring& token,
			TypeSymbol* receiverType,
			AstNodeScope* scope ,
			InvocationSite* invocationSite,
			AstNodeScope*  invocationScope,
			bool implicitCall, 
			TypeSymbol* fieldTypeOrMethodReturnType,
			vector<   pair< VariableSymbol*, TypeSymbol*> >& fieldsFound,
			vector<  VariableSymbol* >& localsFound)
		{
			if (!receiverType)
				return;

			if ( lookupEnvironment->IsBaseType(receiverType))
				return; // nothing else is possible with base types

			if (receiverType->IsArray()) {
				if (token.length() <= lengthField.length()
					&& CharOperation::prefixEquals(token, lengthField, false /* ignore case */
					)) {

					double  relevance = computeBaseRelevance();
				
					relevance += computeRelevanceForExpectingType(lookupEnvironment->int_type);
				
					requestor->acceptField(
						EMPTY_TOKEN,
						EMPTY_TOKEN,
						lengthField,
						EMPTY_TOKEN,
						EMPTY_TOKEN,
						lengthField,
						AccessFlags::ACCESS_PUBLIC,
						completionRang,
						relevance);
				}
				receiverType = lookupEnvironment->Object();
			}
		
			vector<TypeSymbol*>  argTypes;		
			vector<  pair< MethodSymbol*, TypeSymbol*>  > methodsFound;

			findFields(
				token,
				(TypeSymbol*)receiverType,
				scope,
				fieldsFound,
				localsFound,
				DO_NOT_CARE_STATIC,
				invocationSite,
				invocationScope,
				implicitCall,
				fieldTypeOrMethodReturnType,
				DO_NOT_CARE_CONST
			);

			findMethods(
				token,
				argTypes,
				(TypeSymbol*)receiverType,
				scope,
				methodsFound,
				DO_NOT_CARE_STATIC,
				false,
				false,
				invocationSite,
				invocationScope,
				implicitCall, 
				fieldTypeOrMethodReturnType
			);
		}

	
		void CompletionEngine::ProcessCompletionOnMemberAccess(FieldReference* field_reference, AstNodeScope* scope)
		{
		    if(!field_reference)
		    {
				return;
		    }
			insideQualifiedReference = true;
			if (tokenKind == TK_DOT)
			{
				if (!field_reference->symbol)
					return;
				vector<   pair< VariableSymbol*, TypeSymbol*> > fields;
				vector<  VariableSymbol* > localsFound;
				auto symbol = field_reference->symbol;
				if(symbol->_kind == Symbol::VARIABLE)
				{
					auto variableS = reinterpret_cast<VariableSymbol*>(symbol);
					auto qualifiedBinding = variableS->Type();
					if (!qualifiedBinding || qualifiedBinding == lookupEnvironment->no_type)
						return;
					keyWordWeightIncrease = true;
					findFieldsAndMethods(
						EMPTY_TOKEN,
						(TypeSymbol*)(qualifiedBinding),
						scope,
						field_reference,
						scope,
						false,
						NO_TYPE_SYMBOL,
						fields,
						localsFound);

				}
				else if(symbol->_kind == Symbol::TYPE)
				{
					auto qualifiedBinding = symbol;
					if (qualifiedBinding == lookupEnvironment->no_type)
						return;
					keyWordWeightIncrease = true;
					findFieldsAndMethods(
						EMPTY_TOKEN,
						(TypeSymbol*)(qualifiedBinding),
						scope,
						field_reference,
						scope,
						false, 
						NO_TYPE_SYMBOL,
						fields,
						localsFound);
				}
				else if(symbol->_kind == Symbol::PACKAGE)
				{
					auto qualifiedBinding = symbol;
					findTypesAndSubpackages(EMPTY_TOKEN, (PackageSymbol*)qualifiedBinding);
				}
			}
			else
			{
				auto identifier_token = field_reference->identifier_token;
				if(!identifier_token)
				{
					return;
				}
				auto	token = identifier_token->getName();
				if (identifier_token->start_location <= actualCompletionPosition)
				{
					if (tokenKind == TK_DOT)
						token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
					else
						token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
				}
				if(!field_reference->base->symbol || !field_reference->base->symbol)
				{
					return;
				}
				Symbol*  qualifiedBinding = field_reference->base->symbol;
				if (qualifiedBinding && qualifiedBinding->_kind == Symbol::TYPE)
				{
					requestor->SetTokenForMatch(token);
					vector<   pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
					vector<  VariableSymbol* > localsFound;
					keyWordWeightIncrease = true;
					findFieldsAndMethods(
						/*token*/ EMPTY_TOKEN,
						(TypeSymbol*)(qualifiedBinding),
						scope,
						field_reference,
						scope,
						false, 
						NO_TYPE_SYMBOL,
						fieldsFound,
						localsFound);
				}
			}
			
			
		}
		void CompletionEngine::findImplicitMessageSends(
			const wstring& token,
			vector<TypeSymbol*>& argTypes,
			AstNodeScope* scope,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope )
		{

		
			StaticAccess staticsOnly = DO_NOT_CARE_STATIC;
			// need to know if we're in a static context (or inside a constructor)
			vector<  pair< MethodSymbol*, TypeSymbol*>  > methodsFound;

	
		 	while (scope)
			{ // done when a COMPILATION_UNIT_SCOPE is found

				switch (scope->GetScopeKind()) 
				{

					case AstNodeScope::METHOD_SCOPE:
					{
						// handle the error case inside an explicit constructor call (see MethodScope>>findField)
						auto methodScope = (MethodSymbol*)scope->GetSymbol();
						if(methodScope)
						{
							if (methodScope->ACC_STATIC())
								staticsOnly = ONLY_STATIC;
						}
						
					}
					
						break;
					case AstNodeScope::CLASS_SCOPE:
					{
						auto classScope = scope;
						auto  enclosingType = (TypeSymbol*)scope->GetSymbol();
						if(!enclosingType){
							return;
						}

						findMethods(
							token,
							argTypes,
							enclosingType,
							classScope,
							methodsFound,
							staticsOnly,
							true,
							false,
							invocationSite,
							invocationScope, true,nullptr);
						if(enclosingType->ACC_STATIC())
							staticsOnly = ONLY_STATIC ;
					}
					
						break;
					case AstNodeScope::COMPILATION_UNIT_SCOPE:
						return;

					default:
						return;
				}
				scope = scope->parent_scope;
			}
		}



		void CompletionEngine::findIntefacesMethodsLoop(
			const wstring&  selector,
			vector<TypeSymbol*>&  argTypes,
			TypeSymbol* currentType,
			TypeSymbol* receiverType,
			AstNodeScope* scope,
			vector<  pair< MethodSymbol*, TypeSymbol*>  >& methodsFound,
			StaticAccess onlyStaticMethods,
			bool exactMatch,
			bool isCompletingDeclaration,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall,
			set<TypeSymbol*>& visitedTypes, TypeSymbol* returnType)
		 {
			if (!currentType)
				return;

			 if(visitedTypes.find(currentType) != visitedTypes.end()){
				 return;
			 }

			 vector<MethodSymbol*> methods;
			 currentType->availableMethods(methods);
			 visitedTypes.insert(currentType);
			 if (isCompletingDeclaration) {
				 findLocalMethodDeclarations(
					 selector,
					 methods,
					 scope,
					 methodsFound,
					 onlyStaticMethods,
					 exactMatch,
					 receiverType, returnType);

			 }
			 else {
				 findLocalMethods(
					 selector,
					 argTypes,
					 methods,
					 scope,
					 methodsFound,
					 onlyStaticMethods,
					 exactMatch,
					 receiverType,
					 invocationSite,
					 invocationScope,
					 implicitCall, returnType);
			 }
			 if(currentType->interfaces)
			 {
				 for(auto aInterface : *(currentType->interfaces))
				 {
					 findIntefacesMethodsLoop(
						 selector,
						 argTypes,
						 aInterface,
						 receiverType,
						 scope,
						 methodsFound,
						 onlyStaticMethods,
						 exactMatch,
						 isCompletingDeclaration,
						 invocationSite,
						 invocationScope,
						 implicitCall,
						 visitedTypes, returnType);
				 }
			 }
		 }


		void CompletionEngine::findIntefacesMethods(
			const wstring&  selector,
			vector<TypeSymbol*>&  argTypes,
			TypeSymbol* receiverType,
			vector<TypeSymbol*>* itsInterfaces,
			AstNodeScope* scope,
			vector< pair<MethodSymbol*, TypeSymbol*>   >& methodsFound,
			StaticAccess onlyStaticMethods,
			bool exactMatch,
			bool isCompletingDeclaration,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall, TypeSymbol* returnType)
		{
			

			if (!itsInterfaces)
				return;
			
			set<TypeSymbol*> visitedTypes;
			for(auto aInterface : *itsInterfaces)
			{
				findIntefacesMethodsLoop(selector,
					argTypes,
					aInterface,
					receiverType,
					scope,
					methodsFound,
					onlyStaticMethods,
					exactMatch,
					isCompletingDeclaration,
					invocationSite,
					invocationScope,
					implicitCall,
					visitedTypes, returnType);
			}
		
		}
		void CompletionEngine::findLocalMethodDeclarations(
			const wstring& methodName,
			vector< MethodSymbol* >& methods,
			AstNodeScope* scope,
			vector<  pair< MethodSymbol*, TypeSymbol*>  >& methodsFound,
			//	bool noVoidReturnType, how do you know?
			StaticAccess onlyStaticMethods,
			bool exactMatch,
			TypeSymbol* receiverType, TypeSymbol* returnType)
		{

			// Inherited methods which are hidden by subclasses are filtered out
			// No visibility checks can be performed without the scope & invocationSite
			int methodLength = methodName.length();
		 for (int f = methods.size(); --f >= 0;) {

			auto method = methods[f];
			if(!method)
				continue;
			if (method->ACC_SYNTHETIC())
				continue ;

		/*	if (method->ACC_ABSTRACT())
				continue ;*/

			if (method->IsConstructor())
				continue ;

			if (method->ACC_FINAL()) 
				continue ;

			//		if (noVoidReturnType && method.returnType == BaseTypes.VoidBinding) continue next;
			if (onlyStaticMethods == ONLY_STATIC && !method->ACC_STATIC()) 
				continue ;

			if (options.checkVisibility
				&& !method->canBeSeenBy(receiverType, &FakeInvocationSite, scope)) 
				continue ;

			if (exactMatch) {
				if (!CharOperation::equals(methodName, method->getNameString(), false /* ignore case */
				))
					continue ;

			}
			else {

				if (methodLength > method->getNameString().length())
					continue ;

				if (!CharOperation::prefixEquals(methodName, method->getNameString(), false
					/* ignore case */
				))
					continue ;
			}
			bool continue_next = false;
			for (int i = methodsFound.size(); --i >= 0;) {
				auto otherMethod = methodsFound[i].first;
				if(!otherMethod)
				{
					continue;
				}
				if (method == otherMethod)
				{
					continue_next = true;
					break;
				}
				
				if (CharOperation::equals(method->getNameString(), otherMethod->getNameString(), true)
					&& method->areParametersEqual(otherMethod)) 
				{
					continue_next = true;
					break;
				}
			}
			if(continue_next)
				break;

			auto __returnType = method->Type();
			if (returnType && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(returnType, __returnType))
				continue;

			methodsFound.push_back( make_pair( method, nullptr ) );
			  
			int length = method->NumFormalParameters();
			vector<wstring>  parameterPackageNames(length);
			vector<wstring>  parameterTypeNames(length);


			for (int i = 0; i < length; i++) {
				if(!method->FormalParameter(i))
				{
					continue;
				}
				auto  type = method->FormalParameter(i)->Type();
				wstring temp = type->ContainingPackageName();
				parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
				parameterTypeNames[i] = type->qualifiedSourceName('.');
			}

			vector<wstring> parameterNames;
			findMethodParameterNames(method, parameterTypeNames, parameterNames);


		
			wstring returnTypePackageName = __returnType->ContainingPackageName();
			boost::replace_all(returnTypePackageName, L"/", L".");

			wstring  completion;
			// flush uninteresting modifiers
			AccessFlags modifier(method->Flags());
			modifier.ResetACC_NATIVE();
			modifier.ResetACC_ABSTRACT();

			
			if (!exactMatch) {
				if (modifier.Flags() ) {
					std::vector< std::wstring > modifiers;
					modifier.GetModifers(modifiers);
					completion += boost::join(modifiers, L" ");
				}

				wstring returnPackageName = returnTypePackageName;
				auto returnTypeName = returnType->qualifiedSourceName('.');

				if ( mustQualifyType(returnPackageName, returnTypeName) ) {
					completion += returnPackageName + L"." + returnTypeName;
				}
				else {
					completion.append(returnType->sourceName('.'));
				}

				completion.append(L" ");
				completion.append( method->getNameString() );
				completion.append( L"(" );

				for (int i = 0; i < length; i++) {
					if (mustQualifyType(parameterPackageNames[i], parameterTypeNames[i])) {
						completion +=  parameterPackageNames[i] + L"." +parameterTypeNames[i];
					}
					else {
						completion.append(parameterTypeNames[i]);
					}
					completion.append(L" ");

					if (parameterNames.empty() != true) {
						completion.append(parameterNames[i]);
					}
					else {
						completion.push_back('%');
					}

					if (i != (length - 1))
						completion.push_back(',');
				}
				completion.push_back(')');

				auto exceptions_length = method->NumThrows();

				if (exceptions_length > 0) {
					completion.push_back(' ');
					completion.append(THROWS);
					completion.push_back(' ');
					for (int i = 0; i < exceptions_length; i++) {
						auto exception = method->Throws(i);
						if(!exception)
						{
							continue;
						}
						wstring exceptionPackageName = exception->ContainingPackageName();
						boost::replace_all(exceptionPackageName, L"/", L".");
						auto exceptionTypeName = exception->qualifiedSourceName('.');

						if (i != 0) {
							completion.push_back(',');
							completion.push_back(' ');
						}

						if (mustQualifyType(exceptionPackageName, exceptionTypeName)) {
							completion += exceptionPackageName + L"." + exceptionTypeName;
						}
						else {
							completion.append(exception->sourceName('.'));
						}
					}
				}
			}


			
			double  relevance = computeBaseRelevance();
		
			relevance += computeRelevanceForStatic(onlyStaticMethods == ONLY_STATIC , !method->ACC_STATIC());
		



			auto declaringClass = method->containing_type;
			 if(!declaringClass)
			 {
				 return;
			 }
			wstring declaringTypePackageName = declaringClass->ContainingPackageName();
			boost::replace_all(declaringTypePackageName, L"/", L".");

		
			requestor->acceptMethodDeclaration(
				declaringTypePackageName,
				declaringClass->qualifiedSourceName('.'),
				method->getNameString(),
				parameterPackageNames,
				parameterTypeNames,
				parameterNames,
				returnTypePackageName,
				returnType->qualifiedSourceName('.'),
				completion,
				method->Flags(),
				completionRang,
				relevance);
		}
		}
		void  CompletionEngine::findLocalMethods(
			const wstring& methodName,
			vector<TypeSymbol*>& argTypes,
			vector< MethodSymbol* >& methods,
			AstNodeScope* scope,
			vector< pair< MethodSymbol* , TypeSymbol*>  >& methodsFound,
			StaticAccess onlyStaticMethods,
			bool exactMatch,
			TypeSymbol* receiverType,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope,
			bool implicitCall, TypeSymbol* returnType)
		{
			// Inherited methods which are hidden by subclasses are filtered out
			// No visibility checks can be performed without the scope & invocationSite

			int methodLength = methodName.length();
			int minArgLength = argTypes.size();

		 for (int k = methods.size(); --k >= 0;) {
			auto method = methods[k];
			if(!method)
			{
				continue;
			}
			if (method->ACC_SYNTHETIC()) 
				continue ;

			/*	if (method->ACC_ABSTRACT())
					continue ;
	*/
			if (method->IsConstructor() )
				continue ;


			//		if (noVoidReturnType && method.returnType == BaseTypes.VoidBinding) continue next;
			if (onlyStaticMethods == ONLY_STATIC && !method->ACC_STATIC()) 
				continue ;


			if (options.checkVisibility
				&& !method->canBeSeenBy(receiverType, invocationSite, scope))
				continue ;

			if (exactMatch) {
				if (!CharOperation::equals(methodName, method->getNameString(), false /* ignore case */
				))
					continue ;

			}
			else {

				if (methodLength > method->getNameString().length())
					continue ;

				if (!CharOperation::prefixEquals(methodName, method->getNameString(), false
					/* ignore case */
				))
					continue ;
			}
			if (minArgLength > method->NumFormalParameters())
				continue ;
			bool continue_next = false;
			for (int a = minArgLength; --a >= 0;) {
				auto arg = argTypes[a];
				if(arg != lookupEnvironment->no_type &&  arg != nullptr
					&& !lookupEnvironment->system_semantic->CanMethodInvocationConvert(argTypes[a],
						method->FormalParameter(a)->Type()) )
				{
					continue_next = true;
					break;
				}
				
			}
			if (continue_next)
				continue;

			bool prefixRequired = false;

			for (int i = methodsFound.size(); --i >= 0;) {
				
				auto otherMethod = methodsFound[i].first;
				auto otherReceiverType = methodsFound[i].second;
				if (method == otherMethod && receiverType == otherReceiverType)
				{
					continue_next = true;
					break;
				}
				if(!otherMethod)
				{
					continue;
				}
				if (CharOperation::equals(method->getNameString(), otherMethod->getNameString(), true)
					&& method->areParametersEqual(otherMethod)) 
				{
					if (! (otherMethod->containing_type && method->containing_type) )
					{
						continue;
					}
					if(otherMethod->containing_type->IsSubclass(method->containing_type))
					{
						continue_next = true;
						break;
					}
					if (otherMethod->containing_type->ACC_INTERFACE())
						if (method->containing_type->Implements(otherMethod->containing_type))
						{
							continue_next = true;
							break;
						}

					if (method->containing_type->ACC_INTERFACE() )
						if (otherMethod->containing_type->Implements(method->containing_type))
						{
							continue_next = true;
							break;
						}
					prefixRequired = true;
				}
			}


			auto __returnType = method->Type();
			if (returnType && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(returnType, __returnType))
				continue;

			methodsFound.push_back( make_pair( method, receiverType ));

			int length = method->NumFormalParameters();
			vector<wstring>  parameterPackageNames(length);
			vector<wstring>  parameterTypeNames(length);


			for (int i = 0; i < length; i++) {
				auto  type = method->FormalParameter(i)->Type();
				wstring temp = type->ContainingPackageName();
				parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
				parameterTypeNames[i] = type->qualifiedSourceName('.');
			}

			vector<wstring> parameterNames;


			findMethodParameterNames(method, parameterTypeNames, parameterNames);

			wstring completion;

			int previousStartPosition = startPosition;

			// nothing to insert - do not want to replace the existing selector & arguments
			if (!exactMatch) {
					if (cunit->lex_stream == nullptr
						|| cunit->lex_stream->InputBufferLength() > endPosition
						|| cunit->lex_stream->InputBuffer()[endPosition] != '(')
					completion = method->getNameString();
				else
					completion = method->getNameString() +  L"()";
			}
			else {

				if (prefixRequired && (cunit->lex_stream != nullptr)) {
					auto buf = cunit->lex_stream->InputBuffer();
					completion.assign(buf + startPosition, buf + endPosition);
				}
				else {
					startPosition = endPosition;
				}
			}

			if (prefixRequired || options.forceImplicitQualification) {
				auto prefix = computePrefix(ScopeUtil::enclosingSourceType(scope), ScopeUtil::enclosingSourceType(invocationScope)
					, method->ACC_STATIC());
				completion =prefix + L"." + completion;
			}

			double  relevance = R_DEFAULT;
			
			relevance += computeRelevanceForExpectingType(method->Type());

			auto declaringClass = method->containing_type;
			if( !declaringClass )
			{
				return;
			}
			wstring declaringTypePackageName = declaringClass->ContainingPackageName();
			boost::replace_all(declaringTypePackageName, L"/", L".");

		
			wstring returnTypePackageName = __returnType->ContainingPackageName();
			boost::replace_all(returnTypePackageName, L"/", L".");
			requestor->acceptMethod(
				declaringTypePackageName,
				declaringClass->qualifiedSourceName('.'),
				method->getNameString(),
				parameterPackageNames,
				parameterTypeNames,
				parameterNames,
				returnTypePackageName,
				__returnType->qualifiedSourceName('.'),
				completion,
				method->Flags(),
				completionRang,
				relevance);

			startPosition = previousStartPosition;
		}
		}
		void CompletionEngine::findMethods(
			const wstring&  selector,
			vector<TypeSymbol*>&  argTypes,
			TypeSymbol* receiverType,
			AstNodeScope*  scope,
			vector<  pair< MethodSymbol* , TypeSymbol*>  >& methodsFound,
			StaticAccess onlyStaticMethods,
			bool exactMatch,
			bool isCompletingDeclaration,
			InvocationSite* invocationSite,
			AstNodeScope*  invocationScope,
			bool implicitCall,
			TypeSymbol* returnType)
		{
			if (assistNodeIsInsideCase)
				return;		// no methods should be proposed inside case expression
			if(!receiverType)
			{
				return;
			}

			if (isCompletingDeclaration) {

				auto methods_length = receiverType->NumMethodSymbols();
				if (methods_length != 0) {
					for (int i = 0; i < methods_length; i++) {
						auto method = receiverType->MethodSym(i);

						if (method && !method->ACC_STATIC()) {
							methodsFound.push_back( make_pair(method,nullptr) );
						}
					}
				}
			}

			auto currentType = receiverType;
			if (receiverType->ACC_INTERFACE()) {
				if (isCompletingDeclaration) {

					findIntefacesMethods(
						selector,
						argTypes,
						receiverType,
						currentType->interfaces,
						scope,
						methodsFound,
						onlyStaticMethods,
						exactMatch,
						isCompletingDeclaration,
						invocationSite,
						invocationScope,
						implicitCall , 
						returnType);

				}
				else {
					vector<TypeSymbol*> itsInterfaces = { currentType };
					findIntefacesMethods(
						selector,
						argTypes,
						receiverType,
						&itsInterfaces,
						scope,
						methodsFound,
						onlyStaticMethods,
						exactMatch,
						isCompletingDeclaration,
						invocationSite,
						invocationScope,
						implicitCall,
						returnType);
				}

				currentType = lookupEnvironment->Object();
			}
			else {
				if (isCompletingDeclaration) {
					
					findIntefacesMethods(
						selector,
						argTypes,
						receiverType,
						(currentType->interfaces),
						scope,
						methodsFound,
						onlyStaticMethods,
						exactMatch,
						isCompletingDeclaration,
						invocationSite,
						invocationScope,
						implicitCall,
						returnType);
					currentType = receiverType->super;

				}
			}
			bool hasPotentialDefaultAbstractMethods = true;
			while (currentType != nullptr) {
				vector<MethodSymbol*> methods;
			    currentType->availableMethods(methods);

				if (methods.empty() != true) {
					if (isCompletingDeclaration) {
						findLocalMethodDeclarations(
							selector,
							methods,
							scope,
							methodsFound,
							onlyStaticMethods,
							exactMatch,
							receiverType,
							returnType);
					}
					else {
						findLocalMethods(
							selector,
							argTypes,
							methods,
							scope,
							methodsFound,
							onlyStaticMethods,
							exactMatch,
							receiverType,
							invocationSite,
							invocationScope,
							implicitCall,
							returnType);
					}
				}

				if (hasPotentialDefaultAbstractMethods && currentType->ACC_ABSTRACT() ) {
					findIntefacesMethods(
						selector,
						argTypes,
						receiverType,
						currentType->interfaces,
						scope,
						methodsFound,
						onlyStaticMethods,
						exactMatch,
						isCompletingDeclaration,
						invocationSite,
						invocationScope,
						implicitCall,
						returnType);
				}
				else {
					hasPotentialDefaultAbstractMethods = false;
				}
				currentType = currentType->super;
			}

		}
		void CompletionEngine::findConstructors(
			TypeSymbol* currentType,
			vector<TypeSymbol*>&  argTypes,
			AstNodeScope*   scope,
			InvocationSite* invocationSite,
			bool forAnonymousType,
			MethodSymbol* excludeConstructor)
		{
			// No visibility checks can be performed without the scope & invocationSite
			if (!currentType)
				return;

			auto methods_length = currentType->NumMethodSymbols();

			if (methods_length == 0)
			{
				return;
			}
			
			int minArgLength =  argTypes.size();


			for (int k = methods_length; --k >= 0;) 
			{
				auto constructor = currentType->MethodSym(k);
				if (!constructor || !constructor->IsConstructor())
					continue;


				if (constructor->ACC_SYNTHETIC())
					continue ;

				if(excludeConstructor && excludeConstructor == constructor){
					continue;
				}
				if (options.checkVisibility
					&& !constructor->canBeSeenBy(invocationSite, scope))
					continue ;

				int paramLength = constructor->NumFormalParameters();
				if (minArgLength > paramLength)
					continue ;
				bool continue_next = false;
				for (int a = minArgLength; --a >= 0;)
				{
					if (argTypes[a] != nullptr 
						&& !lookupEnvironment->system_semantic->CanMethodInvocationConvert(argTypes[a],
					       constructor->FormalParameter(a)->Type())) // can be null if it could not be resolved properly
					{
						continue_next = true;
						break;
					}
						
				}
				if (continue_next)
					continue;


				int length = constructor->NumFormalParameters();
				vector<wstring>  parameterPackageNames(length);
				vector<wstring>  parameterTypeNames(length);


				for (int i = 0; i < length; i++) {
					auto  type = constructor->FormalParameter(i)->Type();
					if (!type)
						continue;

					wstring temp = type->ContainingPackageName();
					parameterPackageNames[i] = boost::replace_all_copy(temp, L"/", L".");
					parameterTypeNames[i] = type->qualifiedSourceName('.');
				}

				vector<wstring> parameterNames;
				findMethodParameterNames(constructor, parameterTypeNames, parameterNames);

				double  relevance = computeBaseRelevance();
			
			
				wstring  completion;
				// nothing to insert - do not want to replace the existing selector & arguments
				if (   cunit->lex_stream == nullptr
					|| cunit->lex_stream->InputBufferLength() <= endPosition
					|| cunit->lex_stream->InputBuffer()[endPosition] != ')')
				
					completion = L")";

				if (forAnonymousType) {
					wstring typePackageName = currentType->ContainingPackageName();
					boost::replace_all(typePackageName, L"/", L".");
					requestor->acceptAnonymousType(
						typePackageName,
						currentType->qualifiedSourceName('.'),
						parameterPackageNames,
						parameterTypeNames,
						parameterNames,
						completion,
						constructor->Flags(),
						completionRang,
						relevance);
				}
				else {
					wstring typePackageName = currentType->ContainingPackageName();
					boost::replace_all(typePackageName, L"/", L".");
					requestor->acceptMethod(
						typePackageName,
						currentType->qualifiedSourceName('.'),
						currentType->sourceName('.'),
						parameterPackageNames,
						parameterTypeNames,
						parameterNames,
						EMPTY_TOKEN,
						EMPTY_TOKEN,
						completion,
						constructor->Flags(),
						completionRang,
						R_DEFAULT);
				}
				
			}
		}

		void  CompletionEngine::findAnonymousType(
			TypeSymbol* currentType,
			vector<TypeSymbol*>&  argTypes,
			AstNodeScope* scope,
			InvocationSite* invocationSite)
		{
			if (!currentType)
				return;

			if (currentType->ACC_INTERFACE()) {
				wstring completion;
				// nothing to insert - do not want to replace the existing selector & arguments
				if (cunit->lex_stream == nullptr
					|| cunit->lex_stream->InputBufferLength() <= endPosition
					|| cunit->lex_stream->InputBuffer()[endPosition] != ')')
					completion = L")";

				requestor->acceptAnonymousType(
					currentType->ContainingPackageName(),
					currentType->qualifiedSourceName('.'),
					{},
					{},
					{},
					completion,
					AccessFlags::ACCESS_PUBLIC,
					completionRang,
					R_DEFAULT);
			}
			else {
				findConstructors(
					currentType,
					argTypes,
					scope,
					invocationSite,
					true,
					nullptr);
			}
		}

		wstring computeBaseNames(wchar_t firstName, vector<wstring>&  excludeNames) {
			wstring  name =  {firstName};

			for (int i = 0; i < excludeNames.size(); i++) {
				if (CharOperation::equals(name, excludeNames[i], false)) {
					name[0]++;
					if (name[0] > 'z')
						name[0] = 'a';
					if (name[0] == firstName)
						return {};

					i = 0;
				}
			}

			return name;
		}

		void computeNames(const wstring& sourceName, bool forArray, vector<wstring> names) {
		 
			
			bool previousIsUpperCase = false;
			for (int i = sourceName.length() - 1; i >= 0; i--) {
				bool isUpperCase = std::isupper(sourceName[i]);
				if (isUpperCase && !previousIsUpperCase) {
					auto  name = sourceName.substr(i, sourceName.length());
					if (name.length() > 1) {
						
						name[0] = std::tolower(name[0]);

						if (forArray) {
							int length = name.length();
							if (name[length - 1] == 's') {
								name.append(L"es");
							}
							else {
								name.push_back('s');
							}
						}
						names.push_back(name);
					}
				}
				previousIsUpperCase = isUpperCase;
			}

			if (names.size() == 0) {
				auto name = boost::to_lower_copy(sourceName);
				if (forArray) {
					int length = name.length();
					if (name[length - 1] == 's') {
						name.append(L"es");
					}
					else {
						name.push_back('s');
					}
				}
				names.push_back(name);
			}
		}


		void CompletionEngine::findVariableName(const wstring& token, const wstring& qualifiedPackageName, 
			const wstring& qualifiedSourceName, const wstring& sourceName, vector<wstring>& excludeNames, int dim) {
		
			wstring  name;

			// compute variable name for base type
		    for(auto baseName : baseTypes)
		    {
				if(baseName == sourceName)
				{
					if (token.length() == 0)
						return;
					name = computeBaseNames(sourceName[0], excludeNames);
					return;
				}
				
		    }
				
			if (name.empty() != true) {
				double  relevance = R_DEFAULT;
			
				// accept result
				requestor->acceptVariableName(
					qualifiedPackageName,
					qualifiedSourceName,
					name,
					name,
					completionRang,
					relevance);
				return;
			}
			
			

			// compute variable name for non base type
			vector<wstring>  names;
			 computeNames(sourceName, dim > 0, names);

			wstring displayName ;
			if (dim > 0) {
				int l = qualifiedSourceName.length();
				displayName.reserve(l + (2 * dim));
				displayName = qualifiedSourceName;
				for (int i = 0; i < dim; i++) {
					displayName.append(L"[]");
				}
			}
			else {
				displayName = qualifiedSourceName;
				if(!qualifiedSourceName.empty()){
					auto camelPart = qualifiedSourceName;
					camelPart[0] = toupper(camelPart[0]);
					names.push_back(token + camelPart);
				}
			
			}


			for (int i = 0; i < names.size(); i++) 
			{
			name = names[i];

			if (!CharOperation::prefixEquals(token, name, false))
				continue ;

			// completion must be an identifier (not a keyword, ...).
			if(CodeAssistUtil::IsReservedJavaToken(name))
			{
				continue ;
			}
			
			int count = 2;
			auto originalName = name;
			for (int j = 0; j < excludeNames.size(); j++) {
				if (CharOperation::equals(name, excludeNames[j], false)) {
					Jikes::wstringex format;
					format.format(L"%d", count++);
					name = originalName + format;
					j = 0;
				}
			}

			double  relevance = R_DEFAULT;
		
			// accept result
			requestor->acceptVariableName(
				qualifiedPackageName,
				displayName,
				name,
				name,
				completionRang,
				relevance);
		}
		}

		void CompletionEngine::findVariableNames(const wstring&  name, AstType* type, vector< wstring > &  excludeNames)
		{
			
			if (type != nullptr &&
				type->symbol != nullptr) {
				auto tb = type->symbol;
				auto leafComponentType =  tb->leafComponentType();
				if (leafComponentType && leafComponentType != lookupEnvironment->no_type )
				{
					
					findVariableName(
						name,
						leafComponentType->ContainingPackageName(),
						leafComponentType->qualifiedSourceName('.'),
						leafComponentType->sourceName('.'),
						excludeNames,
						type->dimensions());
				}	
			}
		}
		void CompletionEngine::ProcessCompletionOnMessageSend(AstMethodInvocation* messageSend, AstNodeScope* scope)
		{
			if (!messageSend)
				return;

			Symbol* qualifiedBinding;
			if(messageSend->base_opt){
				qualifiedBinding = messageSend->base_opt->symbol;
				if(qualifiedBinding)
				{
					switch (qualifiedBinding->_kind)
					{
					case Symbol::TYPE:
					{
						auto temp = reinterpret_cast<TypeSymbol*>(qualifiedBinding);
						if (temp->num_dimensions)
						{
							qualifiedBinding = lookupEnvironment->Object();
						}
						else if (lookupEnvironment->IsBaseType(temp))
						{
							return;
						}
					}
						break;

					case Symbol::VARIABLE: {
						auto temp = reinterpret_cast<VariableSymbol*>(qualifiedBinding);
						auto  varableType = temp->Type();
					    if(!varableType)
							return;
						if (varableType->num_dimensions)
						{
							qualifiedBinding = lookupEnvironment->Object();
						}
						else if (lookupEnvironment->IsBaseType(varableType))
						{
							return;
						}
						else
						{
							qualifiedBinding = varableType;
						}

					}
						break;
					case Symbol::METHOD:
					{
						auto temp = reinterpret_cast<MethodSymbol*>(qualifiedBinding);
						auto  _Type = temp->Type();
						if (!_Type)
							return;
						if (_Type->num_dimensions)
						{
							qualifiedBinding = lookupEnvironment->Object();
						}
						else if (lookupEnvironment->IsBaseType(_Type))
						{
							return;
						}
						else
						{
							qualifiedBinding = _Type;
						}
					}
					break;
					default:
						return;
					}
					
				}
				else
					return;
			}
			else{
				qualifiedBinding = nullptr;	
			}
			if (!messageSend->arguments)
				return;

			auto num_arguments = messageSend->arguments->NumArguments();
			vector <TypeSymbol*> argTypes(num_arguments);
			for(auto i = 0 ; i < num_arguments; ++i)
			{
				auto expre = messageSend->arguments->Argument(i);
				if (expre && expre->symbol && expre->symbol->_kind == Symbol::TYPE)
					argTypes[i] =reinterpret_cast<TypeSymbol*>( expre->symbol );
			}

			auto identifier_token = messageSend->identifier_token;
			if (!identifier_token)
				return;

			wstring token;
			if(identifier_token->VirtualType() == Token::NORMAL)
			{
				
			}
			else
			{
				token = identifier_token->getName();
				if (identifier_token->start_location < actualCompletionPosition) {
					token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
				}
				requestor->SetTokenForMatch(token);
				token = EMPTY_TOKEN;
			}
			
			
			
			if (qualifiedBinding == nullptr) {
				insideQualifiedReference = false;
				findImplicitMessageSends(token, argTypes, scope, messageSend, scope);
			}
			else {
				insideQualifiedReference = true;
				vector< pair< MethodSymbol*, TypeSymbol*> > methodFound;
				bool exactMatch = true;
				bool isCompletingDeclaration = false;
				bool implicitCall = false;
				findMethods(
					token,
					argTypes,
					reinterpret_cast<TypeSymbol*>(qualifiedBinding),
					scope,
					methodFound,
					DO_NOT_CARE_STATIC,
					exactMatch,
					isCompletingDeclaration,
					messageSend,
					scope,
					implicitCall,
					NO_TYPE_SYMBOL);
			}
		}

		

		void CompletionEngine::ProcessCompletionOnExplicitConstructorCall(Ast* ast, AstNodeScope* scope)
		{
			
			if (!ast)
				return;

			if(ast->kind == Ast::THIS_CALL)
			{
	
				TypeSymbol* qualifiedBinding = ScopeUtil::enclosingSourceType(scope);
				
				if (!qualifiedBinding)
				{
					return;
				}
				auto constructorCall = reinterpret_cast<AstThisCall*>(ast);
			
				auto num_arguments = constructorCall->arguments->NumArguments();
				vector <TypeSymbol*> argTypes(num_arguments);
				for (auto i = 0; i < num_arguments; ++i)
				{
					auto expre = constructorCall->arguments->Argument(i);
					if (expre && expre->symbol &&expre->symbol->_kind == Symbol::TYPE)
						argTypes[i] = reinterpret_cast<TypeSymbol*>(expre->symbol);
				}
				bool forAnonymousType = false;
				keyWordWeightIncrease = true;
				findConstructors(
					qualifiedBinding,
					argTypes,
					scope,
					constructorCall,
					forAnonymousType,
					NO_EXCLUDE_CONSTRUCTOR);
			}
			else
			{
				Symbol* qualifiedBinding;
				auto type_scope = ScopeUtil::enclosingSourceType(scope);
				if (type_scope)
				{
					qualifiedBinding = type_scope->super;
					
				}
				else
				{
					qualifiedBinding  = nullptr;
				}
				if (!qualifiedBinding)
				{
					return;
				}
				auto constructorCall = reinterpret_cast<AstSuperCall*>(ast);
			
				auto num_arguments = constructorCall->arguments->NumArguments();
				vector <TypeSymbol*> argTypes(num_arguments);
				for (auto i = 0; i < num_arguments; ++i)
				{
					auto expre = constructorCall->arguments->Argument(i);
					if (expre && expre->symbol &&expre->symbol->_kind == Symbol::TYPE)
						argTypes[i] = reinterpret_cast<TypeSymbol*>(expre->symbol);
				}
				bool forAnonymousType = false;
				keyWordWeightIncrease = true;
				findConstructors(
					reinterpret_cast<TypeSymbol*>(qualifiedBinding),
					argTypes,
					scope,
					constructorCall,
					forAnonymousType,
					NO_EXCLUDE_CONSTRUCTOR);
			}
		
		}

		void CompletionEngine::ProcessCompletionOnExpression(Expression* expression, AstNodeScope* scope)
		{
			if (!expression)
				return;

			// this 的时候列出 this->x的情况和在构造函数中列出this(的情况
			insideQualifiedReference = true;
			auto qualifiedBinding = expression->symbol;
			if (!qualifiedBinding)
				return;
			
			if (qualifiedBinding->_kind == Symbol::VARIABLE) {
				auto receiverType = ((VariableSymbol*)qualifiedBinding)->Type();
				if (receiverType != nullptr) {
					bool implicitCall = false;
					vector<   pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
					vector<  VariableSymbol* > localsFound;
					keyWordWeightIncrease = true;
					findFieldsAndMethods(EMPTY_TOKEN,
						receiverType, 
						scope,
						&FakeInvocationSite,
						scope, 
						implicitCall,
						NO_TYPE_SYMBOL,
						fieldsFound,
						localsFound
					);
				}

			}
			else if (qualifiedBinding->_kind == Symbol::TYPE) {

				auto receiverType = (TypeSymbol*)qualifiedBinding;

				TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
				if (!enclosingSourceType) {
					return;
				}
				keyWordWeightIncrease = true;
				findMemberTypes(EMPTY_TOKEN, receiverType, scope, enclosingSourceType);

				findClassField(EMPTY_TOKEN, (TypeSymbol*)qualifiedBinding, scope);
				vector< pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
				vector<VariableSymbol*> localsFound;

				findFields(
					EMPTY_TOKEN,
					receiverType,
					scope,
					fieldsFound,
					localsFound,
					NO_STATIC,
					&FakeInvocationSite,
					scope,
					false, 
					NO_TYPE_SYMBOL,
					DO_NOT_CARE_CONST
				);
				vector<TypeSymbol*>  argTypes;
				vector< pair<MethodSymbol*, TypeSymbol*> > methodsFound;
				findMethods(
					EMPTY_TOKEN,
					argTypes,
					receiverType,
					scope,
					methodsFound,
					NO_STATIC,
					false,
					false,
					&FakeInvocationSite,
					scope,
					false,nullptr);

			}
		}

		void CompletionEngine::ProcessCompletionOnAstTypeName(AstTypeName* ast_node, AstNodeScope* scope)
		{
			ProcessCompletionOnTypeReference(ast_node, scope);
		}

		void CompletionEngine::ProcessCompletionOnClassCreationExpressionOnTypeName(AstClassCreationExpression* allocExpression, AstNodeScope* scope)
		{
			 if(!allocExpression || !allocExpression->arguments)
			 {
				 return;
			 }

			// 查找对应的类型 ，而且函数参数不能和当前的class create express  不能不匹配
			auto num_arguments = allocExpression->arguments->NumArguments();
			vector <TypeSymbol*> argTypes(num_arguments);
			for (auto i = 0; i < num_arguments; ++i)
			{
				auto expre = allocExpression->arguments->Argument(i);
				if (expre && expre->symbol &&expre->symbol->_kind == Symbol::TYPE)
					argTypes[i] = reinterpret_cast<TypeSymbol*>(expre->symbol);
			}
			if(allocExpression->anonymous_type_opt){
				return;
			}
			TypeSymbol* ref;
			if(!allocExpression->class_type)
			{
				return;
			}
			auto names = allocExpression->class_type->getTypeName();
			requestor->SetTokenForMatch(names[names.size() - 1]);


			names[names.size() - 1] = L".";
			auto qualifiedName = boost::join(names, L".");

			if (!transitRequstor)
				transitRequstor = new ResultCollector();

			auto temp_requestor = (ResultCollector*)requestor;
			requestor = transitRequstor;
			keyWordWeightIncrease = true;
			findTypes(qualifiedName);
			requestor = temp_requestor;

			for(auto it : transitRequstor->items)
			{
				if (it->iType == IJavaCompletionProposal::TypeProposal 
					|| it->iType == IJavaCompletionProposal::ClassProposal
					|| it->iType == IJavaCompletionProposal::InterfaceProposal )
				{
					auto typeProposal = reinterpret_cast<TypeCompletionProposal*>(it);
					string compoundTypeName;
					wstring wCompoundTypeName = typeProposal->packageName + L"." + typeProposal->typeName;
					compoundTypeName = CharOperation::ConvertUnicodeToUtf8(wCompoundTypeName);
						ref = lookupEnvironment->getType(compoundTypeName);
					if(!ref || ref == lookupEnvironment->no_type)
						continue;
					if (!ref->ACC_INTERFACE()) {
						if (!ref->ACC_ABSTRACT()) {
							findConstructors(
								ref,
								argTypes,
								scope,
								allocExpression,
								false,
								NO_EXCLUDE_CONSTRUCTOR);
						}
					}
					if (!ref->ACC_FINAL()) {
						findAnonymousType(
							ref,
							argTypes,
							scope,
							allocExpression);
					}
				}
			}
			transitRequstor->ClearAndDelete();
			
		}

		void CompletionEngine::ProcessCompletionOnMethodName(MethodDeclaration* method, AstNodeScope* scope)
		{

			TypeSymbol* qualifiedBinding = ScopeUtil::enclosingSourceType(scope);
			if (!qualifiedBinding)
			{
				return;
			}
			size_t length = qualifiedBinding->NumVariableSymbols();
			vector< wstring > excludeNames(length);
			for (size_t i = 0 ; i < length; ++ i)
			{
				excludeNames[i] =  qualifiedBinding->VariableSym(i)->getNameString();
			}
			
			auto token = method->selector();
			keyWordWeightIncrease = true;
			findVariableNames(token, method->type, excludeNames);

		}

		void CompletionEngine::ProcessCompletionOnMethodReturnType(MethodDeclaration* method,AstNodeScope* scope)
		{
			
			if (!method || !method->type)
				return;

			auto type = method->type;
	
			if (type->kind == Ast::TYPE) {
				ProcessCompletionOnTypeReference(reinterpret_cast<AstTypeName*>(type), scope);
			}
			else if (Ast::ARRAY == type->kind)
			{
				auto leftType = reinterpret_cast<AstArrayType*>(type)->type;
				if (leftType->kind == Ast::TYPE)
					ProcessCompletionOnTypeReference(reinterpret_cast<AstTypeName*>(leftType), scope);
			}

		}

		
		void  CompletionEngine::findMemberTypes(
			const wstring&  typeName,
			vector<TypeSymbol*>& memberTypes,
			vector<TypeSymbol*>& typesFound,
			TypeSymbol* receiverType,
			TypeSymbol* invocationType)
		{
			if (memberTypes.empty())return;

			// Inherited member types which are hidden by subclasses are filtered out
			// No visibility checks can be performed without the scope & invocationSite
			int typeLength = typeName.length();

			for (int m = memberTypes.size(); --m >= 0;) {
				auto memberType = memberTypes[m];
				if (!memberType) continue;
				//		if (!wantClasses && memberType.isClass()) continue next;
				//		if (!wantInterfaces && memberType.isInterface()) continue next;
				auto memberTypesourceName = memberType->sourceName('.');
				if (typeLength > memberTypesourceName.length())
					continue ;

				if (!CharOperation::prefixEquals(typeName, memberTypesourceName, false
					/* ignore case */
				))
					continue ;
				bool continue_next = false;
				if (options.checkVisibility
					&& !memberType->canBeSeenBy(receiverType, invocationType))
					continue ;

				
				if (assistNodeIsExtendedType &&  memberType->ACC_FINAL())
					continue;


				for (int i = typesFound.size(); --i >= 0;) {
					auto otherType = typesFound[i];

					if (!otherType)
						continue;

					if (memberType == otherType)
					{
						continue_next = true;
						break;
					}
					

					if (CharOperation::equals(memberTypesourceName, otherType->sourceName('.'), true)) {

						auto memberTypeEnclosingType= memberType->EnclosingType();
						auto otherTypeEnclosingType = otherType->EnclosingType();

						if(!memberTypeEnclosingType || !otherTypeEnclosingType)
						{
							continue;
						}

						if(otherTypeEnclosingType->IsSubclass(memberTypeEnclosingType))
						{
							continue_next = true;
							break;
						}
						

						if (otherTypeEnclosingType->ACC_INTERFACE()){
							if(memberTypeEnclosingType->Implements(otherTypeEnclosingType)){
								continue_next = true;
								break;
							}
						}
						if(memberTypeEnclosingType->ACC_INTERFACE()){
							if (otherTypeEnclosingType->Implements(memberTypeEnclosingType))
							{
								continue_next = true;
								break;
							}
						}	
					}
				}

			if (continue_next)
				continue;

			typesFound.push_back(memberType);

			double  relevance = computeBaseRelevance();
		
			relevance += computeRelevanceForClass();
			
			relevance += computeRelevanceForExpectingType(memberType);


			wstring compoundPackageName = memberType->ContainingPackageName();
			boost::replace_all(compoundPackageName, L"/", L".");
			if (!memberType->ACC_INTERFACE()) {
				//relevance += computeRelevanceForClass();

				requestor->acceptClass(
					compoundPackageName,
					memberType->qualifiedSourceName('.'),
					memberType->sourceName('.'),
					memberType->Flags(),
					completionRang,
					relevance);

			}
			else {
				//relevance += computeRelevanceForInterface();
				requestor->acceptInterface(
					compoundPackageName,
					memberType->qualifiedSourceName('.'),
					memberType->sourceName('.'),
					memberType->Flags(),
					completionRang,
					relevance);
			}
		}
		}
		void  CompletionEngine::findMemberTypesLoop(
			const wstring& typeName,
			TypeSymbol* receiverType,
			TypeSymbol* currentType,
			AstNodeScope* scope,
			TypeSymbol* typeInvocation,
			vector<TypeSymbol*>&  typesFound,
			
			set<TypeSymbol*>& visistedType)
		{
			if(visistedType.find(currentType) != visistedType.end()){
				return;
			}
			if (!currentType)
				return;

			visistedType.insert(currentType);
			if(currentType->nested_types)
			{
				findMemberTypes(
					typeName,
					*currentType->nested_types,
					typesFound,
					receiverType,
					typeInvocation);
			}
			else
			{
			
				auto num = currentType->NumTypeSymbols();
				if(num)
				{
					vector<TypeSymbol*> nested_types(num,nullptr);
					for (auto i = 0; i < num; ++i)
					{
						auto _type = currentType->TypeSym(i);
						if(_type && !_type->Bad())
							nested_types[i] = _type;
					}
					findMemberTypes(
						typeName,
						nested_types,
						typesFound,
						receiverType,
						typeInvocation);
				}

			}

			if(currentType->super)
			{
				findMemberTypesLoop(
					typeName,
					receiverType,
					currentType->super,
					scope,
					typeInvocation,
					typesFound,
					visistedType);
			}
			

			if(currentType->interfaces)
			{
				for(auto it : *(currentType->interfaces) )
				{
					findMemberTypesLoop(
						typeName,
						receiverType,
						it,
						scope,
						typeInvocation,
						typesFound,
						visistedType);
				}
			}

		}
		void  CompletionEngine::findMemberTypes(
			const wstring& typeName,
			TypeSymbol* receiverType,
			AstNodeScope* scope,
			TypeSymbol* typeInvocation)
		{
			auto currentType = receiverType;
			if (!currentType)
				return;

			vector<TypeSymbol*>  typesFound;
			if (insideQualifiedReference
				|| typeName.length() == 0) { // do not search up the hierarchy


				if (currentType->nested_types)
				{

					findMemberTypes(
						typeName,
						*currentType->nested_types,
						typesFound,
						receiverType,
						typeInvocation);
				}
				else
				{

					unsigned num = currentType->NumTypeSymbols();
					if (num)
					{
						vector<TypeSymbol*> nested_types(num, nullptr);
						for (unsigned i = 0; i < num; ++i)
						{
							auto _type = currentType->TypeSym(i);
							if (_type && !_type->Bad())
								nested_types[i] = _type;
						}
						findMemberTypes(
							typeName,
							nested_types,
							typesFound,
							receiverType,
							typeInvocation);
					}
				}
				return;
			}
			set<TypeSymbol*> visistedType;
			findMemberTypesLoop(typeName, receiverType, currentType, scope, typeInvocation, typesFound, visistedType);
		}

	 void CompletionEngine::findTypesAndSubpackages(
			const wstring& token,
			PackageSymbol* packageBinding)
		{
		 if (!packageBinding)
			 return;
			wstring  qualifiedName = packageBinding->PackageNameString();
			qualifiedName += L"." + token;
			boost::replace_all(qualifiedName, L"/", L".");
			
			findTypes(qualifiedName);
			findPackages(qualifiedName);
		}
		void CompletionEngine::ProcessCompletionOnTypeReference(AstTypeName* ast_node, AstNodeScope* scope)
		{
			if (!ast_node ||  !ast_node->name)
				return;

			auto identifier_token = ast_node->name->identifier_token;
			if(!identifier_token)
			{
				return;
			}
			auto	token = identifier_token->getName();
			if (ast_node->symbol && ast_node->symbol != lookupEnvironment->no_type)
			{
				if (tokenKind == TK_DOT)
				{
					insideQualifiedReference = true;
					Symbol*  qualifiedBinding = ast_node->symbol;
					token = EMPTY_TOKEN;
					if (qualifiedBinding->_kind == Symbol::TYPE)
					{	
						TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
						if (!enclosingSourceType) {
							return;
						}
						keyWordWeightIncrease = true;
						findMemberTypes(
							token,
							(TypeSymbol*)qualifiedBinding,
							scope,
							enclosingSourceType
						);
					}
					else if (qualifiedBinding->_kind == Symbol::PACKAGE) {
						keyWordWeightIncrease = true;
						findTypesAndSubpackages(token, (PackageSymbol*)qualifiedBinding);
					}

				}
				else if (ast_node->base_opt || ast_node->name->IsQualifiedName())
				{
					insideQualifiedReference = true;
					if (identifier_token->start_location <= actualCompletionPosition){
						if (tokenKind != TK_DOT){
							token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
						}
						else token = EMPTY_TOKEN;
					}
					requestor->SetTokenForMatch(token);

					token = EMPTY_TOKEN;
					insideQualifiedReference = true;
					Symbol*  qualifiedBinding = nullptr;
					if (ast_node->base_opt &&  ast_node->base_opt->symbol)
					{
						qualifiedBinding = ast_node->base_opt->symbol;
						if (qualifiedBinding == lookupEnvironment->no_type)
							return;
					}
					else
					{
						if (ast_node->name->base_opt && ast_node->name->base_opt->symbol)
						{
							qualifiedBinding = ast_node->name->base_opt->symbol;
						}
					}

					if (qualifiedBinding->_kind == Symbol::TYPE)
					{

						TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
						if (!enclosingSourceType) {
							return;
						}
						keyWordWeightIncrease = true;
						findMemberTypes(
							token,
							(TypeSymbol*)qualifiedBinding,
							scope,
							enclosingSourceType
						);
					}
					else if (qualifiedBinding->_kind == Symbol::PACKAGE) {
						keyWordWeightIncrease = true;
						findTypesAndSubpackages(token, (PackageSymbol*)qualifiedBinding);
					}

				}
			}
			else if(ast_node->base_opt || ast_node->name->IsQualifiedName())
			{
				insideQualifiedReference = true;
				if (identifier_token->start_location <= actualCompletionPosition) {
					if (tokenKind != TK_DOT) {
						token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
					}
					else token = EMPTY_TOKEN;
				}
				requestor->SetTokenForMatch(token);

				token = EMPTY_TOKEN;

				insideQualifiedReference = true;
				Symbol*  qualifiedBinding = nullptr;
				if (ast_node->base_opt &&  ast_node->base_opt->symbol)
				{
					qualifiedBinding = ast_node->base_opt->symbol;
					if(qualifiedBinding == lookupEnvironment->no_type)
						return;
				}
				else
				{
					if(ast_node->name->base_opt && ast_node->name->base_opt->symbol)
					{
						qualifiedBinding = ast_node->name->base_opt->symbol;
					}
				}
				if(!qualifiedBinding)
				{
					return;
				}
				if(qualifiedBinding->_kind == Symbol::TYPE)
				{
			
					TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
					if (!enclosingSourceType){
						return;
					}
					keyWordWeightIncrease = true;
					findMemberTypes(
						token,
						(TypeSymbol*)qualifiedBinding,
						scope,
						enclosingSourceType
					);
				}
				else if(qualifiedBinding->_kind == Symbol::PACKAGE){
					keyWordWeightIncrease = true;
					findTypesAndSubpackages(token, (PackageSymbol*)qualifiedBinding);
				}

			}
			else
			{
				insideQualifiedReference = false;
				if (identifier_token->start_location <= actualCompletionPosition) {
					if (tokenKind != TK_DOT) {
						token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
						requestor->SetTokenForMatch(token);
						token = token;
					}
					else 
						token.push_back('.');
				}	
				keyWordWeightIncrease = true;
				findTypesAndPackages(token, scope);
			}
		}

		void CompletionEngine::ProcessCompletionOnFieldName(AstVariableDeclarator* ast_node, AstNodeScope* scope)
		{
	
			if (!ast_node)
				return;
			TypeSymbol* qualifiedBinding = ScopeUtil::enclosingSourceType(scope);
			
			if (!qualifiedBinding)
			{
				return;
			}
			size_t length = qualifiedBinding->NumVariableSymbols();
			vector< wstring > excludeNames(length);
			for (size_t i = 0; i < length; ++i)
			{
				excludeNames[i] = qualifiedBinding->VariableSym(i)->getNameString();
			}
			wstring token;
			if (ast_node->variable_declarator_name &&ast_node->variable_declarator_name->identifier_token)
				token = ast_node->variable_declarator_name->identifier_token->getName();
			else
				return;

			if (!ast_node->owner)
				return;

			keyWordWeightIncrease = true;
			findVariableNames(token, reinterpret_cast<FieldDeclaration*>(ast_node->owner)->type , excludeNames);
		}

		void CompletionEngine::ProcessCompletionOnLocalFieldName(AstVariableDeclarator* ast_node, AstNodeScope* scope)
		{

			if(!scope || !ast_node){
				return;
			}

			vector<wstring> excludeNames;
			auto symbol = scope->GetSymbol();
			if(!symbol)
				return;
			if(symbol->_kind == Symbol::BLOCK)
			{
				BlockSymbol* block = reinterpret_cast<BlockSymbol*>(symbol);
				size_t length = block->NumVariableSymbols();
				excludeNames.assign(length, EMPTY_TOKEN);
				for (size_t i = 0; i < length; ++i)
				{
					excludeNames[i] = block->VariableSym(i)->getNameString();
				}
			}
			else if(symbol->_kind == Symbol::METHOD)
			{
				
			}
			
			wstring token;
			if (ast_node->variable_declarator_name && ast_node->variable_declarator_name->identifier_token)
				token = ast_node->variable_declarator_name->identifier_token->getName();
			else
				return;
			findVariableNames(token, ast_node->Type(), excludeNames);

		}



		void CompletionEngine::findVariablesAndMethods(
			const wstring& token,
			AstNodeScope* scope,
			InvocationSite* invocationSite,
			AstNodeScope* invocationScope, 
			FindVariableAndMethodOption _findOption,
			TypeSymbol* variableTypeOrMethodReturnType,
			vector<pair< MethodSymbol*, TypeSymbol*>  >& methodsFound,
			vector<pair< VariableSymbol*, TypeSymbol*>  >& fieldsFound,
			vector<VariableSymbol*>  localsFound,
			ConstAccess constAccess
		)
		{
			// Should local variables hide fields from the receiver type or any of its enclosing types?
			// we know its an implicit field/method access... see BlockScope getBinding/getImplicitMethod
			StaticAccess staticsOnly = DO_NOT_CARE_STATIC;
			// need to know if we're in a static context (or inside a constructor)
			int tokenLength = token.length();
			AstNodeScope* currentScope = scope;
		   if(_findOption != MethodOnly)
		   {
			   while (currentScope)
			   { // done when a COMPILATION_UNIT_SCOPE is found
				   bool break_done1 = false;
				   switch (currentScope->GetScopeKind()) {
				   case AstNodeScope::METHOD_SCOPE:
				   {
					   // handle the error case inside an explicit constructor call (see MethodScope>>findField)
					   auto  methodScope = (MethodSymbol*)currentScope->GetSymbol();
					   if (methodScope && methodScope->ACC_STATIC()) {
							staticsOnly = ONLY_STATIC;
					   }
					   if (constAccess == ConstAccess::ONLY_CONST || assistNodeIsInsideCase) {
						   break;
					   }
					   int length = methodScope->NumFormalParameters();
					   for (int i = 0; i < length; i++) {

						   auto local = methodScope->FormalParameter(i);
						   if (local == nullptr)
							   continue;

						   if (tokenLength > local->NameLength())
							   continue;

						   auto localName = local->getNameString();
						   if (!CharOperation::prefixEquals(token, localName, false /* ignore case */
						   ))
							   continue;

						   if (localName == RecoveryScanner::FAKE_IDENTIFIER)
							   continue;

						   bool continue_next = false;
						   for (auto otherLocal : localsFound) {
							   if (!otherLocal)
								   continue;
							   if (CharOperation::equals(otherLocal->getNameString(), localName, true))
								   continue_next = true;
							   break;
						   }
						   if (continue_next)
							   continue;
						   auto localType = local->Type();
						   if(variableTypeOrMethodReturnType 
							  && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(variableTypeOrMethodReturnType, localType)){
							   continue;
						   }

						   localsFound.push_back(local);

						   double  relevance = R_DEFAULT;
					
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
				   break;

				   case AstNodeScope::BLOCK_SCOPE:
				   {
					   auto blockScope = (BlockSymbol*)currentScope->GetSymbol();
					   if (!blockScope)
						   return;
					   int length = blockScope->NumVariableSymbols();
					   for (int i = 0; i < length; i++) {

						   auto local = blockScope->VariableSym(i);
						   if (local == nullptr)
							   continue;


						   if (constAccess == ConstAccess::ONLY_CONST || assistNodeIsInsideCase) {
							   if(!local->ACC_FINAL() || !local->initial_value)
								   continue;
						   }


						   if (tokenLength > local->NameLength())
							   continue;

						   auto localName = local->getNameString();
						   if (!CharOperation::prefixEquals(token, localName, false /* ignore case */ ))
							   continue;

						   bool continue_next = false;
						   for (auto otherLocal : localsFound) {

							   if (!otherLocal)
								   continue;

							   if (CharOperation::equals(otherLocal->getNameString(), localName, true))
								   continue_next = true;

							   break;
						   }

						   if (continue_next)
							   continue;

						   if (localName == RecoveryScanner::FAKE_IDENTIFIER)
							   continue;

						   localsFound.push_back(local);
						   auto localType = local->Type();
						   if (!localType)
							   continue;

						   if (variableTypeOrMethodReturnType
							   && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(variableTypeOrMethodReturnType, localType))
						   {
							   continue;
						   }
						   double  relevance = computeBaseRelevance();
					
						   relevance += computeRelevanceForExpectingType(local->Type());
						   relevance += computeRelevanceForFinal(assistNodeIsInsideCase, local->ACC_FINAL());
						   wstring typePackageName;
			
						   wstring typeName;
						   if (localType) {
							   typePackageName = localType->ContainingPackageName();
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

				   break;

				   case AstNodeScope::COMPILATION_UNIT_SCOPE: {
					   break_done1 = true;
					   break;
				   }

				   }
				   if (break_done1)
					   break;
				   currentScope = currentScope->parent_scope;
			   }

		   }
		   AstNodeScope* previousScope = nullptr;
		   currentScope = scope;
		   while (currentScope)
		   { // done when a COMPILATION_UNIT_SCOPE is found
			   switch (currentScope->GetScopeKind()) {
			   case AstNodeScope::CLASS_SCOPE:
			   {
				   auto classScope = currentScope;
				   auto  enclosingType = (TypeSymbol*)(currentScope->GetSymbol());
				   /*				if (tokenLength == 0) { // only search inside the type itself if no prefix was provided
				   findFields(token, enclosingType.fields(), classScope, fieldsFound, staticsOnly);
				   findMethods(token, enclosingType.methods(), classScope, methodsFound, staticsOnly, false);
				   break done;
				   } else { */
				   if(_findOption != MethodOnly)
				   {
					   findFields(
						   token,
						   enclosingType,
						   classScope,
						   fieldsFound,
						   localsFound,
						   staticsOnly,
						   invocationSite,
						   invocationScope,
						   true , 
						   variableTypeOrMethodReturnType,
						   constAccess);
				   }
				   if(_findOption != VarialeOnly)
				   {
					   vector<TypeSymbol*>  argTypes;
					   findMethods(
						   token,
						   argTypes,
						   enclosingType,
						   classScope,
						   methodsFound,
						   staticsOnly,
						   false,
						   false,
						   invocationSite,
						   invocationScope,
						   true,
						   variableTypeOrMethodReturnType);
				   }
				  
				   if (enclosingType && enclosingType->ACC_STATIC())
					   staticsOnly = ONLY_STATIC;
			   }
				   
				   break;

			   case AstNodeScope::COMPILATION_UNIT_SCOPE:
				
				   if(    variableTypeOrMethodReturnType 
					   && staticsOnly != ONLY_STATIC
					   && _findOption != MethodOnly 
					   && token.empty() )
				   {
					   auto thisType = ScopeUtil::enclosingSourceType(previousScope);
					   if (thisType  
						   && thisType != lookupEnvironment->no_type
						   && !lookupEnvironment->system_semantic->CanMethodInvocationConvert(variableTypeOrMethodReturnType, thisType))
					   {
						   return;
					   }

					   double  relevance = R_DEFAULT;
					
					   wstring typePackageName;
					   wstring typeName;
					  
						typePackageName = thisType->ContainingPackageName();
						boost::replace_all(typePackageName, L"/", L".");
						typeName = thisType->qualifiedSourceName('.');
					   

					   requestor->acceptVariableName(
						   typePackageName,
						   typeName,
						   L"this",
						   L"this",
						   completionRang,
						   relevance);

				   }
				   return;
			   }
			   previousScope = currentScope;
			   currentScope = currentScope->parent_scope;
		   }
		}


		void  CompletionEngine::findClassField(const wstring& token, TypeSymbol* receiverType, AstNodeScope* scope)
		{
		
			//if (token.length() <= classField.length()
			//	&& CharOperation::prefixEquals(token, classField, false /* ignore case */
			//	)) {
			//	double  relevance = R_DEFAULT;
			//	relevance += computeRelevanceForExpectingType(lookupEnvironment->Object());
			//	AccessFlags access;
			//	access.SetACC_PUBLIC();
			//	access.SetACC_STATIC();
			//	requestor->acceptField(
			//		NoChar,
			//		NoChar,
			//		classField,
			//		NoChar,
			//		NoChar,
			//		classField,
			//		access.Flags(),
			//		completionRang,
			//		relevance);
			//}
		}
	

		void CompletionEngine::ProcessCompletionOnNameReference(AstName* ast_name, AstNodeScope* scope)
		{
			if (!ast_name)
				return;

			if(tokenKind != TK_DOT){
				auto identifier_token = ast_name->identifier_token;
				if (!identifier_token)
					return;
				auto token = identifier_token->getName();
				if (identifier_token->start_location < actualCompletionPosition) {
					token = token.substr(0, actualCompletionPosition - identifier_token->start_location + 1);
				}
				requestor->SetTokenForMatch(token);
				auto ref = ast_name;
				if (!ast_name->IsQualifiedName())
				{
					vector<pair< MethodSymbol*, TypeSymbol*>  > methodsFound;
					vector<pair< VariableSymbol*, TypeSymbol*>  > fieldsFound;
					vector<VariableSymbol*>  localsFound;
					/*if(ast_name->symbol && ast_name->symbol->_kind == Symbol::VARIABLE)
					{
						auto tempVSymbol = reinterpret_cast<VariableSymbol*>(ast_name->symbol);
						if(tempVSymbol->IsLocal()){
							localsFound.push_back(tempVSymbol);
						}
						
					}*/
					keyWordWeightIncrease = true;
					findVariablesAndMethods(
						EMPTY_TOKEN,
						scope,
						ast_name,
						scope,
						VarialeAndMethodBoth,
						NO_TYPE_SYMBOL, 
						methodsFound, 
						fieldsFound, 
						localsFound,
						DO_NOT_CARE_CONST);
					// can be the start of a qualified type name
					findTypesAndPackages(EMPTY_TOKEN, scope);
				}
				else
				{
					insideQualifiedReference = true;
					if (!ast_name->base_opt)
						return;

					auto qualifiedBinding = ast_name->base_opt->symbol;
					if (!qualifiedBinding)
						return;
					if (qualifiedBinding->_kind == Symbol::VARIABLE) {


						auto receiverType = ((VariableSymbol*)qualifiedBinding)->Type();
						if (receiverType != nullptr) {
							bool implicitCall = false;
							vector<   pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
							vector<  VariableSymbol* > localsFound;
							keyWordWeightIncrease = true;
							findFieldsAndMethods(
								EMPTY_TOKEN, 
								receiverType,
								scope, 
								ref,
								scope,
								implicitCall, 
								NO_TYPE_SYMBOL,
								fieldsFound,
								localsFound);
						}

					}
					else if (qualifiedBinding->_kind == Symbol::TYPE) {

						auto receiverType = (TypeSymbol*)qualifiedBinding;

						TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);

						if (!enclosingSourceType){
							return;
						}
						keyWordWeightIncrease = true;
						findMemberTypes(EMPTY_TOKEN, receiverType, scope, enclosingSourceType);

						findClassField(EMPTY_TOKEN, (TypeSymbol*)qualifiedBinding, scope);
						vector< pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
						vector<VariableSymbol*> localsFound;
						bool implicitCall = false;
						findFields(
							EMPTY_TOKEN,
							receiverType,
							scope,
							fieldsFound,
							localsFound,
							ONLY_STATIC,
							ref,
							scope,
							implicitCall,
							NO_TYPE_SYMBOL,
							DO_NOT_CARE_CONST);

						vector<TypeSymbol*>  argTypes;
						vector< pair<MethodSymbol*, TypeSymbol*> > methodsFound;

						bool exactMatch = false;
						bool isCompletingDeclaration = false;
						
						findMethods(
							EMPTY_TOKEN,
							argTypes,
							receiverType,
							scope,
							methodsFound,
							ONLY_STATIC,
							exactMatch,
							isCompletingDeclaration,
							ref,
							scope,
							implicitCall,
							NO_TYPE_SYMBOL);

					}
					else if (qualifiedBinding->_kind == Symbol::PACKAGE) {
						keyWordWeightIncrease = true;
						//setSourceRange(astNode.sourceStart, (int)completionPosition);
						// replace to the end of the completion identifier
						findTypesAndSubpackages(EMPTY_TOKEN, (PackageSymbol*)qualifiedBinding);
					}
				}
			}
			else
			{
				insideQualifiedReference = true;
				auto qualifiedBinding = ast_name->symbol;
				if (!qualifiedBinding)
					return;
				keyWordWeightIncrease = true;
				auto ref = ast_name;
				if (qualifiedBinding->_kind == Symbol::VARIABLE) {
					auto receiverType = ((VariableSymbol*)qualifiedBinding)->Type();
					if (receiverType != nullptr) {
						vector<   pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
						vector<  VariableSymbol* > localsFound;
						findFieldsAndMethods(EMPTY_TOKEN, 
							receiverType, 
							scope,
							ref,
							scope, 
							false,
							NO_TYPE_SYMBOL,
							fieldsFound,
							localsFound
						);
					}

				}
				else if (qualifiedBinding->_kind == Symbol::TYPE) {

					auto receiverType = (TypeSymbol*)qualifiedBinding;

					TypeSymbol* enclosingSourceType = ScopeUtil::enclosingSourceType(scope);
					if (!enclosingSourceType){
						return;
					}
					findMemberTypes(EMPTY_TOKEN, receiverType, scope, enclosingSourceType);

					findClassField(EMPTY_TOKEN, (TypeSymbol*)qualifiedBinding, scope);
					vector< pair< VariableSymbol*, TypeSymbol*> > fieldsFound;
					vector<VariableSymbol*> localsFound;

					findFields(
						EMPTY_TOKEN,
						receiverType,
						scope,
						fieldsFound,
						localsFound,
						ONLY_STATIC,
						ref,
						scope,
						false, 
						NO_TYPE_SYMBOL,
						DO_NOT_CARE_CONST);
					vector<TypeSymbol*>  argTypes;
					vector< pair<MethodSymbol*, TypeSymbol*> > methodsFound;
					findMethods(
						EMPTY_TOKEN,
						argTypes,
						receiverType,
						scope,
						methodsFound,
						ONLY_STATIC,
						false,
						false,
						ref,
						scope,
						false,
						NO_TYPE_SYMBOL);

				}
				else if (qualifiedBinding->_kind == Symbol::PACKAGE) {

					//setSourceRange(astNode.sourceStart, (int)completionPosition);
					// replace to the end of the completion identifier
					findTypesAndSubpackages(EMPTY_TOKEN, (PackageSymbol*)qualifiedBinding);
				}

			}
			
		}
	
		
	
} // Close namespace Jikes block

} // Close namespace CodeAssist block

