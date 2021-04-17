
#ifndef VariableShadowSymbol_jikes_INCLUDED
#define VariableShadowSymbol_jikes_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

	class VariableSymbol;
	class VariableShadowSymbol
	{
	public:
		VariableSymbol* variable_symbol;

		VariableShadowSymbol(VariableSymbol* variable_symbol_);

		~VariableShadowSymbol();

		VariableSymbol* Conflict(unsigned i) const;

		 unsigned NumConflicts() const;
		 void AddConflict(VariableSymbol* conflict_symbol);
		 void CompressSpace();

	private:
		friend class ExpandedFieldTable;
		VariableShadowSymbol* next;

		ConvertibleArray<VariableSymbol*>* conflict;

		bool Find(const VariableSymbol* conflict_symbol);
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

