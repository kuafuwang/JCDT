#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/LiteralLookupTable.h"
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
namespace Jikes { // Open namespace Jikes block




	unsigned LiteralLookupTable::primes[] = {
		DEFAULT_HASH_SIZE, 2039, 4093, MAX_HASH_SIZE
	};

	LiteralLookupTable::LiteralLookupTable()
		: symbol_pool(16384),
		hash_size(primes[0]),
		prime_index(0)
	{
		base = (LiteralSymbol**)memset(new LiteralSymbol*[hash_size], 0,
			hash_size * sizeof(LiteralSymbol*));
	}

	LiteralLookupTable::~LiteralLookupTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	void LiteralLookupTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (LiteralSymbol**)memset(new LiteralSymbol*[hash_size], 0,
			hash_size * sizeof(LiteralSymbol*));

		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			LiteralSymbol* ls = symbol_pool[i];
			int k = ls->hash_address % hash_size;
			ls->next = base[k];
			base[k] = ls;
		}
	}


	LiteralSymbol* LiteralLookupTable::FindOrInsertLiteral(const wchar_t* str,
		unsigned len)
	{
		unsigned hash_address = Hash(str, len);
		int k = hash_address % hash_size;
		LiteralSymbol* symbol;
		for (symbol = base[k]; symbol; symbol = (LiteralSymbol*)symbol->next)
		{
			if (hash_address == symbol->hash_address &&
				len == symbol->NameLength() &&
				memcmp(symbol->Name(), str, len * sizeof(wchar_t)) == 0)
			{
				return symbol;
			}
		}

		symbol = new LiteralSymbol();
		symbol_pool.Next() = symbol;
		symbol->Initialize(str, hash_address, len);

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



