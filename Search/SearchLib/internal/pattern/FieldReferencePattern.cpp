#include "stdafx.h"
#include "FieldReferencePattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <SearchLib/internal/matching/MatchSet.h>
#include <SearchLib/internal/matching/SearchResultNode.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	FieldReferencePattern::FieldReferencePattern(
		 std::wstring _name,
		int _matchMode,
		bool _isCaseSensitive,
		std::wstring _declaringQualification,
		std::wstring _declaringSimpleName,
		std::wstring _typeQualification,
		std::wstring _typeSimpleName,
		bool _readAccess,
		bool _writeAccess) :
		MultipleSearchPattern(_matchMode, _isCaseSensitive),
		readAccess(_readAccess), writeAccess(_writeAccess)
	{
		name.swap(_name);
		declaringQualification.swap(_declaringQualification);
		declaringSimpleName.swap(_declaringSimpleName);
		typeQualification.swap(_typeQualification);
		typeSimpleName.swap(_typeSimpleName);
		if(!isCaseSensitive)
		{
			boost::to_lower(name);
			boost::to_lower(declaringQualification);
			boost::to_lower(declaringSimpleName);
			boost::to_lower(typeQualification);
			boost::to_lower(typeSimpleName);
		}
		
		_needsResolve = needsResolve();
	}


	int FieldReferencePattern::matchContainer()
	{
		int _matchContainer = METHOD | FIELD;
		if (writeAccess && !readAccess) {
			_matchContainer |= CLASS;
		}
		return _matchContainer;
	}

	

	int FieldReferencePattern::matchLevel(Ast* node, bool resolve)
	{
		if(!node)
		{
			return IMPOSSIBLE_MATCH;
		}
		switch(node->kind){
		case  Ast::DOT:
		{
			return	matchLevel((FieldReference*)node, resolve);
		}
		break;
		case  Ast::NAME:
		{
			return	matchLevel((AstName*)node, resolve);
		}
		break;
		case  Ast::VARIABLE_DECLARATOR:
		{
			return	matchLevel((AstVariableDeclarator*)node, resolve);
		}
		case Ast::ASSIGNMENT: {
			auto lef_exp = reinterpret_cast<AstAssignmentExpression*>(node)->left_hand_side;
			if(!lef_exp)
			{
				return IMPOSSIBLE_MATCH;
			}
			switch(lef_exp->kind)
			{
			case  Ast::DOT:
			{
				return	matchLevel((FieldReference*)lef_exp, resolve);
			}
			break;
			case  Ast::NAME:
			{
				return	matchLevel((AstName*)lef_exp, resolve);
			}
			default:
				break;
			}
		}
		break;
		case Ast::POST_UNARY: {
			auto lef_exp = reinterpret_cast<AstPostUnaryExpression*>(node)->expression;
			if (!lef_exp)
			{
				return IMPOSSIBLE_MATCH;
			}
			switch (lef_exp->kind)
			{
			case  Ast::DOT:
			{
				return	matchLevel((FieldReference*)lef_exp, resolve);
			}
			break;
			case  Ast::NAME:
			{
				return	matchLevel((AstName*)lef_exp, resolve);
			}
			default:
				break;
			}
		}
		 break;
		case Ast::PRE_UNARY: {
			auto lef_exp = reinterpret_cast<AstPreUnaryExpression*>(node)->expression;
			if (!lef_exp)
			{
				return IMPOSSIBLE_MATCH;
			}
			switch (lef_exp->kind)
			{
			case  Ast::DOT:
			{
				return	matchLevel((FieldReference*)lef_exp, resolve);
			}
			break;
			case  Ast::NAME:
			{
				return	matchLevel((AstName*)lef_exp, resolve);
			}
			default:
				break;
			}
		}
		 break;
		default:
			break;
		}
		return IMPOSSIBLE_MATCH;
	}

	int FieldReferencePattern::matchLevel(FieldReference* fieldRef, bool resolve)
	{
		// field name
		if (!fieldRef||   !fieldRef->identifier_token ||
			!matchesName(name, fieldRef->identifier_token->getName()))
			return IMPOSSIBLE_MATCH;

		if (resolve) {
			// receiver type and field type
			return matchLevel(fieldRef->symbol);
		}
		else {
			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}
//	AstAssignmentExpression
	int FieldReferencePattern::matchLevel(AstVariableDeclarator* fieldDecl, bool resolve)
	{
		if(!fieldDecl || !fieldDecl->variable_initializer_opt 
			|| !fieldDecl->variable_declarator_name || !fieldDecl->variable_declarator_name->identifier_token){
			return IMPOSSIBLE_MATCH;
		}

		// field name
		if (!matchesName(name, fieldDecl->variable_declarator_name->identifier_token->getName()))
			return IMPOSSIBLE_MATCH;

		if (resolve) {
			// receiver type and field type
			return matchLevel(fieldDecl->symbol);
		}
		else {
			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int FieldReferencePattern::matchLevel(AstName* nameRef, bool resolve)
	{
		if (!resolve) {
			// field name
			if (name.empty() == true) {
				return IMPOSSIBLE_MATCH;
			}
			else {
				if (nameRef->identifier_token && matchesName(name, nameRef->identifier_token->getName() )) {
					// can only be a possible match since resolution is needed 
					// to find out if it is a field ref
					return POSSIBLE_MATCH;
				}
				else {
					return IMPOSSIBLE_MATCH;
				}
					
			}
		}
		else {
			return	matchLevel(nameRef->symbol);
		}
	}

	int FieldReferencePattern::matchLevel(Symbol* _symbol)
	{
		if (_symbol == nullptr || _symbol->_kind != Symbol::VARIABLE) {
			return INACCURATE_MATCH;
		}
		VariableSymbol* binding = reinterpret_cast<VariableSymbol*>(_symbol);

		if (binding == nullptr) return INACCURATE_MATCH;

		int level;
		// receiver type
		auto receiverBinding = binding->ContainingType();

		if (receiverBinding == nullptr) {
			if (binding->getNameString() == L"length") {
				// optimized case for length field of an array
				if (declaringQualification.empty() == true 
					&& declaringSimpleName.empty() == true) {
					return ACCURATE_MATCH;
				}
				else {
					return IMPOSSIBLE_MATCH;
				}
			}
			else {
				return INACCURATE_MATCH;
			}
		}
		else {
			// Note there is no dynamic lookup for field access
			level = matchLevelForType(declaringSimpleName, declaringQualification, receiverBinding);
			if (level == IMPOSSIBLE_MATCH) {
				return IMPOSSIBLE_MATCH;
			}
		}

		// look at field type only if declaring type is not specified
		if (declaringSimpleName.empty() == true) {
			int newLevel = matchLevelForType(typeSimpleName, typeQualification, binding->Type());
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

	void ReportReference(Ast* reference, JavaModel::IJavaElement* element, int accuracy, MatchSet* locator ,int extra_info)
	{
		if (!reference || !locator)
			return;

		if (reference->kind == Ast::DOT) {
			auto leftTokon = ((FieldReference*)reference)->identifier_token;
			if(leftTokon)
			{
				locator->report(
					leftTokon->start_location,
					leftTokon->EndLocation(),
					element,
					accuracy, extra_info);
			}
		}
		else {
			auto leftTokon = ((AstName*)reference)->identifier_token;
			if (leftTokon)
			{
				locator->report(
					leftTokon->start_location,
					leftTokon->EndLocation(),
					element,
					accuracy, extra_info);
			}
		}
	}
	
	void FieldReferencePattern::matchReportReference(Ast* reference, JavaModel::IJavaElement* element, 
		int accuracy, MatchSet* locator)
	{
		if (!reference || !locator)
			return;
		switch (reference->kind) {
		case  Ast::DOT:
		case  Ast::NAME:
		{
			if(writesNodes.find(reference) != writesNodes.end()){
				return;
			}
			ReportReference(reference, element, accuracy, locator, FieldReferenceAttribute::READ_VALUE);
		}
		
		break;

		case  Ast::VARIABLE_DECLARATOR:
			{
			auto variable_declarator_name = ((AstVariableDeclarator*)reference)->variable_declarator_name;
			if (!variable_declarator_name || !variable_declarator_name->identifier_token)
				return;
			auto leftTokon = variable_declarator_name->identifier_token;
			locator->report(
				leftTokon->start_location,
				leftTokon->EndLocation(),
				element,
				accuracy,0);
		}
		break;
		case Ast::ASSIGNMENT: {
			auto lef_exp = reinterpret_cast<AstAssignmentExpression*>(reference)->left_hand_side;
			if (!lef_exp)
				return;
			ReportReference(lef_exp, element, accuracy, locator, FieldReferenceAttribute::WRITE_VALUE);
			writesNodes.insert(lef_exp);
		}
		break;

		case Ast::POST_UNARY: {
			auto lef_exp = reinterpret_cast<AstPostUnaryExpression*>(reference)->expression;
			if (!lef_exp)
				return;
			ReportReference(lef_exp, element, accuracy, locator, FieldReferenceAttribute::WRITE_VALUE);
			writesNodes.insert(lef_exp);
		}
		break;
		case Ast::PRE_UNARY: {
			auto lef_exp = reinterpret_cast<AstPreUnaryExpression*>(reference)->expression;
			if (!lef_exp)
				return;
			ReportReference(lef_exp, element, accuracy, locator, FieldReferenceAttribute::WRITE_VALUE);
			writesNodes.insert(lef_exp);
		}
		default:
			locator->report(reference, element, accuracy, 0);
		}	
	}
}


} // Close namespace Jikes block


