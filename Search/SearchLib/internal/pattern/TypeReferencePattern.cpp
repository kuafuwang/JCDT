#include "stdafx.h"
#include "TypeReferencePattern.h" 
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include "SearchLib/internal/matching/MatchSet.h"

namespace Jikes { // Open namespace Jikes block


namespace Search{
	 TypeReferencePattern::TypeReferencePattern(
		 std::wstring _qualification, std::wstring _simpleName, int _matchMode, bool _isCaseSensitive)
		 : MultipleSearchPattern(_matchMode, _isCaseSensitive)
	 {

		
		 qualification.swap(_qualification);
		 simpleName.swap(_simpleName);
		 if(!isCaseSensitive)
		 {
			 boost::to_lower(qualification);
			 boost::to_lower(simpleName);
		 }
	
	/*	if (simpleName.empty()) {
			 qualification == nullptr ? ONE_STAR_CHAR : '.', qualification);
		}*/

		_needsResolve = qualification.empty() != true;
	}

	
	int TypeReferencePattern::matchLevel(Ast* node, bool resolve)
	{
		if(!node || node->kind != Ast::NAME)
		return IMPOSSIBLE_MATCH;

		AstName* nameRef = reinterpret_cast<AstName*>(node);

		if (!resolve) {
			if (simpleName.empty() == true) {
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
			else {
				if (nameRef->identifier_token && matchesName(simpleName, nameRef->identifier_token->getName())) {
					// can only be a possible match since resolution is needed 
					// to find out if it is a type ref
					return POSSIBLE_MATCH;
				}
				else {
					return IMPOSSIBLE_MATCH;
				}
			}
		}
		else {
			auto binding = nameRef->symbol;
			if (binding && binding->_kind == Symbol::TYPE) {
				return matchLevelForType(simpleName, qualification, (TypeSymbol*)binding);
			}
			else {
				return IMPOSSIBLE_MATCH; // must be a type binding
			}
		}
	}

	int TypeReferencePattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;

		if (!(binding->_kind == Symbol::TYPE))
			return IMPOSSIBLE_MATCH;

		auto typeBinding = (TypeSymbol*)binding;

		if (typeBinding->IsArray())
			typeBinding = typeBinding->base_type;

	
		while (typeBinding != nullptr) {
			int level = matchLevelForType(simpleName, qualification, typeBinding);
			if (level != IMPOSSIBLE_MATCH) {
				return level;
			}
			if (typeBinding->IsNested()) {
				typeBinding = typeBinding->EnclosingType();
			}
			else {
				typeBinding = nullptr;
			}
		}
		return IMPOSSIBLE_MATCH;
	}

	 void TypeReferencePattern::matchReportReference(Ast* reference, JavaModel::IJavaElement* element, int accuracy,
		 MatchSet* locator)
	 {
		 if (!reference)
			 return;

		 if (!locator)
			 return;

		 if(Ast::NAME == reference->kind){
			 AstName* _name = AstName::NameCast(reference);
			 locator->report(_name->identifier_token->start_location, _name->identifier_token->EndLocation(), element, accuracy, 0);
		 }
		 else if (Ast::TYPE == reference->kind) {
			 AstTypeName* type_name = AstTypeName::TypeNameCast(reference);
			 if(type_name->name)
			 {
				 locator->report(type_name->name->identifier_token->start_location, type_name->name->identifier_token->EndLocation(), element, accuracy, 0);
			 }
			 
		 }
		 else
		 {
			 MultipleSearchPattern::matchReportReference(reference, element, accuracy, locator);
		 }
	 }
}


} // Close namespace Jikes block


