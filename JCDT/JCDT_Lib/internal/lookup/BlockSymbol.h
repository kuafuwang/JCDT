#ifndef BlockSymbolsymbol_INCLUDED
#define BlockSymbolsymbol_INCLUDED
#include "Symbol.h"


namespace Jikes { // Open namespace Jikes block
	class BindingSymbolTable;
	class VariableSymbol;
	class BindingSymbolTable;
	class NameSymbol;
class BlockSymbol : public Symbol
{
public:
    int max_variable_index;
    // try, synchronized, and foreach need synthetic helper variables
    int helper_variable_index;

    BlockSymbol(unsigned hash_size);
    virtual ~BlockSymbol();

    unsigned NumVariableSymbols();
    VariableSymbol* VariableSym(unsigned);

     VariableSymbol* FindVariableSymbol(const NameSymbol*);
     VariableSymbol* InsertVariableSymbol(const NameSymbol*);
     void InsertVariableSymbol(VariableSymbol*);
     BlockSymbol* InsertBlockSymbol(unsigned);

     void CompressSpace();

     BindingSymbolTable* Table();

	static BlockSymbol* BlockCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<BlockSymbol*>(symbol->Kind() == Symbol::BLOCK ? symbol : NULL);
	}

	static const BlockSymbol* BlockCast(const  Symbol* symbol)
	{
		return DYNAMIC_CAST<const BlockSymbol*>(symbol->Kind() == Symbol::BLOCK ? symbol : NULL);
	}

private:
    BindingSymbolTable* table;
};



} // Close namespace Jikes block


#endif // symbol_INCLUDED

