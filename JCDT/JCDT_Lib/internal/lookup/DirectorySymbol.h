

#ifndef DirectorySymbolsymbol_INCLUDED
#define DirectorySymbolsymbol_INCLUDED

#include "Symbol.h"
#include "JCDT_Lib/internal/util/tuple.h"

namespace Jikes { // Open namespace Jikes block

	class DirectoryEntry;
	class PathSymbol;
	class FileSymbol;
	class BindingSymbolTable;
	class DirectoryTable;

	class DirectorySymbol : public Symbol
	{
	public:

		static DirectorySymbol* DirectoryCast(Symbol* symbol);

		static const DirectorySymbol* DirectoryCast(const Symbol* symbol);

		Symbol* owner;
		const NameSymbol* name_symbol;

		Tuple<DirectorySymbol*> subdirectories;

		DirectorySymbol(const NameSymbol*, Symbol*, bool source_dir_only);
		virtual ~DirectorySymbol();

		virtual const wchar_t* Name() const;
		virtual unsigned NameLength() const;
		virtual const NameSymbol* Identity() const;

		const char* Utf8Name() const;
		unsigned Utf8NameLength() const;
		string Utf8NameString();
		bool IsSourceDirectory();

		DirectoryEntry* FindEntry(char* name, unsigned len);

#ifdef WIN32_FILE_SYSTEM
		DirectoryEntry* FindCaseInsensitiveEntry(char* name, unsigned length);

		void InsertEntry(char* name, unsigned length);
#endif // WIN32_FILE_SYSTEM

		PathSymbol* PathSym();

		bool IsZip();

		void SetDirectoryName();
		string DirectoryNameString();
		const char* DirectoryName();
		unsigned DirectoryNameLength();

		DirectorySymbol* InsertDirectorySymbol(const NameSymbol*, bool);
		DirectorySymbol* FindDirectorySymbol(const NameSymbol*);

		FileSymbol* InsertFileSymbol(const NameSymbol*);
		void InsertFileSymbol(FileSymbol*);
		FileSymbol* FindFileSymbol(const NameSymbol*);

		void ResetDirectory();

		void ReadDirectory();

	private:

		time_t mtime;

		BindingSymbolTable* table;
		inline BindingSymbolTable* Table();

		DirectoryTable* entries;
		string directory_name;

		//unsigned directory_name_length;

		bool source_dir_only;
	};

	inline const wchar_t* DirectorySymbol::Name() const { return name_symbol->Name(); }
	inline unsigned DirectorySymbol::NameLength() const { return name_symbol->NameLength(); }
	inline const NameSymbol* DirectorySymbol::Identity() const { return name_symbol; }

	inline DirectorySymbol* DirectorySymbol::DirectoryCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<DirectorySymbol*>(symbol->Kind() == Symbol::_DIRECTORY ? symbol : NULL);
	}

	inline  const DirectorySymbol* DirectorySymbol::DirectoryCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const DirectorySymbol*>
			(symbol->Kind() == Symbol::_DIRECTORY ? symbol : NULL);
	}

	inline string DirectorySymbol::DirectoryNameString()
	{
		if (directory_name.empty())
			SetDirectoryName();
		return directory_name;
	}
	inline const char* DirectorySymbol::DirectoryName()
	{
		if (directory_name.empty())
			SetDirectoryName();
		return directory_name.c_str();
	}

	inline unsigned DirectorySymbol::DirectoryNameLength()
	{
		auto directory_name_length = directory_name.size();
		if (!directory_name_length)
		{
			SetDirectoryName();
			directory_name_length = directory_name.size();
		}
			
		return directory_name_length;
	}

	inline const char* DirectorySymbol::Utf8Name() const
	{
		return name_symbol->Utf8_literal
			? name_symbol->Utf8_literal->value : (char*)NULL;
	}
	inline string DirectorySymbol::Utf8NameString() 
	{
		if (name_symbol->Utf8_literal)
			return  name_symbol->Utf8_literal->getString();
			
		return{};
	}
	inline unsigned DirectorySymbol::Utf8NameLength() const
	{
		return name_symbol->Utf8_literal
			? name_symbol->Utf8_literal->length : 0;
	}
	inline bool DirectorySymbol::IsSourceDirectory() { return source_dir_only; }

} // Close namespace Jikes block


#endif // symbol_INCLUDED

