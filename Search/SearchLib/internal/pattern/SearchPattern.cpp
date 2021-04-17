#include "stdafx.h"
#include "SearchPattern.h"
#include <SearchLib/internal/matching/MatchSet.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <SearchLib/internal/core/IIndexConstants.h>
#include <JCDT_Lib/internal/ast/ImportReference.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lex/LexStream.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	
	SearchPattern::SearchPattern(int _matchMode, bool _isCaseSensitive)
		:matchMode(_matchMode),isCaseSensitive(_isCaseSensitive), _needsResolve(true)
	{
			
	}

	bool SearchPattern::IsMayInTheFile(BaseLexStream* lex)
	{
		if (!lex)
			return false;

		wstring base_prefix = indexEntryPrefix();
		auto data_base_prefix = base_prefix.data();
		auto size_base_prefix = base_prefix.size();
		auto  tokens = lex->token_stream.data();
		auto token_size = lex->token_stream.size();
		for(auto i =0 ; i < token_size;++i)
		{
			auto token = tokens[i];
			if(token && token->_kind == TK_Identifier && token->additional_info.name_symbol)
			{
				if (matchesName(
					data_base_prefix, 
					size_base_prefix, 
					token->additional_info.name_symbol->Name(), token->additional_info.name_symbol->NameLength()
					))
				{
					return true;
				}
			}
			
		}
		
		return false;
	}

	int SearchPattern::matchCheck(Ast* node, MatchSet* set)
	{
		if (!set)
			return  SearchPattern::IMPOSSIBLE_MATCH;

		auto lv = matchLevel(node, false);
		switch (lv) {
		case SearchPattern::POSSIBLE_MATCH:
			set->addPossibleMatch(node);
			break;
		case SearchPattern::ACCURATE_MATCH:
			set->addTrustedMatch(node);
			break;
		default:
			break;
		}
	
		return lv;
	}

	bool SearchPattern::matchesName(const wstring& pattern, const wstring& name)
	{
		if (name.empty() != true) {
			switch (this->matchMode) {
				case EXACT_MATCH:
				{

					if (isCaseSensitive)
						return pattern == name;
					else
					{
						return CharOperation::equals(pattern, name, isCaseSensitive);
					}
				}
			
				case PREFIX_MATCH:
				{
					return CharOperation::prefixEquals(pattern, name, this->isCaseSensitive);
				
				}
				
				case PATTERN_MATCH:
				{
					return CharOperation::match(pattern, name, this->isCaseSensitive);
				}
				
			}
		}
		return false;
	}

	bool SearchPattern::matchesName(const wchar_t* pattern_data, size_t patten_len, const wchar_t* name_data, size_t name_len)
	{
		if (name_data != nullptr) {
			switch (this->matchMode) {
			case EXACT_MATCH:
			{

				if (isCaseSensitive)
					return CharOperation::equals(pattern_data, patten_len,
						name_data, name_len, isCaseSensitive);
				else
				{
					return CharOperation::equals(pattern_data, patten_len,
						name_data, name_len, isCaseSensitive);
				}
			}

			case PREFIX_MATCH:
			{
				CharOperation::prefixEquals(pattern_data, patten_len,
					name_data, name_len, this->isCaseSensitive);

			}

			case PATTERN_MATCH:
			{
				return CharOperation::match(pattern_data, patten_len,
					name_data, name_len,
					this->isCaseSensitive);
			}

			}
		}
		return false;
	}

	bool SearchPattern::matchesType(const wstring& simpleNamePattern, 
		const wstring& qualificationPattern,
		const wstring& fullyQualifiedTypeName)
	{
		wstring pattern;
		if (simpleNamePattern.empty() == true) 
		{
			if (qualificationPattern.empty() == true) {
				pattern = IIndexConstants::ONE_STAR;
			}
			else {
				pattern = qualificationPattern + L"." + IIndexConstants::ONE_STAR;
			}
		}
		else 
		{
			if (qualificationPattern.empty() == true) {
				pattern = IIndexConstants::ONE_STAR +  simpleNamePattern;
			}
			else {
				pattern = qualificationPattern + L"." + simpleNamePattern;
			}
		}

		return
			CharOperation::match(
				pattern,
				fullyQualifiedTypeName,
				this->isCaseSensitive
			);

	}

	void SearchPattern::matchReportImportRef(ImportReference* importRef,
		Symbol* binding, JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
	{
		// default is to report a match as a regular ref.
		matchReportReference(importRef, element, accuracy, locator);
	}

	void SearchPattern::matchReportReference(Ast* reference,
		JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
	{
		// default is to report a match on the whole node.
		if (!locator)
			return;

		locator->report(reference, element, accuracy,0);
	}

	int SearchPattern::matchLevelAsSubtype(TypeSymbol* type, 
		const wstring& simpleNamePattern, const wstring& qualificationPattern)
	{
		if (type == nullptr) 
			return INACCURATE_MATCH;

		int level;

		// matches type
		if ((level = this->matchLevelForType(simpleNamePattern, qualificationPattern, type)) != IMPOSSIBLE_MATCH)
			return level;

		// matches superclass
		if (!type->ACC_INTERFACE())
		{
			wstring qualifiedTyeName =  type->getCompoundName();
			if(!(qualifiedTyeName == StringConstant::US_java_SL_Object))
			{
				if ((level = this->matchLevelAsSubtype(
					type->super,
					simpleNamePattern, qualificationPattern)) != IMPOSSIBLE_MATCH) {
					return level;
				}
			}
			else{
				return IMPOSSIBLE_MATCH;
			}
			
		}

		// matches interfaces
		
		if (type->interfaces) {
			auto  interfaces = type->interfaces;
			auto data = interfaces->data();
			auto size = interfaces->size();
			for (int i = 0; i < size; i++) {
				if ((level = this->matchLevelAsSubtype(data[i], 
					simpleNamePattern, qualificationPattern)) != IMPOSSIBLE_MATCH) {
					return level;
				};
			}
		}

		return IMPOSSIBLE_MATCH;
	}

	int SearchPattern::matchLevelForType(const vector<wstring>& declaringTypes, 
		TypeSymbol* receiverType)
	{
		if (receiverType == nullptr)
			return INACCURATE_MATCH;

		if (declaringTypes.empty() == true) {
			return INACCURATE_MATCH; // we were not able to compute the declaring types, default to inaccurate
		}
		else {
			wstring compoundName = receiverType->getCompoundName();
			  boost::replace_all(compoundName,L"/",L".");
			for (int i = 0, max = declaringTypes.size(); i < max; i++) {
				if (declaringTypes[i] == compoundName) {
					return ACCURATE_MATCH;
				}
			}
			return IMPOSSIBLE_MATCH;
		}
	}

	int SearchPattern::matchLevelForType(const wstring& simpleNamePattern,
		const wstring& qualificationPattern, TypeSymbol* type)
	{
		if (type == nullptr)
			return INACCURATE_MATCH;
		wstring qualifiedPackageName = type->ContainingPackageName();
		boost::replace_all(qualifiedPackageName, L"/", ".");

		wstring qualifiedSourceName =
		type->IsLocal()?
		L"1."  + type->qualifiedSourceName('.') : //$NON-NLS-1$
		type->qualifiedSourceName('.');
		if (this->matchesType(
			simpleNamePattern,
			qualificationPattern,
			qualifiedPackageName.length() == 0 ?
				qualifiedSourceName :
				qualifiedPackageName + L"."+ qualifiedSourceName)) {
			return ACCURATE_MATCH;
		}
		else {
			return IMPOSSIBLE_MATCH;
		}
	}

	wstring SearchPattern::toArrayName(wstring simpleName, int dimensions)
	{
		if (dimensions == 0)
			return simpleName;
		
		for (int i = 0; i < dimensions; i++){
			simpleName.append(L"[]");
		}
		return simpleName;
	}
}


} // Close namespace Jikes block





