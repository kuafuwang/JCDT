
#ifndef DirectoryTable_INCLUDED
#define DirectoryTable_INCLUDED


#include <boost/unordered/unordered_map.hpp>
namespace Jikes { // Open namespace Jikes block
	class DirectoryEntry;
	class DirectorySymbol;


	class DirectoryTable
	{
	public:
		boost::unordered_map<std::string, DirectoryEntry*> entry_pool;
		DirectoryTable(int estimate = 1024);
		~DirectoryTable();

		DirectoryEntry* FindEntry(const char*, int);
		DirectoryEntry* InsertEntry(DirectorySymbol*, const char*, int);

#ifdef WIN32_FILE_SYSTEM
		//
		// See FoldedDirectoryEntry for an explanation of the use of this function
		//
		DirectoryEntry* FindCaseInsensitiveEntry(const char*, int);
		void InsertCaseInsensitiveEntry(DirectoryEntry*);
#endif

	private:
		enum
		{
			DEFAULT_HASH_SIZE = 1021,
			MAX_HASH_SIZE = 8191
		};
		static unsigned primes[];
	
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

