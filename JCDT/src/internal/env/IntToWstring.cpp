#include <JCDT_Lib/internal/env/IntToWstring.h>
#include <JCDT_Lib/internal/env/U_chars.h>

namespace Jikes { // Open namespace Jikes block


	IntToWstring::IntToWstring(i4 num)
	{
		wstr = &winfo[TAIL_INDEX];
		*wstr = U_NULL;
		u4 n = num < 0 ? -num : num;
		do
		{
			*--wstr = U_0 + n % 10;
			n /= 10;
		} while (n != 0);
		if (num < 0)
			*--wstr = U_MINUS;
	}

	

} // Close namespace Jikes block



