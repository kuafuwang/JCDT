
#ifndef SymbolSymbol_INCLUDED
#define SymbolSymbol_INCLUDED
#include <JCDT_Lib/internal/impl/NameSymbol.h>


namespace Jikes { // Open namespace Jikes block

class Symbol
{
public:
    Symbol* next;

    enum SymbolKind
    {
         NONE,
         NAME,
         PACKAGE,
         TYPE, // class or interface
         METHOD,
         BLOCK,
         VARIABLE,
         LABEL,
         LITERAL,

         PATH,
         _DIRECTORY,
         _FILE,

         _num_kinds
    };

    SymbolKind Kind() const { return _kind; }
	virtual wstring getNameString() const { return wstring(); }
    virtual const wchar_t* Name() const { return NULL; }
    virtual unsigned NameLength() const { return 0; }
    virtual const NameSymbol* Identity() const { return NULL; }

	inline unsigned HashCode()
	{
		return (unsigned)Identity()->index;
	};
 
    virtual ~Symbol() {}
	

    SymbolKind _kind;
};

} // Close namespace Jikes block


#endif // _INCLUDED

