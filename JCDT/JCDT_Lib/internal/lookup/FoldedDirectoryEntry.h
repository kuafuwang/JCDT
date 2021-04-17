
#ifndef FoldedDirectoryEntry_INCLUDED
#define FoldedDirectoryEntry_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "DirectoryEntry.h"

namespace Jikes { // Open namespace Jikes block


#ifdef WIN32_FILE_SYSTEM
				  //
				  // This object is needed only for systems such as Windows NT/95/98 that
				  // treat filenames in a case-insensitive fashion.
				  //
	class FoldedDirectoryEntry : public DirectoryEntry
	{
	public:
		FoldedDirectoryEntry(DirectoryEntry* image_)
		{
			DirectoryEntry::image = image_;
		}
		virtual ~FoldedDirectoryEntry() {}

		virtual DirectoryEntry* Image() { return image; }
	};
#endif // WIN32_FILE_SYSTEM




} // Close namespace Jikes block


#endif // _INCLUDED

