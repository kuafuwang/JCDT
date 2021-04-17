
#ifndef TypeShadowSymbol_jikes_INCLUDED
#define TypeShadowSymbol_jikes_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

	class TypeSymbol;

	class TypeShadowSymbol
	{
	public:
		TypeSymbol* type_symbol;

		TypeShadowSymbol(TypeSymbol* type_symbol_);

		~TypeShadowSymbol();

		TypeSymbol* Conflict(unsigned i) const;

		 unsigned NumConflicts() const;
		

		 void AddConflict(TypeSymbol* conflict_symbol);

		  void CompressSpace();

	private:
		friend class ExpandedTypeTable;
		TypeShadowSymbol* next;

		ConvertibleArray<TypeSymbol*>* conflict;

		bool Find(const TypeSymbol* conflict_symbol);
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

