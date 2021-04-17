#include "stdafx.h"
#include "MethodReferencePattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/ast/AstMethodInvocation.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <SearchLib/internal/matching/MatchSet.h>
#include <SearchLib/internal/matching/SuperTypeNamesCollector.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	MethodReferencePattern::MethodReferencePattern(
		std::wstring _selector,
		int _matchMode,
		bool _isCaseSensitive, 
		std::wstring _declaringQualification,
		std::wstring _declaringSimpleName, 
		std::wstring _returnQualification, 
		std::wstring _returnSimpleName, 
		std::vector<std::wstring>* _parameterQualifications,
		std::vector<std::wstring>* _parameterSimpleNames,
		PathModel::MemberInFileMoses* _declaringType):MethodPattern(_matchMode, _isCaseSensitive), declaringType(_declaringType), allSuperDeclaringTypeNames(nullptr)
	{

		selector.swap(_selector);
		declaringQualification.swap(_declaringQualification);
		declaringSimpleName.swap(_declaringSimpleName);
		returnQualification.swap(_returnQualification);
		returnSimpleName.swap(_returnSimpleName);
		parameterQualifications = _parameterQualifications;
		parameterSimpleNames = _parameterSimpleNames;
		
		if(!isCaseSensitive)
		{
			boost::to_lower(selector);
			boost::to_lower(declaringQualification);
			boost::to_lower(declaringSimpleName);
			boost::to_lower(returnQualification);
			boost::to_lower(returnSimpleName);
			if(parameterQualifications)
			{
				auto data_parameterQualifications = parameterQualifications->data();
				auto size = parameterQualifications->size();
				for (auto i = 0; i < size; ++i)
				{
					boost::to_lower(data_parameterQualifications[i]);
				}
				auto data_parameterSimpleNames = parameterSimpleNames->data();
				size = parameterSimpleNames->size();
				for (auto i = 0; i < size; ++i)
				{
					boost::to_lower(data_parameterSimpleNames[i]);
				}
			}
			

		}
	
		
		_needsResolve = needsResolve();
	}

	void MethodReferencePattern::initializePolymorphicSearch(MatchSet* locator,
		IProgressMonitor* progressMonitor)
	{
		
		/*	auto collector = new SuperTypeNamesCollector(
				this,
				locator,
				declaringType,
				progressMonitor);

			allSuperDeclaringTypeNames =  collector->collect();
			delete collector;
*/
			
	}

	bool MethodReferencePattern::isVirtualInvoke(MethodSymbol* method, AstMethodInvocation* messageSend)
	{

		return method &&  messageSend && 
			!method->ACC_STATIC() 
		   && !messageSend->isSuperAccess()
		   && !method->ACC_PRIVATE();
	}

	int MethodReferencePattern::matchLevel(Ast* node, bool resolve)
	{
		if(!node)
			return IMPOSSIBLE_MATCH;

		if ( node->kind  != Ast::CALL )
			return IMPOSSIBLE_MATCH;

		auto messageSend = (AstMethodInvocation*)node;

		if (resolve) {
			return matchLevel(messageSend->symbol, messageSend);
		}
		else {
			// selector
			if(!messageSend->identifier_token)	
				return IMPOSSIBLE_MATCH;

			auto _the_selector = messageSend->identifier_token->getName();
			if ( !matchesName(selector, _the_selector))
				return IMPOSSIBLE_MATCH;

			// argument types
			int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
			if (argumentCount > -1) {
				int parameterCount = messageSend->arguments == nullptr ? 0 : messageSend->arguments->NumArguments();
				if (parameterCount != argumentCount)
					return IMPOSSIBLE_MATCH;
			}

			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int MethodReferencePattern::matchLevel(Symbol* binding, AstMethodInvocation* messageSend)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;
		if (binding->_kind != Symbol::METHOD) 
			return IMPOSSIBLE_MATCH;
		int level;

		auto method = reinterpret_cast<MethodSymbol*>(binding);

		// selector
		if (selector.empty() != true && !matchesName(selector, method->getNameString()))
			return IMPOSSIBLE_MATCH;

		// receiver type
		TypeSymbol* receiverType =
			binding == nullptr ?
				nullptr :
				method->containing_type;
		 
		auto _receiverType = messageSend->receiverType();

		if (isVirtualInvoke(method, messageSend) && _receiverType && !_receiverType->IsArray()) {
			level = matchLevelAsSubtype(receiverType, declaringSimpleName, declaringQualification);
			if (level == IMPOSSIBLE_MATCH) {
				// get allSuperDeclaringTypeNames;
				if(!allSuperDeclaringTypeNames){
					allSuperDeclaringTypeNames = new std::vector < std::wstring  >();
					auto collector = new SuperTypeNamesCollector(this);
					collector->collect();
					allSuperDeclaringTypeNames->assign(collector->allSuperDeclaringTypeNames.begin(),
						collector->allSuperDeclaringTypeNames.end());
					delete collector;
				}
				level = matchLevelForType(*allSuperDeclaringTypeNames, receiverType);
				if (level == IMPOSSIBLE_MATCH) {
					return IMPOSSIBLE_MATCH;
				}
			}		
		}
		else {
			level = matchLevelForType(declaringSimpleName, declaringQualification, receiverType);
			if (level == IMPOSSIBLE_MATCH) return IMPOSSIBLE_MATCH;
		}

		// look at return type only if declaring type is not specified
		if (declaringSimpleName.empty() == true) {
			int newLevel = matchLevelForType(returnSimpleName, returnQualification, method->Type());
			switch (newLevel) {
			case IMPOSSIBLE_MATCH:
				return IMPOSSIBLE_MATCH;
			case ACCURATE_MATCH: // keep previous level
				break;
			default: // ie. INACCURATE_MATCH
				level = newLevel;
				break;
			}
		}

		// argument types
		int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
		if (argumentCount > -1) 
		{
				int parameterCount = method->NumFormalParameters();

				if (parameterCount != argumentCount) 
					return IMPOSSIBLE_MATCH;
				for (int i = 0; i < parameterCount; i++) {
					auto& qualification = parameterQualifications->operator[](i);
					auto& type = parameterSimpleNames->operator[](i);
					auto parameter = method->FormalParameter(i);
					if (!parameter)
						continue;
					int newLevel = matchLevelForType(type, qualification, parameter->Type());
					switch (newLevel) {
					case IMPOSSIBLE_MATCH:
						return IMPOSSIBLE_MATCH;
					case ACCURATE_MATCH: // keep previous level
						break;
					default: // ie. INACCURATE_MATCH
						level = newLevel;
						break;
					}
				}
		 }

		return level;
	}

	void MethodReferencePattern::matchReportReference(Ast* reference, JavaModel::IJavaElement* element, 
		int accuracy, MatchSet* locator)
	{
		if (!reference)
			return;

		if( reference->kind == Ast::CALL ){
			auto invocate =  reinterpret_cast<AstMethodInvocation*>(reference);
			if (!invocate->identifier_token)
				return;
			// message ref are starting at the selector start
			locator->report(
				invocate->identifier_token->start_location,
				invocate->identifier_token->EndLocation(),
				element,
				accuracy,0);
		}
		else {
			MethodPattern::matchReportReference(reference, element, accuracy, locator);
		}

	}

	
}


} // Close namespace Jikes block


