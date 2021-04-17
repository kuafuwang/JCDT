
#ifndef MethodShadowSymbol_jikse_INCLUDED
#define MethodShadowSymbol_jikes_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

	class MethodSymbol;

	class MethodShadowSymbol
	{
	public:
		MethodSymbol* method_symbol;
		MethodShadowSymbol* next_method;

		MethodShadowSymbol(MethodSymbol* method_symbol_);

		~MethodShadowSymbol();

		MethodSymbol* Conflict(unsigned i) const;

		 unsigned NumConflicts() const;
		 void AddConflict(MethodSymbol* conflict_symbol);

		  void CompressSpace();

	private:
		friend class ExpandedMethodTable;
		MethodShadowSymbol* next;

		ConvertibleArray<MethodSymbol*>* conflict;

		bool Find(const MethodSymbol* conflict_symbol);
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

