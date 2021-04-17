#include "JCDT_Lib/internal/impl/FileLocation.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "JCDT_Lib/internal/impl/FileBase.h"
namespace Jikes { // Open namespace Jikes block


	FileLocation::FileLocation(LexStream* lex_stream, Token* token_index)
	{
		int length = 0;
		if(!token_index || !lex_stream)
		{
			location = new wchar_t[length + 13];
			location[length] = U_NULL;
			return;
		}

		auto  file_symbol = lex_stream->file_symbol;
		auto file_name = file_symbol->GetUnicodeFileName();
		length = file_name.size();
		location = new wchar_t[length + 13];
		for (unsigned i = 0; i < length; i++)
			location[i] = (wchar_t)file_name[i];
		location[length++] = U_COLON;

		IntToWstring line_no(lex_stream->LineFromPos(token_index->start_location));

		for (int j = 0; j < line_no.Length(); j++)
			location[length++] = line_no.String()[j];
		location[length] = U_NULL;
	}
	FileLocation::FileLocation(FileBase* file_symbol)
	{
		auto file_name = file_symbol->GetUnicodeFileName();
		auto length = file_name.size();
		location = new wchar_t[length + 13];
		for (unsigned i = 0; i < length; i++) {
			location[i] = file_name[i];
		}
		location[length] = U_NULL;
	}
	

} // Close namespace Jikes block



