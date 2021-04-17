#include  <JCDT_Lib/internal/lookup/MethodShadowSymbol.h>
#include  <JCDT_Lib/internal/lookup/MethodSymbol.h>

namespace Jikes { // Open namespace Jikes block




	MethodShadowSymbol::MethodShadowSymbol(MethodSymbol* method_symbol_)
		: method_symbol(method_symbol_), next_method(nullptr), next(nullptr),
		conflict(NULL)
	{}

	MethodShadowSymbol::~MethodShadowSymbol()
	{
		delete conflict;
	}

	MethodSymbol* MethodShadowSymbol::Conflict(unsigned i) const { return (*conflict)[i]; }

	 unsigned MethodShadowSymbol::NumConflicts() const
	{
		return conflict ? conflict->Length() : 0;
	}
	 void MethodShadowSymbol::AddConflict(MethodSymbol* conflict_symbol)
	{
		if (method_symbol != conflict_symbol && !Find(conflict_symbol))
			conflict->Next() = conflict_symbol;
	}

	 void MethodShadowSymbol::CompressSpace()
	{
		if (conflict)
			conflict->Array();
	}

	bool MethodShadowSymbol::Find(const MethodSymbol* conflict_symbol)
	{
		if (!conflict)
			conflict = new ConvertibleArray<MethodSymbol*>(4);
		for (unsigned k = 0; k < conflict->Length(); k++)
			if ((*conflict)[k] == conflict_symbol)
				return true;
		return false;
	}

} // Close namespace Jikes block



