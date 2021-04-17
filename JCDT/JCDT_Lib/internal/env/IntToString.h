
#ifndef IntToString_Jikes_INCLUDED
#define IntToString_Jikes_INCLUDED
#include "typedef.h"

namespace Jikes { // Open namespace Jikes block

				  //
				  // Convert an integer to its character string representation.
				  //
	class IntToString
	{
	public:
		IntToString(i4); // Signed decimal conversion.
		IntToString(u4, int width); // Unsigned zero-padded hexadecimal.

		const char* String() { return str; }
		int Length() { return (&info[TAIL_INDEX]) - str; }

	private:
		enum { TAIL_INDEX = 1 + 10 }; // 1 for sign, +10 significant digits

		char info[TAIL_INDEX + 1]; // +1 for '\0'
		char* str;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

