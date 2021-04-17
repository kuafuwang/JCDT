#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/TypeLookupTable.h"
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>

namespace Jikes { // Open namespace Jikes block



	unsigned TypeLookupTable::primes[] = {
		DEFAULT_HASH_SIZE, 8191, 16411, MAX_HASH_SIZE
	};

	TypeLookupTable::TypeLookupTable(int estimate)
		: symbol_pool(estimate),
		hash_size(primes[0]),
		prime_index(0)
	{
		base = (TypeSymbol**)memset(new TypeSymbol*[hash_size], 0,
			hash_size * sizeof(TypeSymbol*));
	}


	TypeLookupTable::~TypeLookupTable()
	{
		delete[] base;
	}


	void TypeLookupTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (TypeSymbol**)memset(new TypeSymbol*[hash_size], 0,
			hash_size * sizeof(TypeSymbol*));

		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			TypeSymbol* type = symbol_pool[i];
			int k = type->hash_address % hash_size;
			type->next_type = base[k];
			base[k] = type;
		}
	}


	TypeSymbol* TypeLookupTable::FindType(const char* str, int len)
	{
		unsigned hash_address = Hash(str, len);
		int k = hash_address % hash_size;

		for (TypeSymbol* type = base[k]; type; type = type->next_type)
		{
			assert(type->fully_qualified_name);

			Utf8LiteralValue* fully_qualified_name = type->fully_qualified_name;
			if (len == fully_qualified_name->length &&
				memcmp(fully_qualified_name->value, str,
					len * sizeof(char)) == 0)
			{
				return type;
			}
		}
		return NULL;
	}


	void TypeLookupTable::InsertType(TypeSymbol* type)
	{
		assert(type && type->fully_qualified_name);

		unsigned hash_address = Hash(type->fully_qualified_name->value,
			type->fully_qualified_name->length);
		int k = hash_address % hash_size;

#ifdef JIKES_DEBUG
		for (TypeSymbol* t = base[k]; t; t = t->next_type)
			assert(type != t && "Type was already entered in type table");
#endif

		symbol_pool.Next() = type;
		type->hash_address = hash_address;

		type->next_type = base[k];
		base[k] = type;

		//
		// If the number of unique elements in the hash table exceeds 2 times
		// the size of the base, and we have not yet reached the maximum
		// allowable size for a base, reallocate a larger base and rehash
		// the elements.
		//
		if (symbol_pool.Length() > (hash_size << 1) && hash_size < MAX_HASH_SIZE)
			Rehash();
	}


	//
	// Remove all elements from the table.
	//
	void TypeLookupTable::SetEmpty()
	{
		symbol_pool.Reset();
		(void)memset(base, 0, hash_size * sizeof(TypeSymbol*));
	}

	

} // Close namespace Jikes block



