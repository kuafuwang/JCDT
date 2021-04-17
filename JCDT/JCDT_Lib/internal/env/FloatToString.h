
#ifndef FloatToString_Jikes_INCLUDED
#define FloatToString_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block

//
	// Convert an float to its character string representation.
	//
	class IEEEfloat;
	class FloatToString
	{
	public:
		FloatToString(const IEEEfloat&);

		const char* String() const { return str; }
		int Length() const { return length; }

	private:
		void Format(char*, int, bool);

		enum
		{
			MAXIMUM_PRECISION = 9,
			// +1 for sign, +9 significant digits +1 for ".", +4 for exponent
			MAXIMUM_STR_LENGTH = 1 + MAXIMUM_PRECISION + 1 + 4
		};

		char str[MAXIMUM_STR_LENGTH + 1]; // +1 for '\0'
		int length;
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

