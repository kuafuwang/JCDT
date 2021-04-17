
#include <JCDT_Lib/internal/lookup/ExpandedTypeTable.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeShadowSymbol.h>

namespace Jikes { // Open namespace Jikes block

	 void ExpandedTypeTable::CompressSpace()
	{
		hash_size = symbol_pool.Length();
		hash_size = hash_size <= 0 ? 1 : hash_size > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size;
		delete[] base;
		base = (TypeShadowSymbol**)
			memset(new TypeShadowSymbol*[hash_size], 0,
				hash_size * sizeof(TypeShadowSymbol*));

		TypeShadowSymbol** array = symbol_pool.Array();
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			array[i]->CompressSpace();

			unsigned k =
				array[i]->type_symbol->name_symbol->index % hash_size;
			array[i]->next = base[k];
			base[k] = array[i];
		}
	}

	ExpandedTypeTable::ExpandedTypeTable(unsigned hash_size_)
		: symbol_pool(10, 4)
	{
		hash_size = hash_size_ <= 0 ? 1 : hash_size_ > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size_;
		base = (TypeShadowSymbol**)
			memset(new TypeShadowSymbol*[hash_size], 0,
				hash_size * sizeof(TypeShadowSymbol*));
	}

	ExpandedTypeTable::~ExpandedTypeTable()
	{
		for (unsigned k = 0; k < symbol_pool.Length(); k++)
			delete symbol_pool[k];
		delete[] base;
	}

	 TypeShadowSymbol* ExpandedTypeTable::InsertTypeShadowSymbol(TypeSymbol* type_symbol)
	{
		unsigned i = type_symbol->name_symbol->index % hash_size;
		TypeShadowSymbol* p = new TypeShadowSymbol(type_symbol);
		p->next = base[i];
		base[i] = p;
		symbol_pool.Next() = p;

		return p;
	}

	 TypeShadowSymbol* ExpandedTypeTable::FindTypeShadowSymbol(const NameSymbol* name_symbol)
	{
		TypeShadowSymbol* p;
		for (p = base[name_symbol->index % hash_size]; p; p = p->next)
			if (p->type_symbol->name_symbol == name_symbol)
				break;
		return p;
	}

	

} // Close namespace Jikes block



