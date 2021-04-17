#include <JCDT_Lib/internal/lookup/VariableShadowSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>

namespace Jikes { // Open namespace Jikes block


	VariableShadowSymbol::VariableShadowSymbol(VariableSymbol* variable_symbol_)
		: variable_symbol(variable_symbol_), next(nullptr),
		conflict(NULL)
	{}

	VariableShadowSymbol::~VariableShadowSymbol()
	{
		delete conflict;
	}

	VariableSymbol* VariableShadowSymbol::Conflict(unsigned i) const { return (*conflict)[i]; }

	 unsigned VariableShadowSymbol::NumConflicts() const
	{
		return conflict ? conflict->Length() : 0;
	}
	 void VariableShadowSymbol::AddConflict(VariableSymbol* conflict_symbol)
	{
		if (variable_symbol != conflict_symbol && !Find(conflict_symbol))
			conflict->Next() = conflict_symbol;
	}
	 void VariableShadowSymbol::CompressSpace()
	{
		if (conflict)
			conflict->Array();
	}
	bool VariableShadowSymbol::Find(const VariableSymbol* conflict_symbol)
	{
		if (!conflict)
			conflict = new ConvertibleArray<VariableSymbol*>(4);
		for (unsigned k = 0; k < conflict->Length(); k++)
			if ((*conflict)[k] == conflict_symbol)
				return true;
		return false;
	}

} // Close namespace Jikes block



