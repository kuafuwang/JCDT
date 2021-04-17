
#ifndef ExpandedFieldTable_jikes_INCLUDED
#define ExpandedFieldTable_jikes_INCLUDED


#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>

namespace Jikes { // Open namespace Jikes block

	class VariableShadowSymbol;
	class VariableSymbol;
	class NameSymbol;
	class ExpandedFieldTable
	{
	public:
		enum
		{
			DEFAULT_HASH_SIZE = 251,
			MAX_HASH_SIZE = 509
		};

		ConvertibleArray<VariableShadowSymbol*> symbol_pool;

		void CompressSpace();

		ExpandedFieldTable(unsigned hash_size_ = DEFAULT_HASH_SIZE);
		~ExpandedFieldTable();

		 VariableShadowSymbol* InsertVariableShadowSymbol(VariableSymbol* variable_symbol);

		 VariableShadowSymbol* FindVariableShadowSymbol(const NameSymbol* name_symbol);

	private:
		VariableShadowSymbol** base;
		unsigned hash_size;
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

