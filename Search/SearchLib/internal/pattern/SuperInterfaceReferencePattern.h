#ifndef  SuperInterfaceReferencePattern_JIKES_SERACH_INCLUDE
#define SuperInterfaceReferencePattern_JIKES_SERACH_INCLUDE
#include "SuperTypeReferencePattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class SuperInterfaceReferencePattern:public SuperTypeReferencePattern
	{
	public:
		SuperInterfaceReferencePattern(std::wstring _superQualification, std::wstring _superSimpleName, 
			int _matchMode, bool _isCaseSensitive)
			:SuperTypeReferencePattern(_superQualification, _superSimpleName, _matchMode, _isCaseSensitive)
		{
			
		}

		/**
		* @see SearchPattern#matchesBinary
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