#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/IntLiteralTable.h"
#include <JCDT_Lib/internal/codegen/code.h>
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>

namespace Jikes { // Open namespace Jikes block



	int IntLiteralTable::int32_limit = 0x7FFFFFFF / 10;
	unsigned IntLiteralTable::primes[] = {
		DEFAULT_HASH_SIZE, 8191, 16411, MAX_HASH_SIZE
	};

	IntLiteralTable::IntLiteralTable(LiteralValue* bad_value_)
		: symbol_pool(16384),
		hash_size(primes[0]),
		prime_index(0),
		bad_value(bad_value_)
	{
		base = (IntLiteralValue**)memset(new IntLiteralValue*[hash_size], 0,
			hash_size * sizeof(IntLiteralValue*));
		symbol_pool.Next() = NULL; // do not use the 0th element
	}

	IntLiteralTable::~IntLiteralTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}

	LiteralValue* IntLiteralTable::FindOrInsertNull()
	{
		return FindOrInsert(0);
	}
	LiteralValue* IntLiteralTable::FindOrInsertChar(LiteralSymbol* literal)
	{
		const wchar_t* name = literal->Name() + 1;
		int len = literal->NameLength() - 2; // discard ''

		if (len <= 0) // An isolated or unterminated quote.
			return literal->value = bad_value;
		if (len == 1) // A regular character.
			return literal->value = FindOrInsert((i4)name[0]);

		i4 value = -1;

		if (name[0] == U_BACKSLASH)
			switch (name[1])
			{
			case U_b:
				value = U_BACKSPACE;
				break;
			case U_f:
				value = U_FORM_FEED;
				break;
			case U_n:
				value = U_LINE_FEED;
				break;
			case U_r:
				value = U_CARRIAGE_RETURN;
				break;
			case U_t:
				value = U_HORIZONTAL_TAB;
				break;
			case U_DOUBLE_QUOTE:
			case U_SINGLE_QUOTE:
			case U_BACKSLASH:
				value = name[1];
				break;
			case U_0:
			case U_1:
			case U_2:
			case U_3:
			case U_4:
			case U_5:
			case U_6:
			case U_7:
			{
				value = 0;
				int i = 0;
				while (++i < len)
					value = value * 8 + name[i] - U_0;
			}
			}
		return literal->value = (value < 0 || value > 65535 ? bad_value
			: FindOrInsert(value));
	}


	LiteralValue* IntLiteralTable::FindOrInsertHexInt(LiteralSymbol* literal)
	{
		const wchar_t* head = literal->Name() + 1; // point to X
		const wchar_t* tail = &literal->Name()[literal->NameLength() - 1];

		u4 uvalue = 0;

		for (++head; tail > head && *head == U_0; head++)
			; // skip leading zeroes
		head--;

		for (int i = 0; i < 32 && tail > head; i += 4, tail--)
			uvalue |= Code::Value(*tail) << i;
		return tail > head ? bad_value : FindOrInsert((i4)uvalue);
	}


	LiteralValue* IntLiteralTable::FindOrInsertOctalInt(LiteralSymbol* literal)
	{
		const wchar_t* head = literal->Name(); // point to initial '0'
		const wchar_t* tail = &head[literal->NameLength() - 1];

		u4 uvalue = 0;
		for (++head; tail > head && *head == U_0; head++) // skip leading zeroes
			;
		head--;

		for (int i = 0; i < 30 && tail > head; i += 3, tail--)
		{
			u4 d = *tail - U_0;
			uvalue |= (d << i);
		}

		if (tail > head)
		{
			u4 d = *tail - U_0;

			if (d <= 3) // max number that can fit in 2 bits
			{
				tail--;
				uvalue |= (d << 30);
			}
		}
		return tail > head ? bad_value : FindOrInsert((i4)uvalue);
	}


	LiteralValue* IntLiteralTable::FindOrInsertInt(LiteralSymbol* literal)
	{
		const wchar_t* name = literal->Name();

		if (name[0] == U_0)
			literal->value = (name[1] == U_x || name[1] == U_X
				? FindOrInsertHexInt(literal)
				: FindOrInsertOctalInt(literal));
		else
		{
			i4 value = 0;

			const wchar_t* p;
			for (p = name; *p; p++)
			{
				int digit = *p - U_0;
				if (value > int32_limit || (value == int32_limit && digit > 7))
					break;
				value = value * 10 + digit;
			}

			literal->value = (*p ? bad_value : FindOrInsert(value));
		}
		return literal->value;
	}


	LiteralValue* IntLiteralTable::FindOrInsertNegativeInt(LiteralSymbol* literal)
	{
		if (literal->value && literal->value != bad_value)
		{
			// A positive value already exists.
			IntLiteralValue* int_literal = (IntLiteralValue*) literal->value;
			return FindOrInsert(-int_literal->value);
		}

		const wchar_t* name = literal->Name();

		//
		// We can assert that the name of a literal contains at least two
		// characters: at least one digit and the terminating '\0'.
		//
		if (name[0] == U_0)
		{
			IntLiteralValue* int_literal =
				(IntLiteralValue*)(name[1] == U_x || name[1] == U_X
					? FindOrInsertHexInt(literal)
					: FindOrInsertOctalInt(literal));
			return FindOrInsert(-int_literal->value);
		}

		i4 value = 0;

		const wchar_t* p;
		for (p = name; *p; p++)
		{
			int digit = *p - U_0;
			if (value > int32_limit || (value == int32_limit && digit > 8))
				break;
			value = value * 10 + digit;
		}
		return *p ? bad_value : FindOrInsert(-value);
	}


	void IntLiteralTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (IntLiteralValue**)memset(new IntLiteralValue*[hash_size], 0,
			hash_size * sizeof(IntLiteralValue*));

		//
		// Recall that the 0th element is unused.
		//
		for (unsigned i = 1; i < symbol_pool.Length(); i++)
		{
			IntLiteralValue* ilv = symbol_pool[i];
			// The unsigned casting turns the negative values into positive values.
			int k = ((unsigned)ilv->value) % hash_size;
			ilv->next = base[k];
			base[k] = ilv;
		}
	}


	IntLiteralValue* IntLiteralTable::Find(i4 value)
	{
		// The unsigned casting turns the negative values into positive values.
		int k = ((unsigned)value) % hash_size;

		IntLiteralValue* lit = NULL;
		for (lit = base[k]; lit; lit = (IntLiteralValue*)lit->next)
		{
			if (lit->value == value)
				break;
		}
		return lit;
	}


	IntLiteralValue* IntLiteralTable::FindOrInsert(i4 value)
	{
		// The unsigned casting turns the negative values into positive values.
		int k = ((unsigned)value) % hash_size;

		IntLiteralValue* lit;
		for (lit = base[k]; lit; lit = (IntLiteralValue*)lit->next)
		{
			if (lit->value == value)
				return lit;
		}

		lit = new IntLiteralValue();
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



