#include <JCDT_Lib/internal/env/IntToString.h>
#include <JCDT_Lib/internal/env/U_chars.h>

namespace Jikes { // Open namespace Jikes block




	IntToString::IntToString(i4 num)
	{
		str = &info[TAIL_INDEX];
		*str = U_NULL;
		u4 n = num < 0 ? -num : num;
		do
		{
			*--str = U_0 + n % 10;
			n /= 10;
		} while (n != 0);
		if (num < 0)
			*--str = U_MINUS;
	}

	IntToString::IntToString(u4 num, int width)
	{
		str = &info[width];
		*str = U_NULL;
		do
		{
			char c = num & 0xf;
			*--str = c < 10 ? U_0 + c : U_a - 10 + c;
			num >>= 4;
		} while (str != info);
	}


} // Close namespace Jikes block



