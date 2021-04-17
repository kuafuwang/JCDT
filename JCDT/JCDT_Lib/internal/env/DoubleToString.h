
#ifndef DoubleToString_Jikes_INCLUDED
#define DoubleToString_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

	//
	// Convert an double to its character string representation.
	//
	class IEEEdouble;
	class DoubleToString
	{
	public:
		DoubleToString(const IEEEdouble&);

		const char* String() const { return str; }
		int Length() const { return length; }

	private:
		void Format(char*, int, bool);

		enum
		{
			MAXIMUM_PRECISION = 17,
			// +1 for sign, +17 significant digits +1 for ".", +5 for exponent
			MAXIMUM_STR_LENGTH = 1 + MAXIMUM_PRECISION + 1 + 5
		};

		char str[MAXIMUM_STR_LENGTH + 1]; // +1 for '\0'
		int length;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

