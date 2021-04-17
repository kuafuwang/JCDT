
#ifndef Utf8LiteralTable_INCLUDED
#define Utf8LiteralTable_INCLUDED
#include "Hash.h"
#include <JCDT_Lib/internal/util/tuple.h>

namespace Jikes { // Open namespace Jikes block

	class Utf8LiteralValue;
	class LiteralSymbol;
	class Expression;

class Utf8LiteralTable
{
public:
    Tuple<Utf8LiteralValue*> symbol_pool;

    Utf8LiteralTable(LiteralValue*);
    ~Utf8LiteralTable();

    LiteralValue* FindOrInsertString(LiteralSymbol*);

    Utf8LiteralValue* FindOrInsert(const char*, int);
    Utf8LiteralValue* FindOrInsert(wchar_t);

    void CheckStringConstant(Expression* expr);

private:

    Tuple<Utf8LiteralValue*>* utf8_literals;
    Expression* leftmost_constant_expr;
    void CollectStrings();
    bool EndsInKnownString(Expression*);

    enum
    {
        DEFAULT_HASH_SIZE = 4093,
        MAX_HASH_SIZE = 32771
    };

    Utf8LiteralValue** base;
    unsigned hash_size;

    static unsigned primes[];
    int prime_index;

    LiteralValue* bad_value;

    inline static unsigned Hash(const char* head, int len)
    {
        return Hash::Function(head, len);
    }

    void Rehash();
};



} // Close namespace Jikes block


#endif // _INCLUDED

