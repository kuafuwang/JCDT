#ifndef VariableSymbolsymbol_INCLUDED
#define VariableSymbolsymbol_INCLUDED

#include "Symbol.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/impl/NameSymbol.h"
#include <JCDT_Lib/internal/impl/FileLocation.h>
#include "JCDT_Lib/internal/impl/access.h"

namespace Jikes { // Open namespace Jikes block



class VariableSymbol : public Symbol, public AccessFlags
{
    enum
    {
        COMPLETE = 0x01, // Used to prevent use of field before declaration
        DEPRECATED = 0x02, // Used to mark deprecated fields
        INITIALIZED = 0x04 // Used when initial value of final field is known
    };

public:
	FieldDeclaration* field_declaration;
    AstVariableDeclarator* declarator;
    FileLocation* file_location;
    const NameSymbol* name_symbol;
    Symbol* owner;
    LiteralValue* initial_value;
    Utf8LiteralValue* signature;

    // Index of element in symbol_pool (in the relevant symbol table) that
    // points to this variable.
    unsigned pool_index;

    VariableSymbol* accessed_local;
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
    void SetLocation();
	const char* Utf8Name() const;
	unsigned Utf8NameLength() const;

	wchar_t* FileLoc();
	void SetExternalIdentity(const NameSymbol* external_name_symbol_);
	const NameSymbol* ExternalIdentity() const;
	const wchar_t* ExternalName() const;
	unsigned ExternalNameLength() const;
	const char* ExternalUtf8Name() const;
	unsigned ExternalUtf8NameLength() const;

	VariableSymbol(const NameSymbol* name_symbol_);

	virtual ~VariableSymbol();

	void SetOwner(Symbol* owner_);

	TypeSymbol* ContainingType();
	const TypeSymbol* ContainingType() const;

    void SetLocalVariableIndex(int index) { local_variable_index = index; }
    //
    // For local variables, returns the index allocated to the variable.
    //
    int LocalVariableIndex() { return local_variable_index; }
    //
    // Returns the local variable index, but for local variables, it adds an
    // offset to account for all final fields. This version should only be
    // needed for definite assignment analysis.
    //
    int LocalVariableIndex(Semantic*);

    bool IsTyped() const { return type_ != NULL; }

	void SetType(TypeSymbol* _type);

    void ProcessVariableSignature(Semantic*, Token*);

	TypeSymbol* Type();
	const TypeSymbol* Type() const;

	void SetSignatureString(const char* signature_, unsigned length);

	bool canBeSeenBy(TypeSymbol* receiverType, InvocationSite* invocationSite, AstNodeScope* scope);
	// Is variable a local variable?
	bool IsLocal();
    // Is variable local to a particular method ?
	bool IsLocal(MethodSymbol* method);

	bool IsFinal(TypeSymbol* type);

	bool IsField() const;
    //
    // These functions are used to identify when the declaration of a field
    // in the body of a class is complete.
    //
    void MarkComplete() { status |= COMPLETE; }
    bool IsDeclarationComplete() { return (status & COMPLETE) != 0; }

    void MarkDeprecated() { status |= DEPRECATED; }
    bool IsDeprecated() { return (status & DEPRECATED) != 0; }

    void MarkInitialized() { status |= INITIALIZED; }
    bool IsInitialized() { return (status & INITIALIZED) != 0; }

	static VariableSymbol* VariableCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<VariableSymbol*>(symbol->Kind() == Symbol::VARIABLE ? symbol : NULL);
	}

	static const VariableSymbol* VariableCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const VariableSymbol*>
			(symbol->Kind() == Symbol::VARIABLE ? symbol : NULL);
	}

private:
    const NameSymbol* external_name_symbol;

    unsigned char status;
    int local_variable_index;
    TypeSymbol* type_;
    char* signature_string;
};

inline const char* VariableSymbol::Utf8Name() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned VariableSymbol::Utf8NameLength() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

inline wchar_t* VariableSymbol::FileLoc()
{
	return (wchar_t*)(file_location ? file_location->location : NULL);
}
inline void VariableSymbol::SetExternalIdentity(const NameSymbol* external_name_symbol_)
{
	external_name_symbol = external_name_symbol_;
}
inline const NameSymbol* VariableSymbol::ExternalIdentity() const
{
	return external_name_symbol ? external_name_symbol : name_symbol;
}
inline const wchar_t* VariableSymbol::ExternalName() const
{
	return external_name_symbol ? external_name_symbol->Name()
		: name_symbol->Name();
}
inline unsigned VariableSymbol::ExternalNameLength() const
{
	return external_name_symbol ? external_name_symbol->NameLength()
		: name_symbol->NameLength();
}
inline const char* VariableSymbol::ExternalUtf8Name() const
{
	return external_name_symbol
		? external_name_symbol->Utf8_literal->value
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned VariableSymbol::ExternalUtf8NameLength() const
{
	return external_name_symbol && external_name_symbol->Utf8_literal
		? external_name_symbol->Utf8_literal->length
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

inline TypeSymbol* VariableSymbol::Type()
{
	// Make sure that the method signature associated with this method is
	// processed prior to invoking this function.
	// ( "this -> ProcessVariableSignature(sem, tok);" )
	assert(type_);
	return type_;
}
inline const TypeSymbol* VariableSymbol::Type() const
{
	assert(type_);
	return type_;
}

inline void VariableSymbol::SetSignatureString(const char* signature_, unsigned length)
{
	signature_string = new char[length + 1];
	strncpy(signature_string, signature_, length);
	signature_string[length] = U_NULL;
}


} // Close namespace Jikes block


#endif // symbol_INCLUDED

