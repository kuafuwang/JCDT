
#ifndef LongLiteralTable_INCLUDED
#define LongLiteralTable_INCLUDED
#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>

namespace Jikes { // Open namespace Jikes block

	class LongLiteralValue;
	class LiteralSymbol;

class LongLiteralTable
{
public:
    Tuple<LongLiteralValue*> symbol_pool;

    LongLiteralTable(LiteralValue*);
    ~LongLiteralTable();

    LiteralValue* FindOrInsertLong(LiteralSymbol*);
    LiteralValue* FindOrInsertHexLong(LiteralSymbol*);
    LiteralValue* FindOrInsertOctalLong(LiteralSymbol*);
    LiteralValue* FindOrInsertNegativeLong(LiteralSymbol*);

    LongLiteralValue* FindOrInsert(LongInt);

#ifdef JIKES_DEBUG
    //
    // To prevent arithmetic conversion to allow illegal calls inadvertently.
    //
    void FindOrInsert(int) {}
    void FindOrInsert(float) {}
    void FindOrInsert(double) {}
#endif

private:

    enum
    {
        DEFAULT_HASH_SIZE = 1021,
        MAX_HASH_SIZE = 8191
    };

    LongLiteralValue** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    static LongInt int64_limit;

    LiteralValue* bad_value;

    inline static unsigned Hash(LongInt value)
    {
        return Hash::Function(value);
    }

    void Rehash();
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

