
#ifndef ExpandedMethodTable_INCLUDED
#define ExpandedMethodTable_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes {
	class Token;
	// Open namespace Jikes block

	class Semantic;
	class MethodSymbol;
	class NameSymbol;
	class MethodShadowSymbol;


	class ExpandedMethodTable
	{
	public:
		enum
		{
			DEFAULT_HASH_SIZE = 251,
			MAX_HASH_SIZE = 509
		};

		ConvertibleArray<MethodShadowSymbol*> symbol_pool;

		 void CompressSpace();

		ExpandedMethodTable(unsigned hash_size_ = DEFAULT_HASH_SIZE);
		~ExpandedMethodTable();

		 MethodShadowSymbol* FindMethodShadowSymbol(const NameSymbol* name_symbol);

		 MethodShadowSymbol* InsertMethodShadowSymbol(MethodSymbol* method_symbol);

		 void Overload(MethodShadowSymbol* base_shadow,
			 MethodSymbol* overload_method);

		 MethodShadowSymbol* Overload(MethodSymbol* overload_method);

		 MethodShadowSymbol* FindOverloadMethodShadow(MethodSymbol* overload_method,
			 Semantic* sem, Token*  tok);

	private:
		MethodShadowSymbol** base;
		unsigned hash_size;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

