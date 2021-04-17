
#ifndef FloatLiteralTable_INCLUDED
#define FloatLiteralTable_INCLUDED
#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

	class FloatLiteralValue;
	class LiteralSymbol;

class FloatLiteralTable
{
public:
    Tuple<FloatLiteralValue*> symbol_pool;

    FloatLiteralTable(LiteralValue*);
    ~FloatLiteralTable();

    LiteralValue* FindOrInsertFloat(LiteralSymbol*);

    FloatLiteralValue* FindOrInsert(IEEEfloat);

#ifdef JIKES_DEBUG
    //
    // To prevent arithmetic conversion to allow illegal calls inadvertently.
    //
    void FindOrInsert(int) {}
    void FindOrInsert(LongInt) {}
    void FindOrInsert(double) {}
#endif

private:

    enum
    {
        DEFAULT_HASH_SIZE = 1021,
        MAX_HASH_SIZE = 8191
    };

    FloatLiteralValue** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    LiteralValue* bad_value;

    inline static unsigned Hash(IEEEfloat value)
    {
        return Hash::Function(value);
    }

    void Rehash();
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

