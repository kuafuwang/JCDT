#include "stdafx.h"
#include "LocalFieldReferencePattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <SearchLib/internal/matching/MatchSet.h>
#include <JCDT_Lib/internal/lex/Token.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <SearchLib/internal/matching/SearchResultNode.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	LocalFieldReferencePattern::LocalFieldReferencePattern(
		const string& _doc_path,
		JavaModel::ISourceRange* _rng,
		 std::wstring _name,
		int _matchMode,
		bool _isCaseSensitive,
		std::wstring _typeQualification,
		std::wstring _typeSimpleName,
		bool _readAccess,
		bool _writeAccess) :
		MultipleSearchPattern(_matchMode, _isCaseSensitive),
		path(_doc_path),
		rng(_rng), start_loc(0), end_loc(0), readAccess(_readAccess)
		, writeAccess(_writeAccess)
	{
		boost::to_lower(path);
		name.swap(_name);
		typeSimpleName.swap(_typeSimpleName);
		typeQualification.swap(_typeQualification);

		if(!isCaseSensitive)
		{
			boost::to_lower(name);
			boost::to_lower(typeQualification);
			boost::to_lower(typeSimpleName);
		}
		
		_needsResolve = needsResolve();
	}
	void LocalFieldReferencePattern::ReportReference(Ast* reference, JavaModel::IJavaElement* element, 
		int accuracy, MatchSet* locator, int extra_info)
	{
		if (!reference || !locator)
			return;

		if (reference->kind == Ast::DOT) {
			auto leftTokon = ((FieldReference*)reference)->identifier_token;
			if(leftTokon)
				locator->report(
				leftTokon->start_location,
				leftTokon->EndLocation(),
				element,
				accuracy, extra_info);
		}
		else {
			auto leftTokon = ((AstName*)reference)->identifier_token;
			if (leftTokon)
				locator->report(
				leftTokon->start_location,
				leftTokon->EndLocation(),
				element,
				accuracy, extra_info);
		}
	}

	void LocalFieldReferencePattern::matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
		int accuracy, MatchSet* locator)
	{
		if (!rng) {
			locator->report(reference, element, accuracy, 0);
			return;
		}
		if (!reference || !locator)
			return;

		auto left = reference->LeftToken();
		if (left) {
			if (start_loc > left->start_location)
				return;
		}
		auto right = reference->RightToken();
		if (right) {
			if (end_loc < right->EndLocation())
				return;
		}


		switch (reference->kind) {
		case  Ast::DOT:
		case  Ast::NAME:
		{
			if (writesNodes.find(reference) != writesNodes.end()) {
				return;
			}
			ReportReference(reference, element, accuracy, locator, FieldReferenceAttribute::READ_VALUE);
		}

		break;

		case  Ast::VARIABLE_DECLARATOR: {
			auto variable_declarator_name = ((AstVariableDeclarator*)reference)->variable_declarator_name;
			if (!variable_declarator_name || !variable_declarator_name->identifier_token)
				return;

			auto leftTokon = variable_declarator_name->identifier_token;
			
			locator->report(
				leftTokon->start_location,
				leftTokon->EndLocation(),
				element,
				accuracy, 0);
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


	
	bool LocalFieldReferencePattern::IsMayInTheFile(BaseLexStream* lex)
	{
		if (!lex)
			return false;

		LexStream* lex_stream = (LexStream*)(lex);
		string file_path = lex_stream->FileName();
		boost::to_lower(file_path);
		if(path == file_path){
			bool ret= SearchPattern::IsMayInTheFile(lex);
			if(ret)
			{
				if(rng){
					start_loc = lex->FindPosition(rng->m_nStartLine, rng->m_nStartIndex);
					end_loc = lex->FindPosition(rng->m_nEndLine,   rng->m_nEndIndex);
				}
				 
			}
			return ret;
		}
		return false;
	}

	int LocalFieldReferencePattern::matchContainer()
	{
		int _matchContainer = METHOD | FIELD;
		return _matchContainer;
	}

	

	int LocalFieldReferencePattern::matchLevel(Ast* node, bool resolve)
	{
	    if(!node)
			return IMPOSSIBLE_MATCH;

		switch (node->kind) {
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
			if (!lef_exp)
				return IMPOSSIBLE_MATCH;

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
		case Ast::POST_UNARY: {
			auto lef_exp = reinterpret_cast<AstPostUnaryExpression*>(node)->expression;
			if (!lef_exp)
				return IMPOSSIBLE_MATCH;
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
				return IMPOSSIBLE_MATCH;
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


	int LocalFieldReferencePattern::matchLevel(FieldReference* fieldRef, bool resolve)
	{
		if(!fieldRef )
			return IMPOSSIBLE_MATCH;
		// field name
		if (fieldRef->identifier_token && !matchesName(name, fieldRef->identifier_token->getName()))
			return IMPOSSIBLE_MATCH;

		if (resolve) {
			// receiver type and field type
			return matchLevel(fieldRef->symbol);
		}
		else {
			
			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int LocalFieldReferencePattern::matchLevel(AstVariableDeclarator* fieldDecl, bool resolve)
	{
		if (!fieldDecl)
			return IMPOSSIBLE_MATCH;
		// field name
		if (fieldDecl->variable_declarator_name && fieldDecl->variable_declarator_name->identifier_token&&
			!matchesName(name, fieldDecl->variable_declarator_name->identifier_token->getName()))
			return IMPOSSIBLE_MATCH;

		if (resolve) {
			// receiver type and field type
			return matchLevel(fieldDecl->symbol);
		}
		else {
			return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int LocalFieldReferencePattern::matchLevel(AstName* nameRef, bool resolve)
	{
		if (!nameRef)
			return IMPOSSIBLE_MATCH;

		if (!resolve) {
			// field name
			if (name.empty() == true) {
				return IMPOSSIBLE_MATCH;
			}
			else {
				if (nameRef->identifier_token && 
					matchesName(name, nameRef->identifier_token->getName() )) {
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

	int LocalFieldReferencePattern::matchLevel(Symbol* _symbol)
	{
		if (_symbol == nullptr || _symbol->_kind != Symbol::VARIABLE) {
			return INACCURATE_MATCH;
		}
		VariableSymbol* binding = reinterpret_cast<VariableSymbol*>(_symbol);

		if (binding == nullptr) return INACCURATE_MATCH;

		if(!binding->IsLocal()){
			return IMPOSSIBLE_MATCH;
		}
		// receiver type
		// look at field type only if declaring type is not specified 
		return   matchLevelForType(typeSimpleName, typeQualification, binding->Type());

	}
}


} // Close namespace Jikes block


