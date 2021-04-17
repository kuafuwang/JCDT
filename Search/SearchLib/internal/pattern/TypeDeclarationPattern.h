#ifndef  TypeDeclarationPattern_JIKES_SERACH_INCLUDE
#define TypeDeclarationPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block
	namespace  PathModel
	{
		class TypeInfo;
	}
namespace Search{
	
	class TypeDeclarationPattern:public SearchPattern
	{
	public:
		 std::wstring pkg;
		 std::vector< std::wstring >  enclosingTypeNames;
		 std::wstring simpleName;
		
		// set to CLASS_SUFFIX for only matching classes 
		// set to INTERFACE_SUFFIX for only matching interfaces
		// set to TYPE_SUFFIX for matching both classes and interfaces
		 wchar_t classOrInterface;


		TypeDeclarationPattern(int _matchMode, bool _isCaseSensitive);

		TypeDeclarationPattern(
			std::wstring _pkg,
			std::vector<std::wstring> _enclosingTypeNames,
			std::wstring _simpleName,
			wchar_t _classOrInterface,
			int _matchMode,
			bool _isCaseSensitive);


		/**
		* @see SearchPattern#matchContainer()
		*/
		 int matchContainer() {
			return COMPILATION_UNIT | CLASS | METHOD | FIELD;
		}
		/**
		* @see SearchPattern#matchesBinary(Object, Object)
		*/
		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, 
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);
		/**
		* Returns whether the given type binding matches the given simple name pattern
		* package pattern and enclosing name pattern.
		*/
		bool matchesType(const wstring& simpleNamePattern, const wstring& pkgPattern,
		                 const wstring& enclosingNamePattern, const wstring& fullyQualifiedTypeName);

	
	
	
		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);
		bool matchIndexEntry(const wstring& decodedPackage,
			std::vector< std::wstring >& decodedEnclosingTypeNames,
			const wstring& decodedSimpleName, wchar_t decodedClassOrInterface);
		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);

		/**
		* Returns whether the given type binding matches the given simple name pattern
		* qualification pattern and enclosing type name pattern.
		*/
		int matchLevelForType(const wstring& simpleNamePattern,
		                      const wstring& qualificationPattern,
							  const wstring& enclosingNamePattern,
		                      TypeSymbol* type);
		std::wstring indexEntryPrefix() override {
			return simpleName;
		}

		bool matchesBinaryPackge(const std::wstring& compoundPackageName) override;
		bool matchesBinaryClass(const std::wstring& mainTypeName) override;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif