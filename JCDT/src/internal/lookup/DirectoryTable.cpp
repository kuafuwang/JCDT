#ifndef DirectoryTable_def_INCLUDED
#define DirectoryTable_def_INCLUDED


#include <JCDT_Lib/internal/lookup/Hash.h>
#include <JCDT_Lib/internal/util/case.h>
#include <JCDT_Lib/internal/lookup/DirectoryTable.h>
#include <JCDT_Lib/internal/lookup/DirectoryEntry.h>
#include <JCDT_Lib/internal/lookup/FoldedDirectoryEntry.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>

namespace Jikes { // Open namespace Jikes block

	unsigned DirectoryTable::primes[] = {
		DEFAULT_HASH_SIZE, 2039, 4093, MAX_HASH_SIZE
	};

	DirectoryTable::DirectoryTable(int estimate)
	{
		
		entry_pool.reserve(estimate);
	}

	DirectoryTable::~DirectoryTable()
	{

		for(auto it : entry_pool)
		{
			delete it.second;
		}
	}


	DirectoryEntry* DirectoryTable::FindEntry(const char* str, int len)
	{
		auto find_it = entry_pool.find(std::string(str, len));
		if (find_it != entry_pool.end())
		{
			if (find_it->second->IsDummy())
				return nullptr;
			else
				return find_it->second;
		}
		return nullptr;
	}


	DirectoryEntry* DirectoryTable::InsertEntry(DirectorySymbol* directory_symbol,
		const	char* str, int len)
	{
	
		DirectoryEntry* entry;
		std::string _name_string(str, len);
		
		auto find_it = entry_pool.find(_name_string);
		if (find_it != entry_pool.end())
		{
			return find_it->second;
		}

		entry = new DirectoryEntry();
		entry_pool.insert(make_pair(_name_string, entry));


		entry->Initialize(directory_symbol, str, len);

		return entry;


	}


#ifdef WIN32_FILE_SYSTEM
	DirectoryEntry* DirectoryTable::FindCaseInsensitiveEntry(const char* name,
		int length)
	{
		char* lower_name = new char[length + 1];
		for (int i = 0; i < length; i++)
			lower_name[i] = Case::ToAsciiLower(name[i]);
		lower_name[length] = U_NULL;

		DirectoryEntry* entry = FindEntry(lower_name, length);
		delete[] lower_name;
		return entry ? entry->Image() : entry;
	}

	void DirectoryTable::InsertCaseInsensitiveEntry(DirectoryEntry* image)
	{
		int length = image->length;
		char* lower_name = new char[length + 1];
		for (int i = 0; i < length; i++)
			lower_name[i] = Case::ToAsciiLower(image->name[i]);
		lower_name[length] = U_NULL;



		std::string lower_name_string(lower_name, length);
		DirectoryEntry* entry;
		auto find_it = entry_pool.find(lower_name_string);
		if (find_it != entry_pool.end())
		{
			delete[] lower_name;
			return;
		}
		
		
		FoldedDirectoryEntry* folded_entry = new FoldedDirectoryEntry(image);
			
		folded_entry->Initialize(image, lower_name, length);
		entry_pool.insert(make_pair(lower_name_string, folded_entry));
		

		delete[] lower_name;
	}
#endif // WIN32_FILE_SYSTEM



	time_t DirectoryEntry::Mtime()
	{
		if (mtime_ == 0)
		{
			auto dirname = this->directory->DirectoryName();
			auto direNameLength = this->directory->DirectoryNameLength();
			string file_name(dirname, dirname + direNameLength);
			
		
			if (dirname[this->directory->DirectoryNameLength() - 1] != U_SLASH)
				file_name.append(StringConstant::U8S_SL);

			file_name.append(this->name);
		
			time_t  st_mtime;
			if (FileHelper::getInstance()->LastModifiedTime(file_name, st_mtime))
				mtime_ = st_mtime;
			else
				assert(false && "Cannot compute system time stamp\n");

			
		}
		return mtime_;
	}



} // Close namespace Jikes block


#endif


