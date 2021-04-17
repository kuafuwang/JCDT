#include <JCDT_Lib/internal/lookup/TypeShadowSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>

namespace Jikes { // Open namespace Jikes block


	TypeShadowSymbol::TypeShadowSymbol(TypeSymbol* type_symbol_)
		: type_symbol(type_symbol_), next(nullptr),
		conflict(NULL)
	{}

	TypeShadowSymbol::~TypeShadowSymbol()
	{
		delete conflict;
	}

	TypeSymbol* TypeShadowSymbol::Conflict(unsigned i) const { return (*conflict)[i]; }

	 unsigned TypeShadowSymbol::NumConflicts() const
	{
		return conflict ? conflict->Length() : 0;
	}

	 void TypeShadowSymbol::AddConflict(TypeSymbol* conflict_symbol)
	{
		if (type_symbol != conflict_symbol && !Find(conflict_symbol))
			conflict->Next() = conflict_symbol;
	}

	 void TypeShadowSymbol::CompressSpace()
	{
		if (conflict)
			conflict->Array();
	}
	bool TypeShadowSymbol::Find(const TypeSymbol* conflict_symbol)
	{
		if (!conflict)
			conflict = new ConvertibleArray<TypeSymbol*>(4);
		for (unsigned k = 0; k < conflict->Length(); k++)
			if ((*conflict)[k] == conflict_symbol)
				return true;
		return false;
	}
} // Close namespace Jikes block



