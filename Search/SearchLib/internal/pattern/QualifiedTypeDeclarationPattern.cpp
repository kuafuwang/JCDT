#include "stdafx.h"
#include "QualifiedTypeDeclarationPattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <SearchLib/internal/core/IIndexConstants.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/classfmt/class.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	QualifiedTypeDeclarationPattern::QualifiedTypeDeclarationPattern(
		std::wstring _qualification,
		std::wstring _simpleName,
		wchar_t _classOrInterface, int _matchMode, bool _isCaseSensitive):
		TypeDeclarationPattern(_matchMode, _isCaseSensitive)
	{

		qualification.swap(_qualification);
		simpleName.swap(_simpleName);
		if(!isCaseSensitive)
		{
			boost::to_lower(qualification);
			boost::to_lower(simpleName);
		}

		
		classOrInterface = _classOrInterface;
		
	}

	bool QualifiedTypeDeclarationPattern::matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
		std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::CLASS_INFO))
			return false;
		Jikes::ClassFile* class_file = (Jikes::ClassFile*)binaryInfo.second;
		if (!class_file)
			return false;
		CPClassInfo* class_info = class_data.second;
		if (!class_info || !class_data.first)
			return false;
		const ConstantPool& pool = *(class_data.first);

		// fully qualified name
		wstring typeName = CharOperation::ConvertUtf8ToUnicode(class_info->TypeName(pool));;
		boost::replace_all(typeName, L"/", L".");
		if (!SearchPattern::matchesType(simpleName, qualification, typeName)) {
			return false;
		}

		// class or interface
		switch (classOrInterface) {
		case IIndexConstants::CLASS_SUFFIX:
			if (class_file->ACC_INTERFACE())
				return false;
			break;
		case IIndexConstants::INTERFACE_SUFFIX:
			if (!class_file->ACC_INTERFACE())
				return false;
			break;
		}

		return true;
	}

	int QualifiedTypeDeclarationPattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr) return INACCURATE_MATCH;
		if (!(binding->_kind  == Symbol::TYPE))
			return IMPOSSIBLE_MATCH;

		auto  type = (TypeSymbol*)binding;

		// class or interface
		switch (classOrInterface) {
		case IIndexConstants::CLASS_SUFFIX:
			if (type->ACC_INTERFACE())
				return IMPOSSIBLE_MATCH;
			break;
		case IIndexConstants::INTERFACE_SUFFIX:
			if (!type->ACC_INTERFACE())
				return IMPOSSIBLE_MATCH;
			break;
		}

		// fully qualified name
		return SearchPattern::matchLevelForType(simpleName, qualification, type);
	}
}


} // Close namespace Jikes block


