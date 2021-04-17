#include <JCDT_Lib/internal/lookup/BlockSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/impl/NameSymbol.h>
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>


namespace Jikes { // Open namespace Jikes block


	BindingSymbolTable* BlockSymbol::Table()
	{
		return table ? table : table = new BindingSymbolTable();
	}

	 VariableSymbol* BlockSymbol::InsertVariableSymbol(const NameSymbol* name_symbol)
	{
		return Table()->InsertVariableSymbol(name_symbol);
	}


	 void BlockSymbol::InsertVariableSymbol(VariableSymbol* variable_symbol)
	{
		Table()->InsertVariableSymbol(variable_symbol);
	}


	 VariableSymbol* BlockSymbol::FindVariableSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindVariableSymbol(name_symbol)
			: (VariableSymbol*)NULL;
	}

	 BlockSymbol* BlockSymbol::InsertBlockSymbol(unsigned hash_size = 0)
	{
		return Table()->InsertBlockSymbol(hash_size);
	}


	 void BlockSymbol::CompressSpace()
	{
		if (table)
			table->CompressSpace();
	}

	 unsigned BlockSymbol::NumVariableSymbols()
	{
		return table ? table->NumVariableSymbols() : 0;
	}
	 VariableSymbol* BlockSymbol::VariableSym(unsigned i)
	{
		return table->VariableSym(i);
	}

	BlockSymbol::BlockSymbol(unsigned hash_size)
		: max_variable_index(-1)
		, helper_variable_index(-1)
		, table(hash_size > 0 ? new BindingSymbolTable(hash_size) : (BindingSymbolTable*)NULL)
	{
		Symbol::_kind = BLOCK;
	}

	BlockSymbol::~BlockSymbol()
	{
		delete table;
	}

	

} // Close namespace Jikes block





