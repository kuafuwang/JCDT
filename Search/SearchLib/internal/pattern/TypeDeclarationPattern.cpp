#include "stdafx.h"
#include "TypeDeclarationPattern.h"
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <SearchLib/internal/core/IIndexConstants.h>
#include <JCDT_Lib/internal/lookup/Symbol.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/classfmt/class.h>

using namespace  Jikes::PathModel;

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	TypeDeclarationPattern::TypeDeclarationPattern(int _matchMode, bool _isCaseSensitive):SearchPattern(_matchMode, _isCaseSensitive), classOrInterface(0)
	{
			
	}

	TypeDeclarationPattern::TypeDeclarationPattern(std::wstring _pkg, std::vector<std::wstring> _enclosingTypeNames, 
		std::wstring _simpleName, wchar_t _classOrInterface,
		int _matchMode, bool _isCaseSensitive):SearchPattern(_matchMode, _isCaseSensitive), classOrInterface(_classOrInterface)
	{

		pkg.swap(_pkg);
		simpleName.swap(_simpleName);
		enclosingTypeNames.swap(_enclosingTypeNames);
		if(!isCaseSensitive)
		{
			boost::to_lower(pkg);
			boost::to_lower(simpleName);
			auto data = enclosingTypeNames.data();
			auto size = enclosingTypeNames.size();
			for(auto i = 0 ; i <  size; ++i)
			{
				boost::to_lower(data[i]);
			}
		}
		
		_needsResolve = pkg.empty() != true && enclosingTypeNames.empty() != true;
	}

	bool TypeDeclarationPattern::matchesBinary(
		std::pair<void*, BinaryInfoType>&  binaryInfo, std::pair<const ConstantPool*, CPClassInfo*>& class_data)
	{
		if (!(binaryInfo.second == BinaryInfoType::CLASS_INFO) || !binaryInfo.first || !class_data.first || !class_data.second)
			return false;

		Jikes::ClassFile* class_file = (Jikes::ClassFile*)binaryInfo.first;
		CPClassInfo* class_info = class_data.second;
		const ConstantPool& pool = *(class_data.first);

		wstring typeName = CharOperation::ConvertUtf8ToUnicode(class_info->TypeName(pool));;
		boost::replace_all(typeName, L"/", L".");

		auto enclosingTypeName = enclosingTypeNames.empty() == true
			? L"" : boost::join(enclosingTypeNames, L".");
		if (!matchesType(simpleName, pkg, enclosingTypeName, typeName)) {
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

		default:
			break;
		}

		return true;
	}

	bool TypeDeclarationPattern::matchesType(const wstring& simpleNamePattern,
		const wstring& pkgPattern, const wstring& enclosingNamePattern,
		const wstring& fullyQualifiedTypeName)
	{
		if (enclosingNamePattern.empty() == true) {
			return SearchPattern::matchesType(simpleNamePattern,
				pkgPattern, 
				fullyQualifiedTypeName);
		}
		else {
			wstring pattern;
			if (pkgPattern.empty() == true) {
				pattern = enclosingNamePattern;
			}
			else {
				pattern = pkgPattern + L"." +  enclosingNamePattern;
			}
			return SearchPattern::matchesType(simpleNamePattern, pattern, fullyQualifiedTypeName);
		}
	}

	
	bool TypeDeclarationPattern::matchIndexEntry(
		const wstring& decodedPackage,
		std::vector< std::wstring >& decodedEnclosingTypeNames,
		const wstring& decodedSimpleName, wchar_t decodedClassOrInterface
		)
	{

		/* check class/interface nature */
		switch (classOrInterface) {
		case IIndexConstants::CLASS_SUFFIX:
		case IIndexConstants::INTERFACE_SUFFIX:
			if (classOrInterface != decodedClassOrInterface) return false;
		default:
			break;
		}
		/* check qualification - exact match only */
		if (pkg.empty() != true && !CharOperation::equals(pkg, decodedPackage, isCaseSensitive))
			return false;
		/* check enclosingTypeName - exact match only */
		auto len =  enclosingTypeNames.size();
		auto decodeLen = decodedEnclosingTypeNames.size();
		if (len != decodeLen) {
			return false;
		}
		
		for (auto i = 0; i < len; ++i)
		{
			if (!CharOperation::equals(enclosingTypeNames[i], decodedEnclosingTypeNames[i], isCaseSensitive))
				return false;
		}
				
		/* check simple name matches */
		if (simpleName.empty() != true) {
			switch (matchMode) {
			case EXACT_MATCH:
				if (!CharOperation::equals(simpleName, decodedSimpleName, isCaseSensitive)) {
					return false;
				}
				break;
			case PREFIX_MATCH:
				if (!CharOperation::prefixEquals(simpleName, decodedSimpleName, isCaseSensitive)) {
					return false;
				}
				break;
			case PATTERN_MATCH:
				if (!CharOperation::match(simpleName, decodedSimpleName, isCaseSensitive)) {
					return false;
				}
			default:
				break;
			}
		}
		return true;
	}




	int TypeDeclarationPattern::matchLevel(Ast* node, bool resolve)
	{
		if(!node)
			return IMPOSSIBLE_MATCH;

		switch(node->kind )
		{
		case Ast::ANNOTATION_TYPE:
		case Ast::ANONYMOUS_DECLARATION:
		case Ast::CLASS:
		case Ast::ENUM_TYPE:
		case Ast::INTERFACE_TYPE:
			break;
		default:
			return IMPOSSIBLE_MATCH;
		}
	
		TypeDeclaration* type = reinterpret_cast<TypeDeclaration*> (node);

		if (resolve) {
			return matchLevel(type->symbol);
		}
		else {
			// type nameConstant
			if (simpleName.empty() != true && !matchesName(simpleName, type->getName()))
				return IMPOSSIBLE_MATCH;
			else
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
		}
	}

	int TypeDeclarationPattern::matchLevel(Symbol* binding)
	{
		if (binding == nullptr) 
			return INACCURATE_MATCH;
		if (!(binding->_kind == Symbol::TYPE))
			return IMPOSSIBLE_MATCH;

		TypeSymbol* type = (TypeSymbol*)binding;

		// class or interface
		switch (classOrInterface) {
		case IIndexConstants::CLASS_SUFFIX:
			if ( type->ACC_INTERFACE() )
				return IMPOSSIBLE_MATCH;
			break;
		case IIndexConstants::INTERFACE_SUFFIX:
			if (!type->ACC_INTERFACE())
				return IMPOSSIBLE_MATCH;
			break;
		}

		// fully qualified name
		wstring enclosingTypeName = enclosingTypeNames.empty() == true ? L"" : boost::join(enclosingTypeNames, L".");
			
		return matchLevelForType(simpleName, pkg, enclosingTypeName, type);
	}

	int TypeDeclarationPattern::matchLevelForType(const wstring& simpleNamePattern,
		const wstring& qualificationPattern,
		const wstring& enclosingNamePattern, 
		TypeSymbol* type)
	{
		if (enclosingNamePattern.empty() == true) {
			return SearchPattern::matchLevelForType(simpleNamePattern, qualificationPattern, type);
		}
		else {
			if (qualificationPattern.empty() == true) {
				return  SearchPattern::matchLevelForType(simpleNamePattern, enclosingNamePattern, type);
			}
			else {
				// pattern was created from a Java element: qualification is the package name.
				auto fullQualificationPattern = qualificationPattern + L"." + enclosingNamePattern ;

				
				auto compound_name = boost::replace_all_copy(type->ContainingPackage()->PackageNameString(), L"/", L".");

				if (pkg == compound_name) {
					return SearchPattern::matchLevelForType(simpleNamePattern, fullQualificationPattern, type);
				}
				else {
					return IMPOSSIBLE_MATCH;
				}
			}
		}
	}

	bool TypeDeclarationPattern::matchesBinaryPackge(const wstring& compoundPackageName)
	{
		if(pkg.find(compoundPackageName) != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool TypeDeclarationPattern::matchesBinaryClass(const std::wstring& mainTypeName){
		if(!enclosingTypeNames.empty()){
			std::wstring qualifiedType = boost::join(enclosingTypeNames, L"$");
			if (!qualifiedType.empty())
				qualifiedType.append(L"$").append(simpleName);
			if (qualifiedType == mainTypeName) {
				return true;
			}	
			return false;
		}
		else{
			return mainTypeName == simpleName;
		}

	}


}


} // Close namespace Jikes block


