#ifndef UnitFileSymbol_jikes_INCLUDED
#define UnitFileSymbol_jikes_INCLUDED
#include <string>
#include "FileSymbol.h"
#include <JCDT_Lib/internal/util/CharOperation.h>

namespace Jikes { // Open namespace Jikes block





		class UnitFileSymbol :public FileSymbol
		{
		public:
			UnitFileSymbol(const NameSymbol* name_symbol_, string& _fileName)
				: FileSymbol(name_symbol_), fileName(_fileName)
			{
			}

			const char* FileName() override {
				return fileName.c_str();
			};
			unsigned FileNameLength() override {
				return fileName.size();
			};
			bool IsZip() override {
				return false;
			};
			wstring GetUnicodeFileName()
			{
					return CharOperation::ConvertUtf8ToUnicode(fileName);
				
			}

			std::string fileName;
		};


}

#endif
