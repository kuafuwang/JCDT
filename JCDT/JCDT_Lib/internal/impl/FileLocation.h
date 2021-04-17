
#ifndef FileLocation_INCLUDED
#define FileLocation_INCLUDED
#include <JCDT_Lib/internal/env/platform.h>


namespace Jikes {
	class Token;
	class FileBase;
	class LexStream;

	// Open namespace Jikes block


	class FileLocation
	{
	public:
		wchar_t* location;

		FileLocation(LexStream* lex_stream, Token* token_index);

		FileLocation(FileBase* file_symbol);

		~FileLocation()
		{
			delete[] location;
		}
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

