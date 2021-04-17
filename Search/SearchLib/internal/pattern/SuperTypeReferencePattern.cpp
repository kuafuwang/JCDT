#include "stdafx.h"
#include "SuperTypeReferencePattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/classfmt/class.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	SuperTypeReferencePattern::SuperTypeReferencePattern(std::wstring _superQualification, 
		std::wstring _superSimpleName, int _matchMode, bool _isCaseSensitive)
		:SearchPattern(_matchMode, _isCaseSensitive), decodedSuperClassOrInterface(0), decodedClassOrInterface(0),
		decodedModifiers(0)
	{

		superQualification.swap(_superQualification);
		superSimpleName.swap(_superSimpleName);
		if(isCaseSensitive)
		{
			boost::to_lower(superQualification);
			boost::to_lower(superSimpleName);
		}
		
	}

	bool SuperTypeReferencePattern::matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
		std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::CLASS_INFO) || !binaryInfo.first  || !class_data.first)
			return false;

		
		Jikes::ClassFile* class_file = (Jikes::ClassFile*)binaryInfo.first;
		const ConstantPool& pool = *(class_data.first);


		CPClassInfo* class_info = class_file->SuperClass();
		if (class_info) {
			wstring superclassName = CharOperation::ConvertUtf8ToUnicode(class_info->TypeName(pool));;
			boost::replace_all(superclassName, L"/", L".");
			if (matchesType(superSimpleName, superQualification, superclassName)) {
				return true;
			}
		}

	
		for (int i = 0, max = class_file->InterfacesCount(); i < max; i++) {
			class_info = class_file->Interface(i);

			if (!class_info)
				continue;

			wstring typeName = CharOperation::ConvertUtf8ToUnicode(class_info->TypeName(pool));;
			boost::replace_all(typeName, L"/", L".");
			if (matchesType(superSimpleName, superQualification, typeName)) {
				return true;
			}
		}
		return false;
	}

	int SuperTypeReferencePattern::matchLevel(Ast* node, bool resolve)
	{
		if(!node)
		{
			return IMPOSSIBLE_MATCH;
		}

		switch(node->kind)
		{
		case Ast::TYPE:
			break;
		default:
			return IMPOSSIBLE_MATCH;
		}
	
		AstTypeName* typeRef = (reinterpret_cast<AstTypeName*>(node));
		if (resolve) {
			auto  binding = typeRef->symbol;
			if (binding == nullptr) {
				return INACCURATE_MATCH;
			}
			else {
				return matchLevelForType(superSimpleName, superQualification, binding);
			}
		}
		else {
			if (superSimpleName.empty() == true) {
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
			else {
				auto typeRefSimpleName = typeRef->name->identifier_token->getName();
				if (matchesName(superSimpleName, typeRefSimpleName))
					return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
				else
					return IMPOSSIBLE_MATCH;
			}
		}
	}

	int SuperTypeReferencePattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;

		if (!(binding->_kind == Symbol::TYPE)) 
			return IMPOSSIBLE_MATCH;

		// super class
		TypeSymbol* type = (TypeSymbol*)binding;
		int level = matchLevelForType(superSimpleName, superQualification, type->super);
		switch (level) {
		case IMPOSSIBLE_MATCH:
			break; // try to find match in super interfaces
		case ACCURATE_MATCH:
			return ACCURATE_MATCH;
		default: // ie. INACCURATE_MATCH
			break; // try to find accurate match in super interfaces
		}

		// super interfaces	
		int i = 0, max = type->NumInterfaces();
		if (!max)
			return ACCURATE_MATCH;

		auto interfaces  = type->interfaces->data();
		for (; i < max; i++) {
			int newLevel = matchLevelForType(superSimpleName, superQualification, interfaces[i] );
			switch (newLevel) {
			case IMPOSSIBLE_MATCH:
				break;
			case ACCURATE_MATCH:
				return ACCURATE_MATCH;
			default: // ie. INACCURATE_MATCH
				level = newLevel;
				break;
			}
		}
		return level;
	}
}


} // Close namespace Jikes block


