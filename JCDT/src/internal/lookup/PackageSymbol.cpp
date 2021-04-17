#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>
namespace Jikes { // Open namespace Jikes block

	 PackageSymbol* PackageSymbol::FindPackageSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindPackageSymbol(name_symbol)
			: (PackageSymbol*)NULL;
	}

	 TypeSymbol* PackageSymbol::InsertSystemTypeSymbol(NameSymbol* name_symbol)
	{
		return Table()->InsertTypeSymbol(name_symbol);
	}

	 TypeSymbol* PackageSymbol::InsertOuterTypeSymbol(NameSymbol* name_symbol)
	{
		return Table()->InsertTypeSymbol(name_symbol);
	}

	
	void PackageSymbol::DeleteTypeSymbol(TypeSymbol* type)
	{
		if (table)
			table->DeleteTypeSymbol(type);
	}




	 TypeSymbol* PackageSymbol::FindTypeSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindTypeSymbol(name_symbol)
			: (TypeSymbol*)NULL;
	}

	 BindingSymbolTable* PackageSymbol::Table()
	{
		return table ? table : table = new BindingSymbolTable(101);
	}


	 PackageSymbol* PackageSymbol::InsertPackageSymbol(NameSymbol* name_symbol)
	{
		return Table()->InsertPackageSymbol(name_symbol, this);
	}

	PackageSymbol::~PackageSymbol()
	{
		delete[] package_name;
		delete table;
	}

	PackageSymbol::PackageSymbol(const NameSymbol* name_symbol_, PackageSymbol* owner_)
		: directory(4)
		, owner(owner_)
		, name_symbol(name_symbol_)
		, table(NULL)
		, package_name(NULL), package_name_length(0)
		  , status(0)
	{
		Symbol::_kind = PACKAGE;
	}
	void PackageSymbol::SetPackageName()
	{
		package_name_length = (owner ? owner->PackageNameLength() + 1 : 0) +
			NameLength(); // +1 for '/'
		package_name = new wchar_t[package_name_length + 1]; // +1 for '\0'

		if (owner)
		{
			wcscpy(package_name, owner->PackageName());
			wcscat(package_name, StringConstant::US_SL);
		}
		else package_name[0] = U_NULL;
		wcscat(package_name, Name());

		assert(wcslen(package_name) == package_name_length);
	}


	

} // Close namespace Jikes block




