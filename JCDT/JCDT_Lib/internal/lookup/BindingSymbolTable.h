
#ifndef BindingSymbolTable_jikes_INCLUDED
#define BindingSymbolTable_jikes_INCLUDED

#include "SymbolTable.h"
#include <JCDT_Lib/internal/lookup/ExpandedFieldTable.h>

namespace Jikes {
	class FileSymbol;
	class DirectorySymbol;
	class NameSymbol;
	class PathSymbol;
	class Symbol;
	// Open namespace Jikes block
	class BlockSymbol;
	class LabelSymbol;
	class AstMethodDeclarator;
	class PackageSymbol;
	class MethodSymbol;
	class TypeSymbol;
	class VariableSymbol;

class BindingSymbolTable:public SymbolTable
{
public:
	unsigned NumOtherSymbols();
	Symbol* OtherSym(unsigned i);
	void AddOtherSymbol(Symbol* symbol);

	unsigned NumAnonymousSymbols();
	TypeSymbol* AnonymousSym(unsigned i);
	void AddAnonymousSymbol(TypeSymbol* symbol);

	unsigned NumTypeSymbols();
	TypeSymbol*& TypeSym(unsigned i);
	void AddTypeSymbol(TypeSymbol* symbol);

	unsigned NumMethodSymbols();
	MethodSymbol* MethodSym(unsigned i);
	void AddMethodSymbol(MethodSymbol* symbol);

	unsigned NumVariableSymbols();
	VariableSymbol* VariableSym(unsigned i);
	void AddVariableSymbol(VariableSymbol* symbol);

	

	BindingSymbolTable(unsigned hash_size_ = DEFAULT_HASH_SIZE);
	virtual~BindingSymbolTable();

	 void CompressSpace();

protected:
	virtual unsigned Size() override;
	virtual void Rehash() override;
private:
	// This array should not be convertible. See SymbolTable::DeleteTypeSymbol
	ConvertibleArray<Symbol*>* other_symbol_pool;
	// This array should not be convertible. See SymbolTable::DeleteTypeSymbol
	Tuple<TypeSymbol*>* type_symbol_pool;

	ConvertibleArray<TypeSymbol*>* anonymous_symbol_pool;
	ConvertibleArray<MethodSymbol*>* method_symbol_pool;
	ConvertibleArray<VariableSymbol*>* variable_symbol_pool;
	

public:

	PathSymbol* InsertPathSymbol(NameSymbol*, DirectorySymbol*);
	PathSymbol* FindPathSymbol(const NameSymbol*);

	DirectorySymbol* InsertDirectorySymbol(const NameSymbol*, Symbol*,
		bool source_path);
	DirectorySymbol* FindDirectorySymbol(const NameSymbol*);
	
	 FileSymbol* InsertFileSymbol(const NameSymbol*);
	 void InsertFileSymbol(FileSymbol*);
	 FileSymbol* FindFileSymbol(const NameSymbol*);

	 PackageSymbol* InsertPackageSymbol(NameSymbol*, PackageSymbol*);
	 PackageSymbol* FindPackageSymbol(const NameSymbol*);

	 TypeSymbol* InsertAnonymousTypeSymbol(NameSymbol*);
	 TypeSymbol* InsertTypeSymbol(NameSymbol*);
	 void DeleteTypeSymbol(TypeSymbol*);
	 void DeleteAnonymousTypes();
	 TypeSymbol* FindTypeSymbol(const NameSymbol*);

	 MethodSymbol* InsertMethodSymbol(MethodSymbol*);
	 MethodSymbol* FindMethodSymbol(const NameSymbol*);
	MethodSymbol* FindOverloadMethod(MethodSymbol*, AstMethodDeclarator*);

	 VariableSymbol* InsertVariableSymbol(const NameSymbol*);
	 VariableSymbol* InsertVariableSymbol(VariableSymbol*);
	 VariableSymbol* FindVariableSymbol(const NameSymbol*);

	 LabelSymbol* InsertLabelSymbol(NameSymbol*);
	 LabelSymbol* FindLabelSymbol(const NameSymbol*);

	 BlockSymbol* InsertBlockSymbol(unsigned hash_size = 0);
};
inline unsigned BindingSymbolTable::NumAnonymousSymbols()
{
	return anonymous_symbol_pool ? anonymous_symbol_pool->Length() : 0;
}
inline TypeSymbol* BindingSymbolTable::AnonymousSym(unsigned i)
{
	return (*anonymous_symbol_pool)[i];
}
inline void BindingSymbolTable::AddAnonymousSymbol(TypeSymbol* symbol)
{
	if (!anonymous_symbol_pool)
		anonymous_symbol_pool = new ConvertibleArray<TypeSymbol*>(256);
	anonymous_symbol_pool->Next() = symbol;
	// not hashed, because anonymous types have no name
}

inline unsigned BindingSymbolTable::NumTypeSymbols()
{
	return type_symbol_pool ? type_symbol_pool->Length() : 0;
}
inline TypeSymbol*& BindingSymbolTable::TypeSym(unsigned i)
{
	return (*type_symbol_pool)[i];
}


inline unsigned BindingSymbolTable::NumMethodSymbols()
{
	return method_symbol_pool ? method_symbol_pool->Length() : 0;
}
inline MethodSymbol* BindingSymbolTable::MethodSym(unsigned i)
{
	return (*method_symbol_pool)[i];
}


inline unsigned BindingSymbolTable::NumVariableSymbols()
{
	return variable_symbol_pool ? variable_symbol_pool->Length() : 0;
}
inline VariableSymbol* BindingSymbolTable::VariableSym(unsigned i)
{
	return (*variable_symbol_pool)[i];
}


inline unsigned BindingSymbolTable::NumOtherSymbols()
{
	return other_symbol_pool ? other_symbol_pool->Length() : 0;
}
inline Symbol* BindingSymbolTable::OtherSym(unsigned i)
{
	return (*other_symbol_pool)[i];
}
inline void BindingSymbolTable::AddOtherSymbol(Symbol* symbol)
{
	if (!other_symbol_pool)
		other_symbol_pool = new ConvertibleArray<Symbol*>(256);
	other_symbol_pool->Next() = symbol;
	// not hashed, because not all symbols have names
}


inline void BindingSymbolTable::CompressSpace()
{
	if (anonymous_symbol_pool)
		anonymous_symbol_pool->Array();
	if (method_symbol_pool)
		method_symbol_pool->Array();
	if (variable_symbol_pool)
		variable_symbol_pool->Array();
	if (other_symbol_pool)
		other_symbol_pool->Array();
}

inline unsigned BindingSymbolTable::Size()
{
	return NumAnonymousSymbols() + NumTypeSymbols() + NumMethodSymbols() +
		NumVariableSymbols() + NumOtherSymbols();
}


} // Close namespace Jikes block


#endif // symbol_INCLUDED

