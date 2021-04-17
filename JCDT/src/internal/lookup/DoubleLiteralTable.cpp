#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
#include <JCDT_Lib/internal/lookup/DoubleLiteralTable.h>

namespace Jikes { // Open namespace Jikes block


	unsigned DoubleLiteralTable::primes[] = {
		DEFAULT_HASH_SIZE, 2039, 4093, MAX_HASH_SIZE
	};

	DoubleLiteralTable::DoubleLiteralTable(LiteralValue* bad_value_)
		: symbol_pool(16384),
		hash_size(primes[0]),
		prime_index(0),
		bad_value(bad_value_)
	{
		base = (DoubleLiteralValue**)memset(new DoubleLiteralValue*[hash_size], 0,
			hash_size * sizeof(DoubleLiteralValue*));
		symbol_pool.Next() = NULL; // do not use the 0th element
	}

	DoubleLiteralTable::~DoubleLiteralTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	LiteralValue* DoubleLiteralTable::FindOrInsertDouble(LiteralSymbol* literal)
	{
		char* name = new char[literal->NameLength() + 1];
		for (unsigned i = 0; i < literal->NameLength(); i++)
			name[i] = (char) literal->Name()[i];
		name[literal->NameLength()] = U_NULL;

		//
		// JLS 3.10.2 states it is an error for a literal to round to infinity or 0
		// Passing the second parameter tells the constructor to set value to NaN
		// if the literal is invalid.
		//
		IEEEdouble value = IEEEdouble(name, true);

		literal->value = (value.IsNaN() ? bad_value : FindOrInsert(value));

		delete[] name;
		return literal->value;
	}


	void DoubleLiteralTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (DoubleLiteralValue**)memset(new DoubleLiteralValue*[hash_size], 0,
			hash_size * sizeof(DoubleLiteralValue*));

		//
		// Recall that the 0th element is unused.
		//
		for (unsigned i = 1; i < symbol_pool.Length(); i++)
		{
			DoubleLiteralValue* dlv = symbol_pool[i];
			// The hash function for double values is cheap so we don't need to
			// save it.
			int k = Hash(dlv->value) % hash_size;
			dlv->next = base[k];
			base[k] = dlv;
		}
	}


	DoubleLiteralValue* DoubleLiteralTable::FindOrInsert(IEEEdouble value)
	{
		int k = Hash(value) % hash_size;

		DoubleLiteralValue* lit;
		for (lit = base[k]; lit; lit = (DoubleLiteralValue*)lit->next)
		{
			if (lit->value.equals(value))
				return lit;
		}

		lit = new DoubleLiteralValue();
		lit->Initialize(value, symbol_pool.Length());
		symbol_pool.Next() = lit;

		lit->next = base[k];
		base[k] = lit;

		//
		// If the number of unique elements in the hash table exceeds 2 times
		// the size of the base, and we have not yet reached the maximum
		// allowable size for a base, reallocate a larger base and rehash
		// the elements.
		//
		if (symbol_pool.Length() > (hash_size << 1) && hash_size < MAX_HASH_SIZE)
			Rehash();
		return lit;
	}

	

} // Close namespace Jikes block



