#ifndef  MethodPattern_JIKES_SERACH_INCLUDE
#define MethodPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class MethodPattern:public SearchPattern
	{
	public:

		// selector	
		 std::wstring selector;

		// declaring type
		 std::wstring declaringQualification;
		 std::wstring declaringSimpleName;

		// return type
		 std::wstring returnQualification;
		 std::wstring returnSimpleName;

		// parameter types
		std::vector< std::wstring > * parameterQualifications;
		std::vector< std::wstring > * parameterSimpleNames;
		MethodPattern(int _matchMode, bool _isCaseSensitive);
		bool IsMayInTheFile(BaseLexStream*) override;
		virtual wstring getPatternName() = 0;
		/**
		* Returns whether a method declaration or message send will need to be resolved to
		* find out if this method pattern matches it.
		*/
		std::wstring indexEntryPrefix() override {
			return selector;
		}
		bool needsResolve() {

			// declaring type
			if (declaringSimpleName.empty() != true || declaringQualification.empty() != true)
				return true;

			// return type
			if (returnSimpleName.empty() != true || returnQualification.empty() != true) 
				return true;

			// parameter types
			if (parameterSimpleNames && parameterQualifications)
			{
				
				for (int i = 0, max = parameterSimpleNames->size(); i < max; i++) {
					if (parameterQualifications->operator[](i).empty() !=  true 
						|| parameterSimpleNames->operator[](i).empty() != true) return true;
				}
			}
			return false;
		}
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif