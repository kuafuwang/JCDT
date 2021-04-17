#include "stdafx.h"
#include "SuperInterfaceReferencePattern.h"
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/classfmt/class.h>
#include <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	bool SuperInterfaceReferencePattern::matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
		std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::CLASS_INFO) || !binaryInfo.first || !class_data.first)
			return false;

		Jikes::ClassFile* class_file = (Jikes::ClassFile*)binaryInfo.first;
		const ConstantPool& pool = *(class_data.first);
		for (int i = 0, max = class_file->InterfacesCount(); i < max; i++) {
			CPClassInfo* class_info = class_file->Interface(i);

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

	int SuperInterfaceReferencePattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr)
			return INACCURATE_MATCH;
		if (!(binding->_kind != Symbol::TYPE)) 
			return IMPOSSIBLE_MATCH;

		// super interfaces
		int level = IMPOSSIBLE_MATCH;
		auto type = (TypeSymbol*)binding;
		int max = type->NumInterfaces();
		if (!max)
			return IMPOSSIBLE_MATCH;

		auto superInterfaces = type->interfaces->data();
		for (int i = 0; i < max; i++) {
			int newLevel = matchLevelForType(superSimpleName, superQualification, superInterfaces[i]);
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


