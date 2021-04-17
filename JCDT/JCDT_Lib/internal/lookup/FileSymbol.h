#ifndef FileSymbolsymbol_jikes_INCLUDED
#define FileSymbolsymbol_jikes_INCLUDED

#include <JCDT_Lib/internal/impl/FileBase.h>
#include "Symbol.h"
#include <JCDT_Lib/internal/util/tuple.h>
#include <vector>


namespace Jikes {// Open namespace Jikes block
	class DirectorySymbol;
	class LookupEnvironment;
	class PathSymbol;
	class TypeSymbol;
	class PackageSymbol;
	class Semantic;
	class CompilationUnitDeclaration;
	class LexStream;
	class ParseError;
	class JikesError;
	class TypeHeaderInfo;
class FileSymbol : public Symbol,public FileBase
{
private:
   
    DirectorySymbol* output_directory;
    char* file_name;
    unsigned file_name_length;
    Utf8LiteralValue* file_name_literal;

public:


	static FileSymbol* FileCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<FileSymbol*>(symbol->Kind() == Symbol::_FILE ? symbol : NULL);
	}

	static const FileSymbol* FileCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const FileSymbol*>(symbol->Kind() == Symbol::_FILE ? symbol : NULL);
	}


    const  NameSymbol* name_symbol;
    DirectorySymbol* directory_symbol;
    PackageSymbol* package;
    
	
    CompilationUnitDeclaration* compilation_unit;

    Semantic* semantic;

	ParseError* parser_error = nullptr;

    vector<TypeSymbol*> types;
	void RelieveRelathioshipWithType(TypeSymbol*);
	void getTypesInfo(set<wstring>&, vector<TypeHeaderInfo*>&);

	bool initial_invocation = false;
	FileSymbol(const NameSymbol* name_symbol_);
	
    virtual ~FileSymbol();

	FileSymbol* Clone();
	virtual wstring getNameString() const
	{
		if (name_symbol && name_symbol->Name()) {
			return wstring(name_symbol->Name(), name_symbol->NameLength());
		}
		else
			return wstring();
	}
    virtual const wchar_t* Name() const { return name_symbol -> Name(); }
    virtual unsigned NameLength() const { return name_symbol -> NameLength(); }
    virtual const NameSymbol* Identity() const { return name_symbol; }
	const char* Utf8Name() const;
	unsigned Utf8NameLength() const;
	
	

	PathSymbol* PathSym();
	virtual bool IsZip();
	Zip* Zipfile();

	wstring GetUnicodeFileName()override;
	const char* FileName() override;
	 unsigned FileNameLength() override;

	 Utf8LiteralValue* FileNameLiteral();

    void SetFileNameLiteral(LookupEnvironment*);

    DirectorySymbol* OutputDirectory();

    void SetFileName();
	void SetFileName(const char* _name,unsigned _length);

    void CleanUp();

	void Reset();
	void TransferError(vector<JikesError*>&);
	void GetImportPackgeNames(vector<wstring>&);
	unsigned NumErrors() const;
	void RemoveClassFileFromOutputDirWhenInJavaFile();
	
};

inline void FileSymbol::Reset()
{
	CleanUp();

	delete[] file_name;
	file_name = NULL;
	types.clear();
}

inline Utf8LiteralValue* FileSymbol::FileNameLiteral()
{
	assert(file_name_literal);
	return file_name_literal;
}


inline FileSymbol* FileSymbol::Clone()
{
	FileSymbol* clone = new FileSymbol(name_symbol);

	clone->kind = kind;
	clone->directory_symbol = directory_symbol;
	clone->mtime = mtime;
	return clone;
}

inline const char* FileSymbol::Utf8Name() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned FileSymbol::Utf8NameLength() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

} // Close namespace Jikes block


#endif // symbol_INCLUDED

