
#include <JCDT_Lib/internal/env/LongToString.h>
#include <JCDT_Lib/internal/env/U_chars.h>
#include <JCDT_Lib/internal/impl/long.h>

namespace Jikes { // Open namespace Jikes block


LongToString::LongToString(const LongInt& num)
{
    str = &info[TAIL_INDEX];
    *str = U_NULL;
    ULongInt n = num < 0 ? (ULongInt) - num : (ULongInt) num;
    do
    {
        *--str = U_0 + (n % 10).LowWord();
        n /= 10;
    } while (n != 0);
    if (num.HighWord() & 0x80000000)
        *--str = U_MINUS;
    base = str;
}

LongToString::LongToString(const ULongInt& num)
{
    str = &info[TAIL_INDEX];
    *str = U_NULL;
    ULongInt n = num;
    do
    {
        *--str = U_0 + (n % 10).LowWord();
        n /= 10;
    } while (n != 0);
    base = str;
}

LongToString::LongToString(const BaseLong& num, bool octal)
{
    str = &info[TAIL_INDEX];
    *str = U_NULL;
    ULongInt value = num;
    do
    {
        char c = value.LowWord() & (octal ? 7 : 0xf);
        *--str = c < 10 ? U_0 + c : U_a - 10 + c;
        value >>= (octal ? 3 : 4);
    } while (value != 0);
    base = str - 1;
    if (! octal)
        *base-- = U_x;
    *base = U_0;
}


} // Close namespace Jikes block



