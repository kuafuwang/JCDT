#ifndef LiteralLookupTable_jikes_INCLUDED
#define LiteralLookupTable_jikes_INCLUDED

#include <JCDT_Lib/internal/util/tuple.h>
#include "Hash.h"

namespace Jikes { // Open namespace Jikes block

class LiteralSymbol;

class LiteralLookupTable
{
public:
    Tuple<LiteralSymbol*> symbol_pool;

    LiteralLookupTable();
    ~LiteralLookupTable();

    LiteralSymbol* FindOrInsertLiteral(const wchar_t*, unsigned);

private:
    enum
    {
        DEFAULT_HASH_SIZE = 1021,
        MAX_HASH_SIZE = 8191
    };

    LiteralSymbol** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    inline static unsigned Hash(const wchar_t* head, int len)
    {
        return Hash::Function(head, len);
    }

    void Rehash();
};


} // Close namespace Jikes block


#endif // _INCLUDED

