
#ifndef CharOperation_INCLUDED
#define CharOperation_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include <vector>
using std::vector;
namespace Jikes { // Open namespace Jikes block

	class CharOperation
	{
	public:
		static bool  endsWith(const wstring& array, const wstring& toBeFound);

		static bool equals(const vector<wstring>& first, const vector<wstring>& second, bool isCaseSensitive);
		static bool equals(const wstring* first, int firstLen, const wstring*  name, int nameLen, bool isCaseSensitive);
		static bool equals(const wstring&first, const wstring&second, bool isCaseSensitive);
		static bool prefixEquals(const wstring& prefix, const wstring& name);
		static bool prefixEquals(const wstring& prefix, const wstring& name, bool isCaseSensitive);

		static bool suffixEquals(const vector<wstring>& suffix, const vector<wstring>& name, bool isCaseSensitive);
		static bool suffixEquals(const wstring* suffix, int suffixLen, const wstring*  name, int nameLen,bool isCaseSensitive);
		static bool suffixEquals(const wstring& suffix, const wstring&  name , bool isCaseSensitive);
		static bool match(const wstring& pattern, const wstring& name, bool isCaseSensitive);
		//
		// Convert the null terminated Unicode string source into its Utf8
		// representation pointed to by target. The char string target is presumed
		// to have been allocated and to be large enough to accomodate the conversion.
		//
		static bool equals(const wchar_t* data_first, size_t first_length, const wchar_t* data_second, size_t second_length,
			bool isCaseSensitive);

		static bool prefixEquals(const wchar_t* prefix_data, size_t prefix_length, const wchar_t* name_data, size_t name_length);
		
		static bool prefixEquals(const wchar_t* prefix_data, size_t prefix_length, const wchar_t* name_data, size_t name_length,
			bool isCaseSensitive);
		
		static bool match(const wchar_t* data_pattern, size_t patternLength, const wchar_t* data_name, size_t nameLength, bool isCaseSensitive);

		static wstring ConvertUtf8ToUnicode(const char* strUtf8);
		static wstring ConvertUtf8ToUnicode(const string& strUtf8);

		static string ConvertUnicodeToUtf8(const wchar_t* strUnicode);
		static string ConvertUnicodeToUtf8(const wstring& strUnicode);

		static	int ConvertUnicodeToUtf8(const wchar_t* source, char* target);
		
		static int  ConvertUtf8ToUnicode(wchar_t* target, const char* source, int len);
	};



	


} // Close namespace Jikes block


#endif // _INCLUDED

