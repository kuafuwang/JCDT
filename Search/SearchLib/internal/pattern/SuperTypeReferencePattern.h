#ifndef  SuperTypeReferencePattern_JIKES_SERACH_INCLUDE
#define SuperTypeReferencePattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	
	class SuperTypeReferencePattern:public SearchPattern
	{
	public:
		 std::wstring superQualification;
		 std::wstring superSimpleName;

		 std::wstring decodedSuperQualification;
		 std::wstring decodedSuperSimpleName;
		 wchar_t decodedSuperClassOrInterface;
		 std::wstring decodedQualification;
		 std::wstring decodedSimpleName;
		 std::wstring decodedEnclosingTypeName;
		 wchar_t decodedClassOrInterface;
		 int decodedModifiers;


		SuperTypeReferencePattern(
			std::wstring _superQualification,
			std::wstring _superSimpleName,
			int _matchMode,
			bool _isCaseSensitive);
		int matchContainer() {
			return CLASS;
		}

		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, 
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
		std::wstring indexEntryPrefix() override{
			return superSimpleName;
		};
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif