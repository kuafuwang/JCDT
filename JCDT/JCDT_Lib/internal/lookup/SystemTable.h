
#ifndef SystemTable_INCLUDED
#define SystemTable_INCLUDED

#include <JCDT_Lib/internal/util/tuple.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
namespace Jikes { // Open namespace Jikes block



	class SystemTable
	{
		enum
		{
			DEFAULT_HASH_SIZE = 13,
			MAX_HASH_SIZE = 1021
		};

	public:

		SystemTable(unsigned = DEFAULT_HASH_SIZE);
		virtual ~SystemTable();

		DirectorySymbol* FindDirectorySymbol(dev_t, ino_t);
		void InsertDirectorySymbol(dev_t, ino_t, DirectorySymbol*);

	private:
		class Element
		{
		public:
			Element(dev_t device_, ino_t inode_,
				DirectorySymbol* directory_symbol_)
				: device(device_),
				inode(inode_),
				directory_symbol(directory_symbol_)
			{}

			Element* next;
			dev_t device;
			ino_t inode;
			DirectorySymbol* directory_symbol;
		};

		Tuple<Element*> directories;

		Element** base;
		unsigned hash_size;

		static unsigned primes[];
		int prime_index;

		unsigned hash(dev_t device, ino_t inode)
		{
			return (device + inode) % hash_size;
		}

		void Rehash();
	};



} // Close namespace Jikes block


#endif // _INCLUDED

