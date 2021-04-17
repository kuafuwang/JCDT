
#ifndef LongToString_Jikes_INCLUDED
#define LongToString_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

				  //
				  // Convert a Long integer to its character string representation.
				  //
	class BaseLong;
	class ULongInt;
	class LongInt;
	class LongToString
	{
	public:
		LongToString(const LongInt&); // Signed decimal conversion.
		LongToString(const ULongInt&); // Unsigned decimal conversion.
									   // Unsigned hexadecimal or octal, with optional base designator.
		LongToString(const BaseLong&, bool octal = false);
		
		const char* String() { return str; }
		const char* StringWithBase() { return base; }
		int Length() { return (&info[TAIL_INDEX]) - str; }

	private:
		enum { TAIL_INDEX = 23 }; // 22 octal digits + base designator

		char info[TAIL_INDEX + 1]; // +1 for '\0'
		char* str;
		char* base;
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

