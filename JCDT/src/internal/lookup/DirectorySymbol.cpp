#include <JCDT_Lib/internal/util/case.h>
#include "JCDT_Lib/internal/lookup/DirectorySymbol.h"
#include "JCDT_Lib/internal/lookup/PathSymbol.h"	  
#include "JCDT_Lib/internal/lookup/DirectoryEntry.h"
#include "JCDT_Lib/internal/lookup/DirectoryTable.h"
#include "JCDT_Lib/internal/lookup/FileSymbol.h"
#include "JCDT_Lib/internal/lookup/BindingSymbolTable.h"
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/filesystem/operations.hpp>

namespace Jikes { // Open namespace Jikes block



	BindingSymbolTable* DirectorySymbol::Table()
	{
		return table ? table : table = new BindingSymbolTable(101);
	}



	FileSymbol* DirectorySymbol::FindFileSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindFileSymbol(name_symbol)
			: (FileSymbol*)NULL;
	}



	FileSymbol* DirectorySymbol::InsertFileSymbol(const NameSymbol* name_symbol)
	{
		return Table()->InsertFileSymbol(name_symbol);
	}

	void DirectorySymbol::InsertFileSymbol(FileSymbol* file_symbol)
	{
		return Table()->InsertFileSymbol(file_symbol);
	}

	DirectorySymbol* DirectorySymbol::InsertDirectorySymbol(const NameSymbol* name_symbol,
		bool source_dir)
	{
		DirectorySymbol* subdirectory_symbol =
			Table()->InsertDirectorySymbol(name_symbol, this,
				source_dir && source_dir_only);
		subdirectories.Next() = subdirectory_symbol;
		return subdirectory_symbol;
	}

	DirectorySymbol* DirectorySymbol::FindDirectorySymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindDirectorySymbol(name_symbol)
			: (DirectorySymbol*)NULL;
	}

	PathSymbol* DirectorySymbol::PathSym()
	{
		PathSymbol* temp = PathSymbol::PathCast(owner);// PathSymbol* temp = owner->PathCast();
		return  temp ? (PathSymbol*)owner
			: ((DirectorySymbol*)owner)->PathSym();
	}

	bool DirectorySymbol::IsZip() { return PathSym()->IsZip(); }

#ifdef WIN32_FILE_SYSTEM
	DirectoryEntry* DirectorySymbol::FindCaseInsensitiveEntry(char* name, unsigned length)
	{
		return entries ? entries->FindCaseInsensitiveEntry(name, length)
			: (DirectoryEntry*)NULL;
	}

	void DirectorySymbol::InsertEntry(char* name, unsigned length)
	{
		assert(entries);

		DirectoryEntry* entry = entries->InsertEntry(this, name, length);
		entries->InsertCaseInsensitiveEntry(entry);
	}
#endif // WIN32_FILE_SYSTEM

	DirectorySymbol::DirectorySymbol(const NameSymbol* name_symbol_,
		Symbol* owner_, bool source_dir_only_)
		: owner(owner_)
		, name_symbol(name_symbol_)
		, mtime(0)
		, table(NULL)
		, entries(NULL)
		, directory_name()
		, source_dir_only(source_dir_only_)
	{
		Symbol::_kind = _DIRECTORY;
	}

	DirectoryEntry* DirectorySymbol::FindEntry(char* name, unsigned len)
	{
		DirectoryEntry* find_it = nullptr;
		if(entries) {
			find_it = entries->FindEntry(name, len);
			if (find_it) {
				return find_it;
			}
		}
		else{
			entries = new DirectoryTable();
		}
		auto _w_dir = DirectoryNameString();
		_w_dir += string(name,name + len);
		using namespace boost::filesystem;
		auto file =  path(CharOperation::ConvertUtf8ToUnicode(_w_dir));
		boost::system::error_code ec;
		if (!exists(file,ec)) {
			return nullptr;
		}
	
		bool is_java = false;
		auto _ext = file.extension().wstring();
		if (_ext == L".java") {
			is_java = true;
		}
		else if (_ext == L".class") {
		}
		else {
			return nullptr;
		}
	
		auto _clean_name = file.filename().string();
		DirectoryEntry* entry =
			entries->InsertEntry(this, _clean_name.c_str(), _clean_name.size());
		if (!is_java)
			entries->InsertCaseInsensitiveEntry(entry);
	
		return entry;
	}


	DirectorySymbol::~DirectorySymbol()
	{
		
		delete entries;
		delete table;
	}

	void DirectorySymbol::SetDirectoryName()
	{
		//PathSymbol* path_symbol = owner->PathCast();
		PathSymbol* path_symbol = PathSymbol::PathCast(owner);
		if (path_symbol)
		{
			auto __utf_name_string = Utf8NameString();
			if (__utf_name_string ==  ".")
			{
				directory_name.swap(__utf_name_string);
			}
			else
			{
				auto directory_name_length = path_symbol->Utf8NameLength();
				auto _name = path_symbol->Utf8Name();
				directory_name.assign(_name, _name + directory_name_length);
			
			}
		}
		else
		{
			DirectorySymbol* owner_directory = DirectorySymbol::DirectoryCast(owner);  // DirectorySymbol* owner_directory = owner->DirectoryCast();

			if (Name()[NameLength() - 1] == U_SLASH ||
				strcmp(owner_directory->DirectoryName(), ".") == 0)
			{
				// An absolute file name, or is the owner "." ?	
				directory_name = Utf8NameString();		
			}
			else
			{
				int owner_length = owner_directory->DirectoryNameLength();
				auto owner_name = owner_directory->DirectoryName();
				auto directory_name_length = owner_length + Utf8NameLength() +
					(owner_name[owner_length - 1] != U_SLASH ? 1 : 0); // +1: '/'

				auto __utf_name_string = Utf8NameString();
				directory_name.assign(owner_name, owner_name + owner_length);
				
				if (owner_name[owner_length - 1] != U_SLASH)
					directory_name.append(StringConstant::U8S_SL);
				directory_name += __utf_name_string;	
			}
		}

		
	}


	void DirectorySymbol::ResetDirectory()
	{
		delete entries;
		entries = NULL;
		ReadDirectory();
	}


	void DirectorySymbol::ReadDirectory()
	{
		assert(!IsZip());
		if (!entries){
			entries = new DirectoryTable();
			using namespace boost::filesystem;
			auto _w_dir =   CharOperation::ConvertUtf8ToUnicode(DirectoryNameString());
			boost::system::error_code ec;
			if(!is_directory(_w_dir , ec)){
				return;
			}
			directory_iterator end;
			for (directory_iterator pos(_w_dir); pos != end; ++pos) {
				if (is_directory(*pos, ec) == false) {
					path file(*pos);
					bool is_java = false;
					auto _ext = file.extension().wstring();
					if (_ext == L".java"){
						is_java = true;
					}
					else if (_ext == L".class") {
					}
					else{
						continue;
					}
					auto _clean_name =  file.filename().string();
					DirectoryEntry* entry =
						entries->InsertEntry(this, _clean_name.c_str(), _clean_name.size());
					if (!is_java)
						entries->InsertCaseInsensitiveEntry(entry);
				}
			}
		}
	}




} // Close namespace Jikes block



