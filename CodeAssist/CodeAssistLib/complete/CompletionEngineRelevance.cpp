#include "stdafx.h"
#include "CompletionEngine.h"
#include <CodeAssistLib/impl/RelevanceConstants.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/util/spell.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/algorithm/string.hpp>
using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

namespace CodeAssist{ 

	int CompletionEngine::computeRelevanceForExpectingType(wstring packageName, const wstring& typeName)
	{
		if (expectedTypes.size() != true)
		{
			boost::replace_all(packageName, L"/", L".");
			for (int i = 0; i < expectedTypes.size(); i++)
			{
				if (expectedTypes[i]->ContainingPackageName() == packageName &&
					expectedTypes[i]->qualifiedSourceName('.') == typeName)
				{
					return R_EXPECTED_TYPE;
				}
			}
		}
		return 0;
	}



	int CompletionEngine::computeRelevanceForExpectingType(TypeSymbol* proposalType) {
		if (expectedTypes.empty() != true && proposalType != nullptr) {
			for (int i = 0; i < expectedTypes.size(); i++) {

				if (lookupEnvironment->system_semantic->CanMethodInvocationConvert(proposalType, expectedTypes[i])) {
					return R_EXPECTED_TYPE;
				}

			}
		}
		return 0;
	}

	double  CompletionEngine::computeRelevanceForCaseMatching(const wstring& token, const wstring& proposalName) 
	{

	 if (CharOperation::prefixEquals(token, proposalName, true /* do not ignore case */)) {
			if (CharOperation::equals(token, proposalName, true /* do not ignore case */))
			{
				return R_CASE + R_EXACT_NAME;
			}
		
			auto leftData = token.c_str();
			auto testToken = proposalName.c_str();
				
			double rate = Spell::IsLeftStringOrderInRightString(leftData, testToken);
			if (rate) {
				int value = (RelevanceConstants::R_CASE * rate);
				return  value > 0 ? value : 1;
			}
			
	}
	 else if (CharOperation::prefixEquals(token, proposalName, false /* ignore case */)) 
	 {
		 if (CharOperation::equals(token, proposalName, false /* ignore case */)) {
			 return R_CASE;
		 }
		 
		 auto leftData = token.c_str();
		 auto testToken = proposalName.c_str();
		 double rate = Spell::IsLeftStringOrderInRightString(leftData, testToken);
		 if (rate) {
		
			 int value = (RelevanceConstants::R_CASE * rate);
			 return  value > 0 ? value : 1;

		 }
	 }
	 else
	 {
		 auto leftData = token.c_str();
		 auto testToken = proposalName.c_str();
		 int  index = Spell::Index(leftData, testToken);
		 if (index > 7) {
			 return (RelevanceConstants::R_EXACT_NAME * index / 10.0);

		 }
		
		 double rate = Spell::IsLeftStringOrderInRightString(leftData, testToken);
		 if (rate) {
			 int value = (RelevanceConstants::R_EXACT_NAME * rate);
			 return  value  >  0 ? value : 1 ;

		 }
		 rate = Spell::levenshtein(leftData, testToken) / (proposalName.size()*1.0);
		 if (rate < 0.3) {
			 return  (RelevanceConstants::R_EXACT_NAME * (1 - rate));

		 }
	 }

	return 0;
	}

	 int  CompletionEngine::computeRelevanceForClass() {
		if (assistNodeIsClass) {
			return R_CLASS;
		}
		return 0;
	}
	 double  CompletionEngine::computeBaseRelevance() {
		 return R_DEFAULT;
	 }
	 int CompletionEngine::computeBaseRelevancePackageRelevance()
	{
		if (hasImportWord){
			return R_PACKAGE;
		}
		else return  0;
		
	}
	 int CompletionEngine::computeRelevanceForResolution() {
		 return computeRelevanceForResolution(true);
	 }
	 int CompletionEngine::computeRelevanceForResolution(bool isResolved) {
		 if (isResolved) {
			 return R_RESOLVED;
		 }
		 return 0;
	 }
	 int CompletionEngine::computeRelevanceForRestrictions(int accessRuleKind) {
		 if (accessRuleKind == IAccessRule::K_ACCESSIBLE) {
			 return R_NON_RESTRICTED;
		 }
		 return 0;
	 }

	 int  CompletionEngine::computeRelevanceForEnum() {
		if (assistNodeIsEnum) {
			return R_ENUM;
		}
		return 0;
	}
	  int CompletionEngine::computeRelevanceForFinal(bool onlyFinal, bool isFinal) {
		 if (onlyFinal && isFinal) {
			 return R_FINAL;
		 }
		 return 0;
	 }
	 int  CompletionEngine::computeRelevanceForEnumConstant(TypeSymbol* proposalType) {
		if (assistNodeIsEnum &&
			proposalType != nullptr) {
			for (int i = 0; i <= expectedTypes.size(); i++) {
				if (proposalType->ACC_ENUM() &&
					proposalType == expectedTypes[i]) {
					return R_ENUM + R_ENUM_CONSTANT;
				}

			}
		}
		return 0;
	}

	 int  CompletionEngine::omputeRelevanceForException() {
		if (assistNodeIsException) {
			return R_EXCEPTION;
		}
		return 0;
	}
	 int  CompletionEngine::computeRelevanceForInterface() {
		if (assistNodeIsInterface) {
			return R_INTERFACE;
		}
		return 0;
	}

	int CompletionEngine::computeRelevanceForQualification(bool prefixRequired) {
		if (!prefixRequired && !insideQualifiedReference) {
			return R_UNQUALIFIED;
		}

		if (prefixRequired && insideQualifiedReference) {
			return R_QUALIFIED;
		}
		return 0;
	}

	 int CompletionEngine::computeRelevanceForStatic(bool onlyStatic, bool isStatic) {
		if (insideQualifiedReference && !onlyStatic && !isStatic) {
			return R_NON_STATIC;
		}
		return 0;
	}
	 int CompletionEngine::computeRelevanceForInheritance(TypeSymbol* receiverType, TypeSymbol* declaringClass) {
		 if (receiverType == declaringClass) return R_NON_INHERITED;
		 return 0;
	 }

} // Close namespace Jikes block

} // Close namespace CodeAssist block

