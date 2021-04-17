
#ifndef TypeLookupTable_INCLUDED
#define TypeLookupTable_INCLUDED

#include <JCDT_Lib/internal/util/tuple.h>
#include "Hash.h"

namespace Jikes { // Open namespace Jikes block
	class TypeSymbol;

class TypeLookupTable
{
public:
    TypeLookupTable(int estimate = 16384);
    ~TypeLookupTable();

    TypeSymbol* FindType(const char*, int);
    void InsertType(TypeSymbol*);
    void SetEmpty();

private:
    Tuple<TypeSymbol*> symbol_pool;

    enum
    {
        DEFAULT_HASH_SIZE = 4093,
        MAX_HASH_SIZE = 32771
    };

    TypeSymbol** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    inline static unsigned Hash(const char* head, int len)
    {
        return Hash::Function(head, len);
    }

    void Rehash();
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

