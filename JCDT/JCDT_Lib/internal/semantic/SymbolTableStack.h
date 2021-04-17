
#ifndef SymbolTableStack_INCLUDED
#define SymbolTableStack_INCLUDED


#include "JCDT_Lib/internal/util/tuple.h"
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>

namespace Jikes { // Open namespace Jikes block


	//
	// Maintains a stack of symbol tables, for storing the different variables
	// which are in scope in a method.
	//
	class SymbolTableStack
	{
	public:
		void Push(BindingSymbolTable* symtab) { table.Next() = symtab; }
		void Pop() { if (table.Length() > 0) table.Reset(table.Length() - 1); }
		int Size() { return table.Length(); }
		BindingSymbolTable* Top()
		{
			return (BindingSymbolTable*)(table.Length() > 0
				? table[table.Length() - 1] : NULL);
		}

		BindingSymbolTable* operator[](const int i) { return table[i]; }

		//
		// Search for a variable in a stack of symbol tables starting at the
		// current symbol table and ending with the symbol table of the method
		// from which this call originates.
		//
		VariableSymbol* FindVariableSymbol(NameSymbol* name_symbol)
		{
			for (int i = table.Length() - 1; i >= 0; i--)
			{
				VariableSymbol* symbol =
					table[i]->FindVariableSymbol(name_symbol);
				if (symbol)
					return symbol;
			}
			return NULL;
		}

		//
		// Search for a type in a stack of symbol tables starting at the current
		// symbol table and ending with the symbol table of the method from which
		// this call originates.
		//
		TypeSymbol* FindTypeSymbol(NameSymbol* name_symbol)
		{
			for (int i = table.Length() - 1; i >= 0; i--)
			{
				TypeSymbol* symbol = table[i]->FindTypeSymbol(name_symbol);
				if (symbol)
					return symbol;
			}
			return NULL;
		}

		//
		// Search for a label in a stack of symbol tables starting at the current
		// symbol table and ending with the symbol table of the method from which
		// this call originates.
		//
		LabelSymbol* FindLabelSymbol(NameSymbol* name_symbol)
		{
			for (int i = table.Length() - 1; i >= 0; i--)
			{
				LabelSymbol* label = table[i]->FindLabelSymbol(name_symbol);
				if (label)
					return label;
			}
			return NULL;
		}

	private:
		Tuple<BindingSymbolTable*> table;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

