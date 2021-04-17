#include "stdafx.h"
#include "FieldDeclarationPattern.h"
#include <JCDT_Lib/internal/ast/ASTNode.h>
#include <JCDT_Lib/internal/ast/AstVariableDeclarator.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/classfmt/FieldInfo.inl>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <JCDT_Lib/internal/classfmt/CPClassInfo.inl>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	FieldDeclarationPattern::FieldDeclarationPattern(
		std::wstring _name,
		int _matchMode, 
		bool _isCaseSensitive, 
		std::wstring _declaringQualification, 
		std::wstring _declaringSimpleName, 
		std::wstring _typeQualification,
		std::wstring _typeSimpleName):
		SearchPattern(_matchMode, _isCaseSensitive), isLocal(false)
	{

		name.swap(_name);
		declaringQualification.swap(_declaringQualification);
		typeQualification.swap(_typeQualification);
		typeSimpleName.swap(_typeSimpleName);
		declaringSimpleName.swap(_declaringSimpleName);
		

		_needsResolve = needsResolve();
	}

	bool FieldDeclarationPattern::matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
		std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::FIELD_INFO) || !class_data.first)
			return false;

		const auto field = (FieldInfo*)binaryInfo.first;

		auto& pool = *(class_data.first);

		// field name
	
		wstring name_symbol = CharOperation::ConvertUtf8ToUnicode(field->Name(pool));


		if (!matchesName(name, name_symbol))
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
	
		// field type
		int idx = 0;
	    auto fieldTypeSignature = Signature::parseType(field->Signature(pool, nullptr), idx);
		if (!matchesType(typeSimpleName, typeQualification, fieldTypeSignature))
		{
			return false;
		}

		return true;
	}

	int FieldDeclarationPattern::matchLevel(Ast* node, bool resolve)
	{
		
		if (!node ||  !(node->kind == Ast::VARIABLE_DECLARATOR))
			return IMPOSSIBLE_MATCH;

		
		
		AstVariableDeclarator* field = (AstVariableDeclarator*)node;

		if (isLocal){
			if (field->isField()){
				return IMPOSSIBLE_MATCH;
			}
		}

		if (resolve) {
			return matchLevel(field->symbol);
		}
		else {
		
			// field name
			if (   field->variable_declarator_name 
				&& field->variable_declarator_name->identifier_token
				&& !matchesName(name, field->variable_declarator_name->identifier_token->getName()))
				return IMPOSSIBLE_MATCH;

			// field type
			if (!field->owner)
				return IMPOSSIBLE_MATCH;

			AstType*fieldType = reinterpret_cast<FieldDeclaration*>(field->owner)->type;
			
			
			auto fieldTypeName = fieldType->getTypeName();
			auto sourceName = toArrayName(
				fieldTypeName[fieldTypeName.size() - 1],
				fieldType->dimensions());

			if (!matchesName(typeSimpleName, sourceName))
				return IMPOSSIBLE_MATCH;

			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int FieldDeclarationPattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;

		if (binding->_kind != Symbol::VARIABLE) 
			return IMPOSSIBLE_MATCH;

		int level;

		VariableSymbol* field = reinterpret_cast<VariableSymbol*>(binding);

		// field name
		if (!matchesName(name, field->getNameString()))
			return IMPOSSIBLE_MATCH;

		// declaring type
		auto declaringBinding = field->ContainingType();
		if (declaringBinding == nullptr) {
			return INACCURATE_MATCH;
		}
		else {
			level = matchLevelForType(declaringSimpleName, declaringQualification, declaringBinding);
			if (level == IMPOSSIBLE_MATCH) return IMPOSSIBLE_MATCH;
		}

		// look at field type only if declaring type is not specified
		if (declaringSimpleName.empty() == true) {
			int newLevel = matchLevelForType(typeSimpleName, typeQualification, field->Type());
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

		return level;
	}
	bool FieldDeclarationPattern::matchesBinaryPackge(const wstring& compoundPackageName)
	{
		if (_needsResolve) {
			if (declaringQualification.find(compoundPackageName) != 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}
	bool FieldDeclarationPattern::matchesBinaryClass(const std::wstring& mainTypeName) {
		Jikes::wstringex mainTypeNameEx(mainTypeName);
		if (mainTypeNameEx.end_with(declaringSimpleName)) {
			return true;
		}
		else {
			return false;
		}
	}
}


} // Close namespace Jikes block


