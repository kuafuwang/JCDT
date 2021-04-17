#ifndef PackageSymbol_jikes_INCLUDED
#define PackageSymbol_jikes_INCLUDED
#include "Symbol.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/impl/NameSymbol.h"



namespace Jikes { // Open namespace Jikes block
	class DirectorySymbol;
	class TypeSymbol;
	class BindingSymbolTable;

class PackageSymbol : public Symbol
{
    enum
    {
        DEPRECATED = 0x01
    };

public:
    Tuple<DirectorySymbol*> directory;
    PackageSymbol* owner;

	PackageSymbol(const NameSymbol* name_symbol_, PackageSymbol* owner_);

    virtual ~PackageSymbol();
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
	const char* Utf8Name() const;
	unsigned Utf8NameLength() const;
    // This name is fully qualified, using slashes.

    void SetPackageName();
    // This name is fully qualified, using slashes.
	wstring PackageNameString();
	wchar_t* PackageName();
	unsigned PackageNameLength();

     PackageSymbol* FindPackageSymbol(const NameSymbol*);
     PackageSymbol* InsertPackageSymbol(NameSymbol*);

     TypeSymbol* FindTypeSymbol(const NameSymbol*);
     TypeSymbol* InsertSystemTypeSymbol(NameSymbol*);
     TypeSymbol* InsertOuterTypeSymbol(NameSymbol*);
     void DeleteTypeSymbol(TypeSymbol*);

    void MarkDeprecated() { status |= DEPRECATED; }
    bool IsDeprecated() { return (status & DEPRECATED) != 0; }

	static PackageSymbol* PackageCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<PackageSymbol*>(symbol->Kind() == Symbol::PACKAGE ? symbol : NULL);
	}

	static const PackageSymbol* PackageCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const PackageSymbol*>(symbol->Kind() == Symbol::PACKAGE ? symbol : NULL);
	}
private:
    const NameSymbol* name_symbol;
    BindingSymbolTable* table;
    inline BindingSymbolTable* Table();

    wchar_t* package_name;
    unsigned package_name_length;
    u1 status;
};

inline const char* PackageSymbol::Utf8Name() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned PackageSymbol::Utf8NameLength() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

	inline wstring PackageSymbol::PackageNameString()
	{
		PackageName();
		if (package_name)
		{
			return wstring(package_name, package_name + package_name_length);
		}
		return wstring();
	}

	inline wchar_t* PackageSymbol::PackageName()
{
	if (!package_name)
		SetPackageName();
	return package_name;
}
inline unsigned PackageSymbol::PackageNameLength()
{
	if (!package_name)
		SetPackageName();
	return package_name_length;
}

} // Close namespace Jikes block


#endif // PackageSymbol_INCLUDED

