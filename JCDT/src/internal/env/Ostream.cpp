
#include <JCDT_Lib/internal/env/Ostream.h>
#include <JCDT_Lib/internal/env/LongToString.h>
#include <JCDT_Lib/internal/impl/long.h>

namespace Jikes { // Open namespace Jikes block

	Ostream& Ostream::operator<<(signed char c)
	{
		if (c == U_CARRIAGE_RETURN)
		{
			++lineNo;
		}
		*os << c;
		return *this;
	}

	Ostream& Ostream::operator<<(unsigned char c)
	{
		if (c == U_LF)
		{
			++lineNo;
		}
		*os << c;
		return *this;
	}
	Ostream& Ostream::operator<<(const wchar_t* str)
	{
		for (; *str; str++)
			(*this) << *str;
		return *this;
	}

	Ostream& Ostream::operator<<(const std::wstring& str)
	{
		*this << str.data();
		return *this;
	}

	Ostream& Ostream::operator<<(char c)
	{
		if (c == U_LF)
		{
			++lineNo;
		}
		*os << c;
		return *this;
	}

	Ostream& Ostream::operator<<(const char* c)
	{
		lineNo += std::count(c, c + strlen(c), U_LF);
		*os << c;
		return *this;
	}

	Ostream& Ostream::operator<<(const signed char* c)
	{
		lineNo += std::count(c, c + strlen((const char*)c), U_LF);
		*os << c;
		return *this;
	}

	Ostream& Ostream::operator<<(const unsigned char* c)
	{
		lineNo += std::count(c, c + strlen((const char*)c), U_LF);
#ifndef HAVE_OSTREAM_CONST_UNSIGNED_CHAR_PTR
# ifdef HAVE_CONST_CAST
		*os << const_cast<unsigned char*>(c);
# else
			*os << (unsigned char*)c;
# endif
#else
			*os << c;
#endif
		return *this;
	}

	Ostream& Ostream::operator<<(LongInt a)
{
    if (os -> flags() & os -> dec)
    {
        LongToString long_int(a);
        *os << long_int.String();
    }
    else if (os -> flags() & os -> oct)
    {
        LongToString long_int(a, true);
        *os << (os -> flags() & os -> showbase
                ? long_int.StringWithBase() : long_int.String());
    }
    else if (os -> flags() & os -> hex)
    {
        LongToString long_int(a, false);
        *os << (os -> flags() & os -> showbase
                ? long_int.StringWithBase() : long_int.String());
    }
    else
    {
         os -> flush();
         assert(false && "invalid format for printing signed long");
    }

    return *this;
}

Ostream& Ostream::operator<<(ULongInt a)
{
    if (os -> flags() & os -> dec)
    {
        LongToString ulong_int(a);
        *os << ulong_int.String();
    }
    else if (os -> flags() & os -> oct)
    {
        LongToString ulong_int(a, true);
        *os << (os -> flags() & os -> showbase
                ? ulong_int.StringWithBase() : ulong_int.String());
    }
    else if (os -> flags() & os -> hex)
    {
        LongToString ulong_int(a, false);
        *os << (os -> flags() & os -> showbase
                ? ulong_int.StringWithBase() : ulong_int.String());
    }
    else
    {
        os -> flush();
        assert(false && "invalid format for printing unsigned long");
    }

    return *this;
}

	

	Ostream& Ostream::operator<<(std::ios&(*func)(std::ios&))
	 {
		 (*func)(*os);
		 return *this;
	 }
	 Ostream& Ostream::operator<<(std::ostream& (*func)(std::ostream&))
	 {
		 (*func)(*os);
		 if (func == std::endl)
		 {
			 lineNo++;
		 }
		 return *this;
	 }
Ostream Coutput;

	

} // Close namespace Jikes block



