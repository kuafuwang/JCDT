#ifndef  PackageDeclarationPattern_JIKES_SERACH_INCLUDE
#define PackageDeclarationPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class PackageDeclarationPattern:public SearchPattern
	{
	public:
		std::wstring pkgName;
		PackageDeclarationPattern(std::wstring _pkgName, int _matchMode, bool _isCaseSensitive);
		int matchContainer() {
			// used only in the case of a OrPattern
			return 0;
		}
		int matchLevel(Ast* node, bool resolve) {
			// used only in the case of a OrPattern
			return ACCURATE_MATCH;
		}

		std::wstring indexEntryPrefix() override {
			return pkgName;
		}


	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif