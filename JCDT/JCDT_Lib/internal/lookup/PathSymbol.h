#ifndef PathSymbol_INCLUDED
#define PathSymbol_INCLUDED

#include "Symbol.h"
#include "JCDT_Lib/internal/impl/NameSymbol.h"

namespace Jikes { // Open namespace Jikes block

	class Zip;
	class DirectorySymbol;

class PathSymbol : public Symbol
{
public:
	static PathSymbol* PathCast(Symbol*);
	static const  PathSymbol* PathCast(const Symbol*);


    const NameSymbol* name_symbol;
    Zip* zipfile;

    PathSymbol(const NameSymbol*);
    virtual ~PathSymbol();
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
    const char* Utf8Name() const
    {
        return name_symbol -> Utf8_literal
            ? name_symbol -> Utf8_literal -> value : (char*) NULL;
    }
    unsigned Utf8NameLength() const
    {
        return name_symbol -> Utf8_literal
            ? name_symbol -> Utf8_literal -> length : 0;
    }

    inline bool IsZip() { return zipfile != NULL; }

    inline DirectorySymbol* RootDirectory() { return root_directory; }


    DirectorySymbol* root_directory;
};

inline PathSymbol* PathSymbol::PathCast(Symbol* symbol)
{
	return DYNAMIC_CAST<PathSymbol*>(symbol->Kind() == Symbol::PATH ? symbol : NULL);
}
inline  const PathSymbol* PathSymbol::PathCast(const Symbol* symbol)
{
	return DYNAMIC_CAST<const PathSymbol*>(symbol->Kind() == Symbol::PATH ? symbol : NULL);
}


} // Close namespace Jikes block


#endif // PathSymbol_INCLUDED

