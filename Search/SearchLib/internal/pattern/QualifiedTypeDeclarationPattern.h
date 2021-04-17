#ifndef  QualifiedTypeDeclarationPattern_JIKES_SERACH_INCLUDE
#define QualifiedTypeDeclarationPattern_JIKES_SERACH_INCLUDE
#include "TypeDeclarationPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class QualifiedTypeDeclarationPattern:public TypeDeclarationPattern
	{
	public:
		 std::wstring qualification;
		 std::wstring decodedQualification;

		QualifiedTypeDeclarationPattern(
			std::wstring _qualification,
			std::wstring _simpleName,
			wchar_t _classOrInterface,
			int _matchMode,
			bool _isCaseSensitive);


		/**
		* @see SearchPattern#matchesBinary(Object, Object)
		*/
		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, 
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);
		  
		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
	
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif