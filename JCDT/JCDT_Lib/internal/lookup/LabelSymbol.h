
#ifndef LabelSymbol_jikes_INCLUDED
#define LabelSymbol_jikes_INCLUDED

#include "Symbol.h"
#include <JCDT_Lib/internal/ast/ast.h>
namespace Jikes { // Open namespace Jikes block

	


class LabelSymbol : public Symbol
{
public:
    AstBlock* block; // the block that is labeled by this symbol
    const NameSymbol* name_symbol;

    unsigned nesting_level;
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

    LabelSymbol(const NameSymbol* name_symbol_)
        : block(NULL)
        , name_symbol(name_symbol_)
        , nesting_level(0)
    {
        Symbol::_kind = LABEL;
    }

    virtual ~LabelSymbol() {}

	static LabelSymbol* LabelCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<LabelSymbol*>(symbol->Kind() == Symbol::LABEL ? symbol : NULL);
	}

	static const LabelSymbol* LabelCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const LabelSymbol*>(symbol->Kind() == Symbol::LABEL ? symbol : NULL);
	}
};




} // Close namespace Jikes block


#endif // LabelSymbol_INCLUDED

