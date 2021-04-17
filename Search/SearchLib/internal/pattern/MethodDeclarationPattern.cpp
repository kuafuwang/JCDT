#include "stdafx.h"
#include "MethodDeclarationPattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/ast/AstMethodDeclaration.h>
#include <JCDT_Lib/internal/ast/AstMethodDeclarator.h>
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <JCDT_Lib/internal/classfmt/CPClassInfo.inl>
#include <JCDT_Lib/internal/classfmt/MethodInfo.inl>

namespace Jikes { // Open namespace Jikes block

namespace Search{
	MethodDeclarationPattern::MethodDeclarationPattern(
		std::wstring _selector, 
		int _matchMode,
		bool _isCaseSensitive, 
		std::wstring _declaringQualification,
		std::wstring _declaringSimpleName,
		std::wstring _returnQualification,
		std::wstring _returnSimpleName,
		std::vector<std::wstring>* _parameterQualifications, 
		std::vector<std::wstring>* _parameterSimpleNames):MethodPattern(_matchMode, _isCaseSensitive)
	{
		selector.swap(_selector);
		declaringQualification.swap(_declaringQualification);
		declaringSimpleName.swap(_declaringSimpleName);
		returnQualification.swap(_returnQualification);
		returnSimpleName.swap(_returnSimpleName);
		parameterSimpleNames = _parameterSimpleNames;
		parameterQualifications = _parameterQualifications;
		if(!isCaseSensitive)
		{
			boost::to_lower(selector);
			boost::to_lower(declaringQualification);
			boost::to_lower(declaringSimpleName);
			boost::to_lower(returnQualification);
			boost::to_lower(returnSimpleName);
			if(parameterSimpleNames)
			{
				auto data = parameterSimpleNames->data();
				auto data_parameterQualifications = parameterQualifications->data();
				for (size_t i = 0, size = parameterSimpleNames->size(); i < size; ++i) {
					boost::to_lower(data[i]);
					boost::to_lower(data_parameterQualifications[i]);

				}
			}		
		}	
		_needsResolve = needsResolve();
	}

	bool MethodDeclarationPattern::matchesBinary(
		std::pair<void*, BinaryInfoType>&  binaryInfo, std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::METHOD_INFO))
			return false;

		auto method = (MethodInfo*)binaryInfo.first;
		if (!method || !class_data.first)
			return false;
		// selector

		const ConstantPool& pool = *(class_data.first);
	
		auto w_selector = CharOperation::ConvertUtf8ToUnicode(method->Name(pool));
		if (!matchesName(selector, w_selector))
			return false;

		// declaring type
		CPClassInfo* class_info = class_data.second;

		if (class_info != nullptr) {

			wstring declaringTypeName = CharOperation::ConvertUtf8ToUnicode(class_info->TypeName(pool));;
			boost::replace_all(declaringTypeName, L"/", L".");
			if (!matchesType(declaringSimpleName, declaringQualification, declaringTypeName)) {
				return false;
			}
		}

	
		auto  _ret = Signature::parseSignature(method->Signature(pool, nullptr));
		auto & formal_parameters = _ret.first;
		auto & returnTypeSignature = _ret.second;
		// look at return type only if declaring type is not specified
		if (declaringSimpleName.empty() == true) {
			if (!matchesType(returnSimpleName, returnQualification, returnTypeSignature)) {
				return false;
			}
		}

		// parameter types
		int parameterCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
		if (parameterCount > -1) {
			int argumentCount = formal_parameters.size();
			if (parameterCount != argumentCount)
				return false;
			for (int i = 0; i < parameterCount; i++) {

				auto& qualification = parameterQualifications->operator[](i);
				auto& type = parameterSimpleNames->operator[](i);
				if (!matchesType(type, qualification, formal_parameters[i]))
					return false;
			}
		}


		return true;
	}

	int MethodDeclarationPattern::matchLevel(Ast* node, bool resolve)
	{
		if (!node)
			return IMPOSSIBLE_MATCH;

		if (node->kind != Ast::METHOD)
			return IMPOSSIBLE_MATCH;

		MethodDeclaration *method = reinterpret_cast<MethodDeclaration*>(node);

		if (resolve) {
			return matchLevel(method->method_symbol);
		}
		else {
			// selector
			if(method->method_declarator|| method->method_declarator->identifier_token )
				return IMPOSSIBLE_MATCH;

			if (! matchesName(selector, method->method_declarator->identifier_token->getName()))
				return IMPOSSIBLE_MATCH;

			// return type
			auto methodReturnType = method->type;
			if (methodReturnType != nullptr) {

				auto methodReturnTypeName = methodReturnType->getTypeName();
				auto sourceName = toArrayName(
					methodReturnTypeName[methodReturnTypeName.size() - 1],
					methodReturnType->dimensions());
				if (!matchesName(returnSimpleName, sourceName))
					return IMPOSSIBLE_MATCH;
			}

			// parameter types
			int parameterCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
			if (parameterCount > -1) {
				int argumentCount = method->method_declarator->NumFormalParameters();
				if (parameterCount != argumentCount)
					return IMPOSSIBLE_MATCH;
			}

			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int MethodDeclarationPattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;
		if (!(binding->_kind == Symbol::METHOD )) 
			return IMPOSSIBLE_MATCH;

		int level;

		MethodSymbol* method = (MethodSymbol*)binding;

		// selector
		if (!matchesName(selector, method->getNameString()))
			return IMPOSSIBLE_MATCH;

		// declaring type
		auto declaringType = method->containing_type;
		if (!method->ACC_STATIC() && !method->ACC_PRIVATE()) {
			level = matchLevelAsSubtype(declaringType, declaringSimpleName, declaringQualification);
		}
		else {
			level = matchLevelForType(declaringSimpleName, declaringQualification, declaringType);
		}
		if (level == IMPOSSIBLE_MATCH) {
			return IMPOSSIBLE_MATCH;
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

		// parameter types
		int parameterCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
		if (parameterCount > -1) {
		
			int argumentCount = method->NumFormalParameters();
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

	bool MethodDeclarationPattern::matchesBinaryPackge(const wstring& compoundPackageName)
	{
		if(_needsResolve){
			if(declaringQualification.find(compoundPackageName) != 0){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return true;
		}
	}

	bool MethodDeclarationPattern::matchesBinaryClass(const std::wstring& mainTypeName){
		Jikes::wstringex mainTypeNameEx(mainTypeName);
		if(mainTypeNameEx.end_with(declaringSimpleName)){
			return true;
		}
		else{
			return false;
		}
	}
}


} // Close namespace Jikes block


