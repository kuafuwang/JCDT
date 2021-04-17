
#ifndef NameLookupTable_INCLUDED
#define NameLookupTable_INCLUDED

#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

	class NameSymbol;

class NameLookupTable
{
public:
    Tuple<NameSymbol*> symbol_pool;

    NameLookupTable(int estimate = 16384);
    ~NameLookupTable();

    NameSymbol* FindOrInsertName(const wchar_t*, unsigned);

private:
    enum
    {
        DEFAULT_HASH_SIZE = 4093,
        MAX_HASH_SIZE = 32771
    };

    NameSymbol** base;
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

