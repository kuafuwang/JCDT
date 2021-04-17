#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
#include <JCDT_Lib/internal/lookup/LongLiteralTable.h>
#include <JCDT_Lib/internal/codegen/code.h>

namespace Jikes { // Open namespace Jikes block



	LongInt LongLiteralTable::int64_limit = LongInt(0x7FFFFFFF, 0xFFFFFFFF) / 10;
	unsigned LongLiteralTable::primes[] = {
		DEFAULT_HASH_SIZE, 2039, 4093, MAX_HASH_SIZE
	};

	LongLiteralTable::LongLiteralTable(LiteralValue* bad_value_)
		: symbol_pool(16384),
		hash_size(primes[0]),
		prime_index(0),
		bad_value(bad_value_)
	{
		base = (LongLiteralValue**)memset(new LongLiteralValue*[hash_size], 0,
			hash_size * sizeof(LongLiteralValue*));
		symbol_pool.Next() = NULL; // do not use the 0th element
	}

	LongLiteralTable::~LongLiteralTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	LiteralValue* LongLiteralTable::FindOrInsertHexLong(LiteralSymbol* literal)
	{
		u4 high = 0;
		u4 low = 0;
		const wchar_t* head = literal->Name() + 1; // point to X
												   // -2 to skip the 'L' suffix
		const wchar_t* tail = &literal->Name()[literal->NameLength() - 2];

		for (++head; tail > head && *head == U_0; head++) // skip leading zeroes
			;
		head--;

		for (int i = 0; i < 32 && tail > head; i += 4, tail--)
			low |= Code::Value(*tail) << i;
		for (int j = 0; j < 32 && tail > head; j += 4, tail--)
			high |= Code::Value(*tail) << j;
		return tail > head ? bad_value : FindOrInsert(LongInt(high, low));
	}


	LiteralValue* LongLiteralTable::FindOrInsertOctalLong(LiteralSymbol* literal)
	{
		const wchar_t* head = literal->Name(); // point to initial '0'
											   // -2 to skip the 'L' suffix
		const wchar_t* tail = &head[literal->NameLength() - 2];

		ULongInt uvalue = 0;
		for (++head; tail > head && *head == U_0; head++) // skip leading zeroes
			;
		head--;

		for (int i = 0; i < 63 && tail > head; i += 3, tail--)
		{
			ULongInt d = (u4)(*tail - U_0);
			uvalue |= (d << i);
		}

		if (tail > head)
		{
			u4 d = *tail - U_0;

			if (d <= 1) // max number that can fit in 1 bit
			{
				tail--;
				uvalue |= ULongInt((d << 31), 0);
			}
		}
		return tail > head ? bad_value : FindOrInsert((LongInt)uvalue);
	}


	LiteralValue* LongLiteralTable::FindOrInsertLong(LiteralSymbol* literal)
	{
		const wchar_t* name = literal->Name();

		//
		// We can assert that the name of a literal contains at least two
		// characters: at least one digit and the terminating '\0'.
		//
		if (name[0] == U_0)
			literal->value = (name[1] == U_x || name[1] == U_X
				? FindOrInsertHexLong(literal)
				: FindOrInsertOctalLong(literal));
		else
		{
			LongInt value = 0;

			const wchar_t* p;
			for (p = name; *p != U_L && *p != U_l; p++)
			{
				u4 digit = *p - U_0;
				if (value > int64_limit || (value == int64_limit && digit > 7))
					break;
				value = value * 10 + digit;
			}

			literal->value = (*p != U_L && *p != U_l ? bad_value
				: FindOrInsert(value));
		}
		return literal->value;
	}


	LiteralValue* LongLiteralTable::FindOrInsertNegativeLong(LiteralSymbol* literal)
	{
		// A positive value already exists.
		if (literal->value && literal->value != bad_value)
		{
			LongLiteralValue* long_literal = (LongLiteralValue*) literal->value;
			return FindOrInsert(-long_literal->value);
		}

		const wchar_t* name = literal->Name();
		//
		// We can assert that the name of a literal contains at least two
		// characters: at least one digit and the terminating '\0'.
		//
		if (name[0] == U_0)
		{
			LongLiteralValue* long_literal =
				(LongLiteralValue*)(name[1] == U_x || name[1] == U_X
					? FindOrInsertHexLong(literal)
					: FindOrInsertOctalLong(literal));
			return FindOrInsert(-long_literal->value);
		}

		LongInt value = 0;

		const wchar_t* p;
		for (p = name; *p != U_L && *p != U_l && value >= 0; p++)
		{
			u4 digit = *p - U_0;
			if (value > int64_limit || (value == int64_limit && digit > 8))
				break;
			value = value * 10 + digit;
		}
		return *p != U_L && *p != U_l ? bad_value : FindOrInsert(-value);
	}


	void LongLiteralTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (LongLiteralValue**)memset(new LongLiteralValue*[hash_size], 0,
			hash_size * sizeof(LongLiteralValue*));

		//
		// Recall that the 0th element is unused.
		//
		for (unsigned i = 1; i < symbol_pool.Length(); i++)
		{
			LongLiteralValue* llv = symbol_pool[i];
			// The hash function for LongInt values is cheap so we don't need to
			// save it.
			int k = Hash(llv->value) % hash_size;
			llv->next = base[k];
			base[k] = llv;
		}
	}


	LongLiteralValue* LongLiteralTable::FindOrInsert(LongInt value)
	{
		int k = Hash(value) % hash_size;

		LongLiteralValue* lit;
		for (lit = base[k]; lit; lit = (LongLiteralValue*)lit->next)
		{
			if (lit->value == value)
				return lit;
		}

		lit = new LongLiteralValue();
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



