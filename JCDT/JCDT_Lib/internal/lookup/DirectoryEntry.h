
#ifndef DirectoryEntry_INCLUDED
#define DirectoryEntry_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"

namespace Jikes { // Open namespace Jikes block

	class DirectorySymbol;

	class DirectoryEntry
	{
	public:
		
		char* name;
		int length;

		DirectoryEntry()
			: 
			name(NULL),
			length(0),
			directory(NULL),
			mtime_(0)
		{
			image = this;
		}

		virtual ~DirectoryEntry()
		{
			delete[] name;
		}


		inline void Initialize(DirectorySymbol* directory_,const char* name_,
			int length_)
		{
			directory = directory_;
			length = length_;
			name = new char[length + 1];
			memcpy(name, name_, length * sizeof(char));
			name[length] = U_NULL;
		}

		inline void Initialize(DirectoryEntry* entry, char* name_, int length_)
		{
			Initialize(entry->directory, name_, length_);
		}

		time_t Mtime();

		bool IsDummy() { return this != image; }

		//
		// See FoldedDirectoryEntry for an explanation of the use of this function
		//
		virtual DirectoryEntry* Image() { return this; }

	protected:
		DirectorySymbol* directory;
		DirectoryEntry* image;
		time_t mtime_;
	};


} // Close namespace Jikes block


#endif // _INCLUDED

