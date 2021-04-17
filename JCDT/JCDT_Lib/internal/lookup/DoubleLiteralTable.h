
#ifndef DoubleLiteralTable_INCLUDED
#define DoubleLiteralTable_INCLUDED

#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block


	class DoubleLiteralValue;
	class LiteralSymbol;
class DoubleLiteralTable
{
public:
    Tuple<DoubleLiteralValue*> symbol_pool;

    DoubleLiteralTable(LiteralValue*);
    ~DoubleLiteralTable();

    LiteralValue* FindOrInsertDouble(LiteralSymbol*);

    DoubleLiteralValue* FindOrInsert(IEEEdouble);

#ifdef JIKES_DEBUG
    //
    // To prevent arithmetic conversion to allow illegal calls inadvertently.
    //
    void FindOrInsert(int) {}
    void FindOrInsert(LongInt) {}
    void FindOrInsert(float) {}
#endif

private:

    enum
    {
        DEFAULT_HASH_SIZE = 1021,
        MAX_HASH_SIZE = 8191
    };

    DoubleLiteralValue** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    LiteralValue* bad_value;

    inline static unsigned Hash(IEEEdouble value)
    {
        return Hash::Function(value);
    }

    void Rehash();
};

	

} // Close namespace Jikes block


#endif // _INCLUDED

