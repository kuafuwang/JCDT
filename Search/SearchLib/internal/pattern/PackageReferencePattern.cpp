#include "stdafx.h"
#include "PackageReferencePattern.h"
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <SearchLib/internal/matching/MatchLocator.h>
#include <SearchLib/internal/matching/MatchSet.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{

	 int PackageReferencePattern::matchLevel(Ast* node , bool resolve)
	{
		 if (!node)
			 return IMPOSSIBLE_MATCH;

		if(node->kind != Ast::NAME)
		{
			return IMPOSSIBLE_MATCH;
		}
		AstName* qNameRef = reinterpret_cast<AstName*>(node);
		if (!resolve) {
			if (pkgName.empty() == true) {
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
			else {
				switch (matchMode) {
				case EXACT_MATCH:
				case PREFIX_MATCH:
				{
					vector<wstring> tokens;
					GetAstNameAllNames(qNameRef, tokens);
					if (CharOperation::prefixEquals(pkgName, boost::join(tokens,L"."), isCaseSensitive)) {
						return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
					}
					else {
						return IMPOSSIBLE_MATCH;
					}
				}

				case PATTERN_MATCH:
				{
					auto pattern = pkgName[pkgName.length() - 1] == '*' ? pkgName
						: pkgName + L".*"; //$NON-NLS-1$
					vector<wstring> tokens;
					GetAstNameAllNames(qNameRef, tokens);
					if (CharOperation::match(pattern, boost::join(tokens, L"."), isCaseSensitive)) {
						return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
					}
					else {
						return IMPOSSIBLE_MATCH;
					}
				}
					
				default:
					return IMPOSSIBLE_MATCH;
				}
			}
		}
		else {
			return matchLevel(qNameRef->symbol);
		}
	}

	int PackageReferencePattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr) {
			return INACCURATE_MATCH;
		}
		if (binding->_kind == Symbol::PACKAGE ) {
			PackageSymbol* pkgBinding = reinterpret_cast<PackageSymbol*>(binding);
			wstring compoundName = pkgBinding->getNameString();
			boost::replace_all(compoundName, L"/", L".");

			if (matchesName(pkgName, compoundName)) {
				return ACCURATE_MATCH;
			}
			else {
				return IMPOSSIBLE_MATCH;
			}
		}
		else if (binding->_kind == Symbol::TYPE) {

			auto typeBinding = reinterpret_cast<TypeSymbol*>(binding);		
			if (typeBinding->IsArray()) {
				typeBinding = typeBinding->base_type;
			}

			if (typeBinding == nullptr) {
				return INACCURATE_MATCH;
			}

			auto pkgBinding = typeBinding->ContainingPackage();
			wstring compoundName = pkgBinding->getNameString();
			boost::replace_all(compoundName, L"/", L".");

			if (matchesName(pkgName, compoundName)) {
				return ACCURATE_MATCH;
			}
			else {
				return IMPOSSIBLE_MATCH;
			}
		}
		else {
			return IMPOSSIBLE_MATCH;
		}
	}
}


} // Close namespace Jikes block


