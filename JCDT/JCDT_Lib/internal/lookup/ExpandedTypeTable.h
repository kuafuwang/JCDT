
#ifndef ExpandedTypeTable_INCLUDED
#define ExpandedTypeTable_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>

namespace Jikes {
	class TypeSymbol;
	// Open namespace Jikes block


	class TypeShadowSymbol;
	class NameSymbol;
	class ExpandedTypeTable
	{
	public:
		enum
		{
			DEFAULT_HASH_SIZE = 251,
			MAX_HASH_SIZE = 509
		};

		ConvertibleArray<TypeShadowSymbol*> symbol_pool;

		void CompressSpace();

		ExpandedTypeTable(unsigned hash_size_ = DEFAULT_HASH_SIZE);

		~ExpandedTypeTable();

		 TypeShadowSymbol* InsertTypeShadowSymbol(TypeSymbol* type_symbol);

		 TypeShadowSymbol* FindTypeShadowSymbol(const NameSymbol* name_symbol);

	private:
		TypeShadowSymbol** base;
		unsigned hash_size;
};

} // Close namespace Jikes block


#endif // _INCLUDED

