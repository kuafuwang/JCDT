#include "stdafx.h"
#include "ConstructorDeclarationPattern.h"
#include <JCDT_Lib/internal/ast/AstConstructorDeclaration.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/classfmt/class.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/algorithm/string.hpp>
#include  <JCDT_Lib/internal/codegen/Signature.h>
namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	ConstructorDeclarationPattern::ConstructorDeclarationPattern(
		std::wstring declaringSimpleName, 
		int _matchMode, bool _isCaseSensitive, 
		std::wstring declaringQualification,
		std::vector<std::wstring>* parameterQualifications,
		std::vector<std::wstring> *parameterSimpleNames)
		:MethodDeclarationPattern(
			L"",
			_matchMode, 
			_isCaseSensitive, 
			declaringQualification, 
			declaringSimpleName,
			L"",
			L"",
			nullptr,
			nullptr)
	{
	}

	bool ConstructorDeclarationPattern::matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, 
		std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::METHOD_INFO )
			|| !class_data.first 
			|| !binaryInfo.first)
			return false;

		auto method = (MethodInfo*)binaryInfo.first;

		// must be a constructor
		const ConstantPool& pool = *(class_data.first);
		auto str = method->Name(pool);

		string selector;
		if(str)
			selector.assign(str, str + method->NameLength(pool));

		if(selector != "<init>")
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

		// parameter types
		int parameterCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
		if (parameterCount > -1) {
			str = method->Signature(pool, nullptr);
			if (!str)
				return false;

			string sig(str,
				str + method->SignatureLength(pool, nullptr));
			auto  _ret = Signature::parseSignature(sig);
			auto & formal_parameters = _ret.first;
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

	int ConstructorDeclarationPattern::matchLevel(Ast* node, bool resolve)
	{
		if (!node ||  node->kind != CONSTRUCTOR) {
			return IMPOSSIBLE_MATCH;
		}
		

		ConstructorDeclaration* constructor = (ConstructorDeclaration*)node;

		if (resolve) {
			return matchLevel(constructor->method_symbol);
		}
		else {
			// constructor name is stored in selector field
			auto  declarator  =constructor->method_declarator;

			if (!declarator || !declarator->identifier_token)
				return IMPOSSIBLE_MATCH;

			if (	declaringSimpleName.empty() != true
				&& ! matchesName(declaringSimpleName, declarator->identifier_token->getName())
				)
				return IMPOSSIBLE_MATCH;

			// parameter types
			int parameterCount = parameterSimpleNames==nullptr ? -1: parameterSimpleNames->size();
			if (parameterCount > -1) {
				int argumentCount = declarator->NumFormalParameters();
				if (parameterCount != argumentCount)
					return IMPOSSIBLE_MATCH;
			}

			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int ConstructorDeclarationPattern::matchLevel(Symbol* binding)
	{
		if (!binding) 
			return INACCURATE_MATCH;

		if (binding->_kind != Symbol::METHOD)
			return IMPOSSIBLE_MATCH;

		int level;

		MethodSymbol* constructor = reinterpret_cast<MethodSymbol*>(binding);

		// must be a constructor
		if (!constructor->IsConstructor())
			return IMPOSSIBLE_MATCH;

		// declaring type
		auto declaringType = constructor->containing_type;

		if(!declaringType)
			return IMPOSSIBLE_MATCH;

		if (!constructor->ACC_STATIC() && !constructor->ACC_PRIVATE()) {
			level = matchLevelAsSubtype(declaringType, declaringSimpleName, declaringQualification);
		}
		else {
			level = matchLevelForType(declaringSimpleName, declaringQualification, declaringType);
		}
		if (level == IMPOSSIBLE_MATCH) {
			return IMPOSSIBLE_MATCH;
		}

		// parameter types
		int parameterCount = parameterSimpleNames== nullptr? -1 : parameterSimpleNames->size();
		if (parameterCount >= -1) {
			int argumentCount = constructor->NumFormalParameters();
			if (parameterCount != argumentCount)
				return IMPOSSIBLE_MATCH;

			for (int i = 0; i < parameterCount; i++) {
				auto& qualification = parameterQualifications->operator[](i);
				auto& type = parameterSimpleNames->operator[](i);
				auto parameter = constructor->FormalParameter(i);
				if (!parameter)
					continue;

				int newLevel = matchLevelForType(type, qualification, parameter->Type());
				switch (newLevel)
				{
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
}


} // Close namespace Jikes block


