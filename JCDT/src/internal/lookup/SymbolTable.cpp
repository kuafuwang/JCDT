
#include <JCDT_Lib/internal/lookup/SymbolTable.h>
#include <JCDT_Lib/internal/lookup/Symbol.h>

namespace Jikes { // Open namespace Jikes block
	 void SymbolTable::Hash(Symbol* symbol)
	{
		unsigned k = symbol->Identity()->index % hash_size;
		symbol->next = base[k];
		base[k] = symbol;
		//
		// If the set is "adjustable" and the number of unique elements in it
		// exceeds 2 times the size of the base, and we have not yet reached
		// the maximum allowable size for a base, reallocate a larger base and
		// rehash the elements.
		//
		if (Size() > (hash_size << 1) && hash_size < MAX_HASH_SIZE)
			Rehash();
	}

	SymbolTable::SymbolTable(unsigned hash_size_)

	{
		hash_size = (hash_size_ <= 0 ? 1 : hash_size_);

		prime_index = -1;
		do
		{
			if (hash_size < primes[prime_index + 1])
				break;
			prime_index++;
		} while (primes[prime_index] < MAX_HASH_SIZE);

		base = (Symbol**)memset(new Symbol*[hash_size], 0,
			hash_size * sizeof(Symbol*));
	}
	SymbolTable::~SymbolTable()
	{

		delete[] base;
	}


} // Close namespace Jikes block



