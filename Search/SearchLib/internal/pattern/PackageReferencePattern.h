#ifndef  PackageReferencePattern_JIKES_SERACH_INCLUDE
#define PackageReferencePattern_JIKES_SERACH_INCLUDE
#include "AndPattern.h"
#include <vector>
#include <JCDT_Lib/internal/ast/AstName.h>

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class PackageReferencePattern:public AndPattern
	{
	public:
		std::wstring  pkgName;

		
		 PackageReferencePattern(const std::wstring& _pkgName, int _matchMode, bool _isCaseSensitive) 
			:AndPattern(_matchMode, _isCaseSensitive), pkgName(_pkgName)
		 {
			
		}
		 /**
		 * @see SearchPattern#matchContainer()
		 */
		  int matchContainer() {
			 return COMPILATION_UNIT | CLASS | METHOD | FIELD;
		 }
		
		
		int matchLevel(Ast* node, bool resolve) override;
	
		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
		std::wstring indexEntryPrefix() override {
			return pkgName;
		}

	};

}// Close namespace Search block
	

} // Close namespace Jikes block

#endif