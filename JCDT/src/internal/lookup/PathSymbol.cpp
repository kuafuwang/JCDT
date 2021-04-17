

#include <JCDT_Lib/internal/lookup/PathSymbol.h>
#include <JCDT_Lib/internal/util/zip/zip.h>

namespace Jikes { // Open namespace Jikes block


	PathSymbol::PathSymbol(const NameSymbol* name_symbol_)
		: name_symbol(name_symbol_)
		, zipfile(NULL), root_directory(nullptr)
	{
		Symbol::_kind = PATH;
	}

	PathSymbol::~PathSymbol()
	{
		if (zipfile)
			delete zipfile;
	}
	

} // Close namespace Jikes block




