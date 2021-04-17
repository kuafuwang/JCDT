#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/SystemTable.h"
namespace Jikes { // Open namespace Jikes block


	unsigned SystemTable::primes[] = { DEFAULT_HASH_SIZE, 101, 401, MAX_HASH_SIZE };

	SystemTable::SystemTable(unsigned hash_size_)
		: directories(1024)
	{
		hash_size = (hash_size_ <= 0 ? 1 : hash_size_);

		prime_index = -1;
		do
		{
			if (hash_size < primes[prime_index + 1])
				break;
			prime_index++;
		} while (primes[prime_index] < MAX_HASH_SIZE);

		base = (Element**)memset(new Element*[hash_size], 0,
			hash_size * sizeof(Element*));
	}

	DirectorySymbol* SystemTable::FindDirectorySymbol(dev_t device, ino_t inode)
	{
		int k = hash(device, inode);

		for (Element* element = base[k]; element; element = element->next)
		{
			if (element->device == device && element->inode == inode)
				return element->directory_symbol;
		}
		return NULL;
	}

	void SystemTable::InsertDirectorySymbol(dev_t device, ino_t inode,
		DirectorySymbol* directory_symbol)
	{
		int k = hash(device, inode);

		Element* element = new Element(device, inode, directory_symbol);
		directories.Next() = element;

		element->next = base[k];
		base[k] = element;

		//
		// If the set is "adjustable" and the number of unique elements in it
		// exceeds 2 times the size of the base, and we have not yet reached the
		// maximum allowable size for a base, reallocate a larger base and rehash
		// the elements.
		//
		if (directories.Length() > (hash_size << 1) &&
			hash_size < MAX_HASH_SIZE)
		{
			Rehash();
		}
	}

	SystemTable::~SystemTable()
	{
		for (unsigned i = 0; i < directories.Length(); i++)
			delete directories[i];

		delete[] base;
	}

	void SystemTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (Element**)memset(new Element*[hash_size], 0,
			hash_size * sizeof(Element*));

		for (unsigned k = 0; k < directories.Length(); k++)
		{
			Element* element = directories[k];

			int i = hash(element->device, element->inode);
			element->next = base[i];
			base[i] = element;
		}
	}


} // Close namespace Jikes block



