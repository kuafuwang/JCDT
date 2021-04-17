
#ifndef IntToWstring_Jikes_INCLUDED
#define IntToWstring_Jikes_INCLUDED
#include "typedef.h"

namespace Jikes { // Open namespace Jikes block


				  //
				  // Same as IntToString for wide strings.
				  //
	class IntToWstring
	{
	public:
		IntToWstring(i4);

		const wchar_t* String() { return wstr; }
		int Length() { return (&winfo[TAIL_INDEX]) - wstr; }

	private:
		enum { TAIL_INDEX = 1 + 10 }; // 1 for sign, +10 significant digits

		wchar_t winfo[TAIL_INDEX + 1]; // 1 for sign, +10 significant digits + '\0'
		wchar_t* wstr;
	};



} // Close namespace Jikes block


#endif // _INCLUDED

