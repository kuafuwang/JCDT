
#ifndef DirectoryZip_jikes_INCLUDED
#define DirectoryZip_jikes_INCLUDED


#include <JCDT_Lib/internal/util/zip/zip.h>


namespace Jikes {
	
	// Open namespace Jikes block


class DirectoryZip;
class DirectorySymbol;
class FileBase;
class NameSymbol;


class DirectoryZip:public Zip
{
public:
    DirectoryZip(INameEnvironment *, const char *);
	DirectoryZip(INameEnvironment *, const wchar_t *);
    ~DirectoryZip();

    DirectorySymbol *RootDirectory() { return root_directory; }
	virtual void Clear();
protected:
   
    DirectorySymbol *root_directory;
	INameEnvironment* nameEnv;

    void ReadDirectory();

   
    DirectorySymbol *ProcessSubdirectoryEntries(DirectorySymbol *, char *, int);
	
    void ProcessDirectoryEntry();
	NameSymbol * ProcessFilename(char *name, int name_length);
};


} // Close namespace Jikes block


#endif // zip_INCLUDED

