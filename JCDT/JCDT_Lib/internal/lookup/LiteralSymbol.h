
#ifndef LiteralSymbol_INCLUDED
#define LiteralSymbol_INCLUDED

#include "Symbol.h"
namespace Jikes { // Open namespace Jikes block


class LiteralSymbol : public Symbol
{
public:
    LiteralValue* value;

    virtual const wchar_t* Name() const
    {
	    return name_;
    }
    virtual unsigned NameLength() const
    {
	    return length;
    }
    virtual const NameSymbol* Identity() const
    {
	    return NULL;
    }

    LiteralSymbol() : value(nullptr), name_(NULL), length(0), hash_address(0)
    {}
    virtual ~LiteralSymbol() { delete [] name_; }

	//void Initialize(const wchar_t* str, unsigned hash_address_, int length_);

	void Initialize(const wchar_t* str, unsigned hash_address_, int length_)
	{
		Symbol::_kind = Symbol::LITERAL;

		hash_address = hash_address_;

		length = length_;
		name_ = new wchar_t[length + 1];
		memcpy(name_, str, length * sizeof(wchar_t));
		name_[length] = U_NULL;

		value = NULL;
	}

	static LiteralSymbol* LiteralCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<LiteralSymbol*>(symbol->Kind() == Symbol::LITERAL ? symbol : NULL);
	}

	static const LiteralSymbol* LiteralCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const LiteralSymbol*>(symbol->Kind() == Symbol::LITERAL ? symbol : NULL);
	}
private:

    friend class LiteralLookupTable;

    wchar_t* name_;
    int length;
    unsigned hash_address;
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

