
#ifndef Ostream_Jikes_INCLUDED
#define Ostream_Jikes_INCLUDED
#include "typedef.h"
#include "U_chars.h"
#include <ostream>
#include <iostream>

namespace Jikes { // Open namespace Jikes block


	
	class LongInt;
	class ULongInt;
	//
	//
	//
	class Ostream
	{
		std::ostream* os;
		bool expand_wchar;
	
	public:
		void SetLineNo(int no)
		{
			lineNo = no;
		};
		int  lineNo;
		Ostream() : os(&std::cerr),
			expand_wchar(false), lineNo(0)
		{}

		Ostream(std::ostream* _os)
			: os(_os), lineNo(0)
			, expand_wchar(false)
		{}

		void StandardOutput() { os = &std::cout; }
		void StandardError() { os = &std::cerr; }
		void SetExpandWchar(bool val = true) { expand_wchar = val; }
		bool ExpandWchar() { return expand_wchar; }

		virtual Ostream& operator<<(wchar_t ch)
		{
			if(ch == U_LF)
			{
				++lineNo;
			}
			// output only printable characters directly
			if (ch == U_CARRIAGE_RETURN || ch == U_LINE_FEED)
				*os << (char)U_LINE_FEED;
			else if (ch >= U_SPACE && ch < 0x0ff)
				*os << (char)ch;
			else
			{
				if (expand_wchar == 0)
					*os << (char)U_QUESTION;
				else
				{
					*os << (char)U_BACKSLASH
						<< (char)U_u;
					char str[4];
					for (int i = 3; i >= 0; i--)
					{
						int d = ch & 0x0f;
						switch (d)
						{
						case 10: case 11: case 12: case 13: case 14: case 15:
							str[i] = U_A - 10 + d;
							break;
						default:
							str[i] = U_0 + d;
							break;
						}
						ch >>= 4;
					}
					*os << str[0] << str[1] << str[2] << str[3];
				}
			}

			return *this;
		}

		Ostream& operator<<(const wchar_t* str);
		
		Ostream& operator<<(const std::wstring& str);

		virtual Ostream& operator<<(char c);

		virtual Ostream& operator<<(signed char c);

		virtual Ostream& operator<<(unsigned char c);

		virtual Ostream& operator<<(const char* c);

		virtual Ostream& operator<<(const signed char* c);

		virtual Ostream& operator<<(const unsigned char* c);

		virtual 	Ostream& operator<<(int a)
		{
			*os << a;
			return *this;
		}

		virtual Ostream& operator<<(unsigned int a)
		{
			*os << a;
			return *this;
		}

		virtual Ostream& operator<<(long a)
		{
			*os << a;
			return *this;
		}

		virtual Ostream& operator<<(unsigned long a)
		{
			*os << a;
			return *this;
		}

		virtual Ostream& operator<<(LongInt);
		virtual Ostream& operator<<(ULongInt);

		virtual Ostream& operator<<(float f)
		{
			*os << f;
			return *this;
		}

		virtual Ostream& operator<<(double d)
		{
			*os << d;
			return *this;
		}

		char fill(char c) { return os->fill(c); }

		virtual Ostream& flush()
		{
			os->flush();
			return *this;
		}

		virtual std::streamsize width(int w)
		{
			return os->width(w);
		}

		virtual Ostream& operator<<(std::ios& (*func)(std::ios&));

		virtual Ostream& operator<<(std::ostream& (*func)(std::ostream&));
		
	};



	extern Ostream Coutput;

	//
	// From now on, DO NOT USE cout or cerr !
	// Instead, use Coutput, which wraps either cout or cerr as determined by
	// command line flags, and translates unicode output properly.
	
	

} // Close namespace Jikes block


#endif // _INCLUDED

