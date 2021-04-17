#include  <JCDT_Lib/internal/lookup/ExpandedFieldTable.h>
#include  <JCDT_Lib/internal/lookup/VariableShadowSymbol.h>
#include  <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include  <JCDT_Lib/internal/impl/NameSymbol.h>
namespace Jikes { // Open namespace Jikes block

 void ExpandedFieldTable::CompressSpace()
	{
		hash_size = symbol_pool.Length();
		hash_size = hash_size <= 0 ? 1 : hash_size > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size;
		delete[] base;
		base = (VariableShadowSymbol**)
			memset(new VariableShadowSymbol*[hash_size], 0,
				hash_size * sizeof(VariableShadowSymbol*));

		VariableShadowSymbol** array = symbol_pool.Array();
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			array[i]->CompressSpace();

			unsigned k = array[i]->variable_symbol->name_symbol->index %
				hash_size;
			array[i]->next = base[k];
			base[k] = array[i];
		}
	}

	ExpandedFieldTable::ExpandedFieldTable(unsigned hash_size_ )
		: symbol_pool(10, 4)
	{
		hash_size = hash_size_ <= 0 ? 1 : hash_size_ > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size_;
		base = (VariableShadowSymbol**)
			memset(new VariableShadowSymbol*[hash_size], 0,
				hash_size * sizeof(VariableShadowSymbol*));
	}
	ExpandedFieldTable::~ExpandedFieldTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}

	 VariableShadowSymbol* ExpandedFieldTable::InsertVariableShadowSymbol(VariableSymbol* variable_symbol)
	{
		unsigned i = variable_symbol->name_symbol->index % hash_size;
		VariableShadowSymbol* p = new VariableShadowSymbol(variable_symbol);
		p->next = base[i];
		base[i] = p;
		symbol_pool.Next() = p;

		return p;
	}

	 VariableShadowSymbol* ExpandedFieldTable::FindVariableShadowSymbol(const NameSymbol* name_symbol)
	{
		VariableShadowSymbol* p;
		for (p = base[name_symbol->index % hash_size]; p; p = p->next)
			if (p->variable_symbol->name_symbol == name_symbol)
				break;
		return p;
	}
	

} // Close namespace Jikes block



