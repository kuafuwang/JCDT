
#include <JCDT_Lib/internal/env/DoubleToString.h>
#include <JCDT_Lib/internal/impl/double.h>

namespace Jikes { // Open namespace Jikes block


//
// Convert an double to its character string representation.
//
DoubleToString::DoubleToString(const IEEEdouble& d)
{
    int bbits, b2, b5, be, i,
        j, j1, k, m2, m5, s2, s5;
    bool neg,      // f is negative
        k_check,   // need to check if k is near power of ten
        spec_case, // f is normalized power of two
        denorm,    // f is denormalized
        round;     // round trailing 9's up
    IEEEdouble ds, d1;
    char* s;
    char dig;

    //
    // Start with exceptional cases: zero, infinity, NaN
    //
    neg = d.IsNegative();
    if (d.IsNaN())
    {
        strcpy(str, StringConstant::U8S_NaN);
        length = strlen(str);
        return;
    }
    else if (d.IsInfinite())
    {
        if (neg)
            strcpy(str, StringConstant::U8S_neg_Infinity);
        else
            strcpy(str, StringConstant::U8S_pos_Infinity);
        length = strlen(str);
        return;
    }
    else if (d.IsZero())
    {
        if (neg)
            strcpy(str, StringConstant::U8S_neg_Zero);
        else
            strcpy(str, StringConstant::U8S_pos_Zero);
        length = strlen(str);
        return;
    }

    //
    // Create BigInt holding d.
    // bbits = # significant bits in d
    // be = log2(least significant bit)
    // i = log2(most significant bit)
    // d1 = mantissa of d
    // Therefore, d == d1 * 2**i, and i == be + bbits - 1.
    //
    s = str;
    BigInt b(d, be, bbits);
    LongInt x;
    i = d.SplitInto(x);
    d1 = IEEEdouble(x) /
        IEEEdouble(LongInt(LongInt(1) << IEEEdouble::FractSize()));
    denorm = i <= -IEEEdouble::Bias();
    //
    // log(x)   ~=~ log(1.5) + (x-1.5)/1.5
    // log10(x)  =  log(x) / log(10)
    //          ~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
    // log10(d)  =  log10(d2 * 2**i)
    //           =  i*log10(2) + log10(d2)
    //
    // This suggests computing an approximation k to log10(d) by
    //
    // k = i*0.301029995663981
    //   + ( 0.176091259055681 + (d2-1.5)*0.289529654602168 );
    //
    // We want k to be too large rather than too small.
    // The error in the first-order Taylor series approximation
    // is in our favor, so we just round up the constant enough
    // to compensate for any error in the multiplication of
    // i by 0.301029995663981; since |i| <= 1077,
    // and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
    // adding 1e-13 to the constant term more than suffices.
    // Hence we adjust the constant term to 0.1760912590558.
    // (We could get a more accurate k by invoking log10,
    //  but this is probably not worthwhile.)
    //
    ds = IEEEdouble(i) * 0.301029995663981 + 0.1760912590558
        + (d1 - 1.5) * 0.289529654602168;
    k = ds.IntValue();
    d1 = d.IsNegative() ? -d : d;
    k_check = true;
    if (ds < 0 && ds != k)
        k--;
    else if (k >= 0 && k <= 22)
    {
        if (d1 < IEEEdouble::tens[k])
            k--;
        k_check = false;
    }

    //
    // We have an integer (no fraction) represented in 53 bits.
    // For this special case, math on doubles has no rounding errors.
    //
    if (be >= 0 && k <= 14)
    {
        ds = IEEEdouble::tens[k];
        do
        {
            dig = (char) (d1 / ds).IntValue();
            d1 -= ds * (i4) dig;
            *s++ = U_0 + dig;
        } while ((d1 *= 10) != 0);
        Format(s, k, neg);
        return;
    }

    //
    // Begin work. Find S = 2**s2 * 5**s5, and b adjustment 2**b2 * 5**b5,
    // that will be needed later on.
    //
    if (be <= 0)
    {
        b2 = 0;
        s2 = -be;
    }
    else
    {
        b2 = be;
        s2 = 0;
    }
    if (k >= 0)
    {
        b5 = 0;
        s5 = k;
        s2 += k;
    }
    else
    {
        b2 -= k;
        b5 = -k;
        s5 = 0;
    }

    m2 = b2;
    m5 = b5;
    i = denorm ? be + IEEEdouble::Bias() + IEEEdouble::FractSize()
               : 2 + IEEEdouble::FractSize() - bbits;
    b2 += i;
    s2 += i;
    BigInt mhi(1);
    if (m2 > 0 && s2 > 0)
    {
        i = m2 < s2 ? m2 : s2;
        b2 -= i;
        m2 -= i;
        s2 -= i;
    }
    if (b5 > 0)
    {
        if (m5 > 0)
        {
            mhi.pow5mult(m5);
            b *= mhi;
        }
        if ((j = b5 - m5) != 0)
            b.pow5mult(j);
    }
    BigInt S(1);
    if (s5 > 0)
        S.pow5mult(s5);
    spec_case = false;
    if (! (d.FractBits()) && d.Exponent())
    {
        b2++;
        s2++;
        spec_case = true;
    }

    // Arrange for convenient computation of quotients:
    // shift left if necessary so divisor has 4 leading 0 bits.
    //
    // Perhaps we should just compute leading 28 bits of S once
    // and for all and pass them and a shift to quorem, so it
    // can do shifts and ors to compute the numerator for q.
    //
    if ((i = ((s5 ? 32 - S.hi0bits() : 1) + s2) & 0x1f) != 0)
        i = 32 - i;
    if (i > 4)
    {
        i -= 4;
        b2 += i;
        m2 += i;
        s2 += i;
    }
    else if (i < 4)
    {
        i += 28;
        b2 += i;
        m2 += i;
        s2 += i;
    }
    if (b2 > 0)
        b <<= b2;
    if (s2 > 0)
        S <<= s2;
    if (k_check && b.compareTo(S) < 0)
    {
        k--;
        b *= 10;
        mhi *= 10;
    }
    if (m2 > 0)
        mhi <<= m2;
    BigInt mlo(mhi);
    if (spec_case)
        mhi = mlo << 1;
    round = false;
    while (true)
    {
        dig = (char) b.quorem(S) + U_0;
        //
        // Do we have the shortest decimal string that will round to d?
        //
        j = b.compareTo(mlo);
        BigInt delta = S - mhi;
        j1 = delta.IsNegative() ? 1 : b.compareTo(delta);
        if (j1 == 0 && ! (d.LowWord() & 1))
        {
            if (dig == U_9)
                round = true;
            else if (j > 0)
                dig++;
            *s++ = dig;
            break;
        }
        if ((j < 0 || j == 0 && ! (d.LowWord() & 1)) && s != str)
        {
            if (! b.IsZero() && j1 > 0)
            {
                b <<= 1;
                j1 = b.compareTo(S);
                if ((j1 > 0 || j1 == 0 && dig & 1) && dig++ == U_9)
                {
                    *s++ = U_9;
                    round = true;
                    break;
                }
            }
            *s++ = dig;
            break;
        }
        if (j1 > 0 && s != str)
        {
            if (dig == U_9)
            {
                *s++ = U_9;
                round = true;
            }
            else
                *s++ = dig + 1;
            break;
        }
        *s++ = dig;
        b *= 10;
        mlo *= 10;
        mhi *= 10;
    }
    if (round)
    {
        while (*--s == U_9)
            if (s == str)
            {
                k++;
                *s = U_0;
                break;
            }
        ++*s++;
    }
    Format(s, k, neg);
}

void DoubleToString::Format(char* s, int exp, bool neg)
{
    //
    // at this point, str contains just the precise digits in the answer,
    // and s points to the slot just after the last digit
    //
    length = s - str + 1; // strlen(str) + '.'
    bool eneg;
    int i;
    switch (exp)
    {
    case -3: case -2: case -1:
        // remove final trailing 0, not needed in this format
        if (*(s - 1) == U_0)
        {
            length--;
            s--;
        }
        s--;
        // add enough of leading "0.00"
        length += -exp;
        do
            *(s + (neg ? 2 : 1) - exp) = *s;
        while (s-- != str);
        for (i = (neg ? 1 : 0); i < (neg ? 2 : 1) - exp; i++)
            str[i] = U_0;
        if (neg)
            str[0] = U_MINUS;
        str[neg ? 2 : 1] = U_DOT;
        break;
    case 0: case 1: case 2: case 3:
    case 4: case 5: case 6:
        while (length < exp + 3)
            // add trailing '0's
            str[length++ - 1] = U_0;
        s = &str[length - 2];
        do
            *(s + (neg ? 2 : 1)) = *s;
        while (s-- != str + exp + 1);
        if (neg)
        {
            do
                *(s + 1) = *s;
            while (s-- != str);
            str[0] = U_MINUS;
        }
        str[exp + (neg ? 2 : 1)] = U_DOT;
        break;
    default:
        if (length == 2)
            // add trailing '0', so at least one digit follows '.'
            str[length++ - 1] = U_0;
        eneg = exp < 0;
        if (eneg)
        {
            length++; // exponent '-'
            exp = -exp;
        }
        if (exp < 10)
            length += 2; // 'E' + 1 digit exponent
        else if (exp < 100)
            length += 3; // 'E' + 2 digit exponent
        else if (exp < 1000)
            length += 4; // 'E' + 3 digit exponent
        else
            assert (! "unexpected exponent");
        s = &str[length + (neg ? 1 : 0)];
        do
            *--s = exp % 10 + U_0;
        while ((exp /= 10) != 0);
        if (eneg)
            *--s = U_MINUS;
        *--s = U_E;
        --s;
        do
            *s = *(s - (neg ? 2 : 1)); // shift digits right, to add '.'
        while (--s != str + (neg ? 2 : 1));
        if (neg)
        {
            str[1] = str[0];
            str[0] = U_MINUS;
        }
        str[neg ? 2 : 1] = U_DOT;
    }
    if (neg)
        length++;
    str[length] = U_NULL;
    assert(length <= MAXIMUM_STR_LENGTH);
}

	

} // Close namespace Jikes block



