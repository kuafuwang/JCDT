#ifndef MethodSymbolsymbol_jikes_INCLUDED
#define MethodSymbolsymbol_jikes_INCLUDED
#include "Symbol.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/impl/access.h"
#include <JCDT_Lib/internal/impl/FileLocation.h>


namespace Jikes { // Open namespace Jikes block


class MethodSymbol : public Symbol, public AccessFlags
{
    enum
    {
        DEPRECATED = 0x0001,
		CONSTRUCTOR_METHOD = 0x0002,
		BINARY_METHOD = 0x0004
		
    };

public:
    Ast* declaration; // MethodDeclaration or ConstructorDeclaration
    const NameSymbol* name_symbol;
    TypeSymbol* containing_type;
    BlockSymbol* block_symbol;
    MethodSymbol* next_method;
    Utf8LiteralValue* signature;
    FileLocation* file_location;
    // Index of element in symbol_pool (in the relevant symbol table) that
    // points to this method.
    unsigned pool_index;

    unsigned max_block_depth;
	bool isConstructor ;
    //
    // If this method is a method that permits access to a private member of an
    // enclosing type then accessed_member identifies the member in question.
    //
    Symbol* accessed_member;
	inline bool AccessesStaticMember();
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

	wchar_t* FileLoc();
    void SetLocation();


	bool areParametersEqual(MethodSymbol*) const;

	MethodSymbol(const NameSymbol* name_symbol_);

    virtual ~MethodSymbol();

    bool IsTyped() const { return type_ != NULL; }

	void SetType(TypeSymbol* _type);

    void ProcessMethodSignature(Semantic*, Token*);
    void ProcessMethodThrows(Semantic*, Token*);

	TypeSymbol* Type();
	const TypeSymbol* Type() const;

	unsigned NumFormalParameters() const;
	VariableSymbol* FormalParameter(unsigned i) const;
	void AddFormalParameter(VariableSymbol* variable);

	unsigned NumThrows();
	TypeSymbol* Throws(unsigned i);
	void AddThrows(TypeSymbol* exception);

	unsigned NumThrowsSignatures();
	char* ThrowsSignature(unsigned i);
	void AddThrowsSignature(const char* signature_, unsigned length);

	void SetExternalIdentity(const NameSymbol* external_name_symbol_);
	const NameSymbol* ExternalIdentity() const;
	const wchar_t* ExternalName() const;
	unsigned ExternalNameLength() const;
	const char* ExternalUtf8Name() const;
	unsigned ExternalUtf8NameLength() const;

	void SetContainingType(TypeSymbol* containing_type_);
	void SetBlockSymbol(BlockSymbol* block_symbol_);


    void SetSignature(LookupEnvironment&, TypeSymbol* = NULL);
    void SetSignature(Utf8LiteralValue* signature_) { signature = signature_; }
    const char* SignatureString() const { return signature -> value; }
    wchar_t* Header();

    void CleanUp();

    void MarkDeprecated() { status |= DEPRECATED; }
    bool IsDeprecated() const { return (status & DEPRECATED) != 0; }


	void MarkConstructor()
    {
	    status |= CONSTRUCTOR_METHOD;
    }
	bool IsConstructor()const
    {
	    return (status & CONSTRUCTOR_METHOD) != 0;
    }

	void GetParameterNames(vector<wstring>& parameterPackageNames,
						   vector<wstring>& parameterTypeNames);
	void GetParameterTypeQuilifiedNames(vector<wstring>& parameterTypes);

	static MethodSymbol* MethodCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<MethodSymbol*>(symbol->Kind() == Symbol::METHOD ? symbol : NULL);
	}

	static const MethodSymbol* MethodCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const MethodSymbol*>(symbol->Kind() == Symbol::METHOD ? symbol : NULL);
	}
	bool canBeSeenBy(InvocationSite* invocationSite, AstNodeScope* scope);
	bool canBeSeenBy(TypeSymbol* receiverType, InvocationSite* invocationSite, AstNodeScope* scope);
private:
    const NameSymbol* external_name_symbol;

    unsigned char status;
    wchar_t* header;

    // The return type of methods, and the containing type of constructors.
    TypeSymbol* type_;

    Tuple<VariableSymbol*>* formal_parameters;
    Tuple<TypeSymbol*>* throws;
    Tuple<char*>* throws_signatures;
};


inline bool MethodSymbol::AccessesStaticMember()
{
	return accessed_member &&
		DYNAMIC_CAST<AccessFlags*>(accessed_member)->ACC_STATIC();
}
inline const char* MethodSymbol::Utf8Name() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned MethodSymbol::Utf8NameLength() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}
inline wchar_t* MethodSymbol::FileLoc()
{
	return (wchar_t*)(file_location ? file_location->location : NULL);
}

inline void MethodSymbol::SetType(TypeSymbol* _type)
{
	if(!_type)
		type_ = _type;
	else 
		type_ = _type;
}


inline TypeSymbol* MethodSymbol::Type()
{
	// Make sure that the method signature associated with this method is
	// processed prior to invoking this function.
	//  ( "this -> ProcessMethodSignature(sem, tok);" )
	assert(type_);
	return type_;
}
inline const TypeSymbol* MethodSymbol::Type() const
{
	assert(type_);
	return type_;
}

inline unsigned MethodSymbol::NumFormalParameters() const
{
	assert(type_);
	return formal_parameters ? formal_parameters->Length() : 0;
}
inline VariableSymbol* MethodSymbol::FormalParameter(unsigned i) const
{
	return (*formal_parameters)[i];
}
inline void MethodSymbol::AddFormalParameter(VariableSymbol* variable)
{
	if (!formal_parameters)
		formal_parameters = new Tuple<VariableSymbol*>(8);
	formal_parameters->Next() = variable;
}

inline unsigned MethodSymbol::NumThrows()
{
	assert(!throws_signatures);
	return throws ? throws->Length() : 0;
}
inline TypeSymbol* MethodSymbol::Throws(unsigned i)
{
	return (*throws)[i];
}
inline void MethodSymbol::AddThrows(TypeSymbol* exception)
{
	if (!throws)
		throws = new Tuple<TypeSymbol*>(8);
	throws->Next() = exception;
}

inline unsigned MethodSymbol::NumThrowsSignatures()
{
	return throws_signatures ? throws_signatures->Length() : 0;
}
inline char* MethodSymbol::ThrowsSignature(unsigned i)
{
	return (*throws_signatures)[i];
}
inline void MethodSymbol::AddThrowsSignature(const char* signature_, unsigned length)
{
	char* signature = new char[length + 1];
	strncpy(signature, signature_, length);
	signature[length] = U_NULL;

	if (!throws_signatures)
		throws_signatures = new Tuple<char*>(8);
	throws_signatures->Next() = signature;
}

inline void MethodSymbol::SetExternalIdentity(const NameSymbol* external_name_symbol_)
{
	external_name_symbol = external_name_symbol_;
}
inline const NameSymbol* MethodSymbol::ExternalIdentity() const
{
	return external_name_symbol ? external_name_symbol : name_symbol;
}
inline const wchar_t* MethodSymbol::ExternalName() const
{
	return external_name_symbol ? external_name_symbol->Name()
		: name_symbol->Name();
}
inline unsigned MethodSymbol::ExternalNameLength() const
{
	return external_name_symbol ? external_name_symbol->NameLength()
		: name_symbol->NameLength();
}
inline const char* MethodSymbol::ExternalUtf8Name() const
{
	return external_name_symbol
		? external_name_symbol->Utf8_literal->value
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned MethodSymbol::ExternalUtf8NameLength() const
{
	return external_name_symbol && external_name_symbol->Utf8_literal
		? external_name_symbol->Utf8_literal->length
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

inline void MethodSymbol::SetContainingType(TypeSymbol* containing_type_)
{
	containing_type = containing_type_;
}
inline void MethodSymbol::SetBlockSymbol(BlockSymbol* block_symbol_)
{
	block_symbol = block_symbol_;
}



} // Close namespace Jikes block


#endif // symbol_INCLUDED

