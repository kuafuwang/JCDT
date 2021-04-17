#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/NameLookupTable.h"
#include "JCDT_Lib/internal/impl/NameSymbol.h"

namespace Jikes { // Open namespace Jikes block



	unsigned NameLookupTable::primes[] = {
		DEFAULT_HASH_SIZE, 8191, 16411, MAX_HASH_SIZE
	};

	NameLookupTable::NameLookupTable(int estimate)
		: symbol_pool(estimate),
		hash_size(primes[0]),
		prime_index(0)
	{
		base = (NameSymbol**)memset(new NameSymbol*[hash_size], 0,
			hash_size * sizeof(NameSymbol*));
	}

	NameLookupTable::~NameLookupTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	void NameLookupTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (NameSymbol**)memset(new NameSymbol*[hash_size], 0,
			hash_size * sizeof(NameSymbol*));

		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			NameSymbol* ns = symbol_pool[i];
			int k = ns->hash_address % hash_size;
			ns->next = base[k];
			base[k] = ns;
		}
	}


	NameSymbol* NameLookupTable::FindOrInsertName(const wchar_t* str, unsigned len)
	{
		unsigned hash_address = Hash(str, len);
		int k = hash_address % hash_size;
		NameSymbol* symbol;
		for (symbol = base[k]; symbol; symbol = (NameSymbol*)symbol->next)
		{
			if (hash_address == symbol->hash_address &&
				len == symbol->NameLength() &&
				memcmp(symbol->Name(), str, len * sizeof(wchar_t)) == 0)
			{
				return symbol;
			}
		}

		int index = symbol_pool.Length(); // index of the next element
		symbol = new NameSymbol();
		symbol_pool.Next() = symbol;
		symbol->Initialize(str, len, hash_address, index);

		symbol->next = base[k];
		base[k] = symbol;

		//
		// If the number of unique elements in the hash table exceeds 2 times
		// the size of the base, and we have not yet reached the maximum
		// allowable size for a base, reallocate a larger base and rehash
		// the elements.
		//
		if (symbol_pool.Length() > (hash_size << 1) && hash_size < MAX_HASH_SIZE)
			Rehash();
		return symbol;
	}


} // Close namespace Jikes block



