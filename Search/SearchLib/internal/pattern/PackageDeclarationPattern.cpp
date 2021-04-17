#include "stdafx.h"
#include "PackageDeclarationPattern.h"
namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	PackageDeclarationPattern::PackageDeclarationPattern(std::wstring _pkgName, int _matchMode, bool _isCaseSensitive):SearchPattern(_matchMode, _isCaseSensitive), pkgName(_pkgName)
	{
		
	}
}


} // Close namespace Jikes block


