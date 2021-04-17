#include "stdafx.h"
#include "SourceMapperFile.h"
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		wstring SourceMapperFile::GetUnicodeFileName()
		{
			return CharOperation::ConvertUtf8ToUnicode(fileName);
		}

		SourceMapperFile::~SourceMapperFile()
		{
			delete lex_stream;
		}

		SourceMapperFile::SourceMapperFile(const string& _path): fileName(_path)
		{
			
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

