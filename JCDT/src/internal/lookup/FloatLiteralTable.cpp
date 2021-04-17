#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
#include "JCDT_Lib/internal/lookup/FloatLiteralTable.h"
namespace Jikes { // Open namespace Jikes block



	unsigned FloatLiteralTable::primes[] = {
		DEFAULT_HASH_SIZE, 2039, 4093, MAX_HASH_SIZE
	};

	FloatLiteralTable::FloatLiteralTable(LiteralValue* bad_value_)
		: symbol_pool(16384),
		hash_size(primes[0]),
		prime_index(0),
		bad_value(bad_value_)
	{
		base = (FloatLiteralValue**)memset(new FloatLiteralValue*[hash_size], 0,
			hash_size * sizeof(FloatLiteralValue*));
		symbol_pool.Next() = NULL; // do not use the 0th element
	}

	FloatLiteralTable::~FloatLiteralTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	LiteralValue* FloatLiteralTable::FindOrInsertFloat(LiteralSymbol* literal)
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
		IEEEfloat value = IEEEfloat(name, true);

		literal->value = (value.IsNaN() ? bad_value : FindOrInsert(value));

		delete[] name;
		return literal->value;
	}


	void FloatLiteralTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (FloatLiteralValue**)memset(new FloatLiteralValue*[hash_size], 0,
			hash_size * sizeof(FloatLiteralValue*));

		//
		// Recall that the 0th element is unused.
		//
		for (unsigned i = 1; i < symbol_pool.Length(); i++)
		{
			FloatLiteralValue* flv = symbol_pool[i];
			// The hash function for float values is cheap so we don't need to
			// save it.
			int k = Hash(flv->value) % hash_size;
			flv->next = base[k];
			base[k] = flv;
		}
	}


	FloatLiteralValue* FloatLiteralTable::FindOrInsert(IEEEfloat value)
	{
		int k = Hash(value) % hash_size;

		FloatLiteralValue* lit;
		for (lit = base[k]; lit; lit = (FloatLiteralValue*)lit->next)
		{
			if (lit->value.equals(value))
				return lit;
		}

		lit = new FloatLiteralValue();
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



