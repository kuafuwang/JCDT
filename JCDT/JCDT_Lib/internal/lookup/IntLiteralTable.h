
#ifndef IntLiteralTable_INCLUDED
#define IntLiteralTable_INCLUDED
#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>

namespace Jikes {
	class LiteralValue;
	// Open namespace Jikes block


	class LiteralSymbol;
	class IntLiteralValue;
	class LiteralValue;
class IntLiteralTable
{
public:
    Tuple<IntLiteralValue*> symbol_pool;

    IntLiteralTable(LiteralValue*);
    ~IntLiteralTable();

	LiteralValue* FindOrInsertNull();
   

    LiteralValue* FindOrInsertChar(LiteralSymbol*);
    LiteralValue* FindOrInsertInt(LiteralSymbol*);
    LiteralValue* FindOrInsertHexInt(LiteralSymbol*);
    LiteralValue* FindOrInsertOctalInt(LiteralSymbol*);
    LiteralValue* FindOrInsertNegativeInt(LiteralSymbol*);

    IntLiteralValue* FindOrInsert(i4);
    IntLiteralValue* Find(i4);
    // Forwarding methods, if needed, so that Find(0) works.
#ifndef TYPE_I4_IS_INT
    inline IntLiteralValue* FindOrInsert(int i)
    {
        return FindOrInsert((i4) i);
    }
    inline IntLiteralValue* Find(int i) { return Find((i4) i); }
#endif // TYPE_I4_IS_INT

#ifdef JIKES_DEBUG
    //
    // To prevent arithmetic conversion to allow illegal calls inadvertently.
    // Since the return type is wrong, compilation will fail !
    //
    void FindOrInsert(LongInt) {}
    void FindOrInsert(float) {}
    void FindOrInsert(double) {}
#endif

private:
    enum
    {
        DEFAULT_HASH_SIZE = 4093,
        MAX_HASH_SIZE = 32771
    };

    IntLiteralValue** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    static int int32_limit;

    LiteralValue* bad_value;

    void Rehash();
};

	

} // Close namespace Jikes block


#endif // _INCLUDED

