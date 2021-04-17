
#ifndef TypeSymbolsymbol_jikes_INCLUDED
#define TypeSymbolsymbol_jikes_INCLUDED

#include "Symbol.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/impl/access.h"
#include <JCDT_Lib/internal/util/set.h>
#include <JCDT_Lib/internal/impl/FileLocation.h>

namespace Jikes { // Open namespace Jikes block

	class SemanticEnvironment;
	class LookupEnvironment;
	class AstClassBody;
	class FileSymbol;
	
	class ExpandedTypeTable;
	class ExpandedFieldTable;
	class ExpandedMethodTable;
	class PackageSymbol;
	class BindingSymbolTable;
	class TypeHeaderInfo;
class TypeSymbol : public Symbol, public AccessFlags 
{
    enum
    {
        CONSTRUCTOR_MEMBERS_PROCESSED = 0x0001,
        METHOD_MEMBERS_PROCESSED = 0x0002,
        FIELD_MEMBERS_PROCESSED = 0x0004,
        LOCAL_CLASS_PROCESSING_COMPLETED = 0x0008,
        SOURCE_PENDING = 0x0010,
        ANONYMOUS = 0x0020,
        HEADER_PROCESSED = 0x0040,
        PRIMITIVE = 0x0080,
        DEPRECATED = 0x0100,
        ENUM_TYPE = 0x0200, // can't use ACC_ENUM on types :(
        BAD = 0x0400,
        CIRCULAR =0x0800
    };

public:



    SemanticEnvironment* semantic_environment;
    AstClassBody* declaration;

    FileSymbol* file_symbol;
    FileLocation* file_location;
    const NameSymbol* name_symbol;
    Symbol* owner;

    // A nested class identifies the outer most type that contains it. If a
    // class is not nested then it identifies itself as its outermost type.
    TypeSymbol* outermost_type;
	wstring*  compoundName;
    TypeSymbol* super;

    // Indicates the base type (type of elements in the last dimension) of an
    // array. For a normal type base_type is NULL. If base_type is a "bad"
    // type it points to itself (this).
    TypeSymbol* base_type;

    // This variable is used in TypeCycleChecker to determine if this type
    // forms an inter-type cycle in its "extends" or "implements" relationship.
    int index;

    // This variable is used in TypeCycleChecker to check if this type
    // forms an intra-type cycle in its "extends" or "implements" relationship;
    int unit_index;

    // This variable is used in TypeCycleChecker to determine which types
    // (files) need to be recompiled based on the "dependent" relationship.
    int incremental_index;

	unsigned NumLocalConstructorCallEnvironments();
	SemanticEnvironment*& LocalConstructorCallEnvironment(unsigned i);
	void AddLocalConstructorCallEnvironment(SemanticEnvironment* environment);

	unsigned NumPrivateAccessMethods();
	MethodSymbol*& PrivateAccessMethod(unsigned i);
	void AddPrivateAccessMethod(MethodSymbol* method_symbol);

	unsigned NumPrivateAccessConstructors();
	MethodSymbol*& PrivateAccessConstructor(unsigned i);
	void AddPrivateAccessConstructor(MethodSymbol* constructor_symbol);

	unsigned NumConstructorParameters();
	VariableSymbol*& ConstructorParameter(unsigned i);
	void AddConstructorParameter(VariableSymbol* variable_symbol);

	VariableSymbol*& EnclosingInstance();

	unsigned NumClassLiterals();
	VariableSymbol*& ClassLiteral(unsigned i);
	void AddClassLiteral(VariableSymbol* literal_symbol);

	VariableSymbol* AssertVariable();

	unsigned NumNestedTypes();
	TypeSymbol*& NestedType(unsigned i);
	void AddNestedType(TypeSymbol* type_symbol);

	unsigned NumInterfaces() const;
  
	void ResetInterfaces();
	TypeSymbol* Interface(unsigned i) const;
	void AddInterface(TypeSymbol* type_symbol);

	unsigned NumAnonymousTypes();
	TypeSymbol*& AnonymousType(unsigned i);
	void AddAnonymousType(TypeSymbol* type_symbol);
    void DeleteAnonymousTypes();
    unsigned NumLocalTypes();
	void CleanUpType();
	void getTypesInfo(set<wstring>&, vector<TypeHeaderInfo*>&);

    SymbolSet* local;
    SymbolSet* non_local;
    SymbolSet* supertypes_closure;
    SymbolSet* subtypes;
    SymbolSet* subtypes_closure;
    SymbolSet* innertypes_closure;
    SymbolSet* dependents;
    SymbolSet* parents;
    SymbolSet* static_parents;
    SymbolSet* dependents_closure;  // processed in cycle.cpp
    SymbolSet* parents_closure;     // processed in cycle.cpp

    // Index of element in symbol_pool (in the relevant symbol table) that
    // points to this type.
    unsigned pool_index;

    Utf8LiteralValue* signature;
    Utf8LiteralValue* fully_qualified_name;

    ExpandedTypeTable* expanded_type_table;
    ExpandedFieldTable* expanded_field_table;
    ExpandedMethodTable* expanded_method_table;


	 

    unsigned num_dimensions;

    //
    // Initializer blocks and variable declarations which require
    // initialization are coalesced into these two methods. Notice that
    // bytecode.cpp emits an actual method named '<clinit>' for the static
    // case, and one named 'this' for the instance case (yes, that is a legal
    // VM name, but an illegal Java source code name). Constructors that do
    // not invoke another constructor via the this() statement will defer
    // variable initialization to a generated call to the method 'this'. This
    // relies on VM's allowing the assignment of final instance fields in an
    // instance method instead of a constructor.
    //
    MethodSymbol* instance_initializer_method;
    MethodSymbol* static_initializer_method;
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


	void SetExternalIdentity(const NameSymbol* external_name_symbol_);
	const NameSymbol* ExternalIdentity() const;

	wstring ExternalNameString() const;
	const wchar_t* ExternalName() const;
	unsigned ExternalNameLength() const;
	const char* ExternalUtf8Name() const;
	unsigned ExternalUtf8NameLength() const;

    TypeSymbol(const NameSymbol*);
    virtual ~TypeSymbol();

    void ProcessTypeHeaders();
    void ProcessMembers();
    void CompleteSymbolTable();
    void ProcessExecutableBodies();
	bool canBeSeenBy(TypeSymbol* receiverType, TypeSymbol* invocationType);
	void RemoveCompilationReferences();

    VariableSymbol* InsertThis0();

    TypeSymbol* FindOrInsertClassLiteralClass();
    MethodSymbol* FindOrInsertClassLiteralMethod(LookupEnvironment&);
    MethodSymbol* ClassLiteralMethod()
    {
        return class_literal_method;
    }
    Utf8LiteralValue* FindOrInsertClassLiteralName(LookupEnvironment&);
    VariableSymbol* FindOrInsertClassLiteral(TypeSymbol*);
    VariableSymbol* FindOrInsertLocalShadow(VariableSymbol*);
    VariableSymbol* FindOrInsertAssertVariable();

    //
    // Get an accessor method in this class for the given symbol, with
    // qualifying type defaulting to this type if unspecified
    //
    MethodSymbol* GetReadAccessMethod(MethodSymbol*, TypeSymbol* = NULL);
    MethodSymbol* GetReadAccessConstructor(MethodSymbol*);
    MethodSymbol* GetReadAccessMethod(VariableSymbol*, TypeSymbol* = NULL);
    MethodSymbol* GetWriteAccessMethod(VariableSymbol*, TypeSymbol* = NULL);
    MethodSymbol* GetWriteAccessFromReadAccess(MethodSymbol*);
    TypeSymbol* GetPlaceholderType();

    bool IsArray() const { return num_dimensions > 0; }

    void SetOwner(Symbol* owner_) { owner = owner_; }

	bool IsOwner(TypeSymbol* type);

	TypeSymbol* ContainingType();
	const TypeSymbol* ContainingType() const;

    TypeSymbol* EnclosingType();
    bool HasEnclosingInstance(TypeSymbol*, bool = false);
    bool HasProtectedAccessTo(TypeSymbol*);

    //
    // For JSR 201, control.int_class -> BoxedType() returns lookupEnvironment.Integer(),
    // types without boxing return themselves. UnboxedType() works the other
    // direction.
    //
    TypeSymbol* BoxedType(LookupEnvironment&);
    TypeSymbol* UnboxedType(LookupEnvironment&);

    //
    // Note that this test considers a class a subclass of itself, and also
    // interfaces are a subclass of Object. See also IsSubtype.
    //
	bool IsSubclass(const TypeSymbol* super_class) const;

    //
    // Note that this test considers an interface a subtype of itself, but
    // does not work for classes. See also IsSubtype.
    //
	bool IsSubinterface(const TypeSymbol* super_interface) const;

    //
    // This test works for classes, but not for interfaces. See also IsSubtype.
    //
	bool Implements(const TypeSymbol* inter) const;

    //
    // The most generic subtype relation; returns true if this type is a
    // subtype of the argument type. This correctly checks a class's
    // superclasses and superinterfaces, an interfaces's superinterfaces and
    // Object, and an array's compatible types (smaller dimensions of Object,
    // Cloneable, Serializable, and all equal dimension arrays where the
    // element type is a subtype). For simplicity, a type subtypes itself.
    //
	bool IsSubtype(const TypeSymbol* type) const;

	wchar_t* FileLoc();

    void SetLocation();

    //
    // Returns the array type of the given number of dims with the same base
    // type as this (ie. Object[] -> GetArrayType(...,2) returns Object[][]).
    //
	TypeSymbol* leafComponentType();
    TypeSymbol* GetArrayType(Semantic*, unsigned);

	TypeSymbol* ArraySubtype() const;

    void SetSignature(LookupEnvironment&);
    void SetSignature(Utf8LiteralValue* signature_) { signature = signature_; }
    const char* SignatureString() const
    {
		if (signature)
			return signature->value;
		else
			return "";
    }

	void SetClassLiteralName(Utf8LiteralValue* class_literal_name_);

	PackageSymbol* ContainingPackage() const;
    // Returns the fully-qualified '/' separated package name.
	const wchar_t* ContainingPackageName() const;
	const wchar_t* getCompoundName();
	

    bool IsNestedIn(TypeSymbol*);

    bool IsNested() const { return outermost_type != this; }

    //
    // JLS2 8.1.2 states that ALL local and anonymous classes are inner
    // classes, even when they occur in a static context.  Even in the static
    // context, such classes are not implicitly static, they simply lack an
    // enclosing instance.  In other words, the JLS definition of inner class
    // is lame. If everything works correctly, these classes will correctly
    // be marked nested, yet never static.
    //
	bool IsInner() const;

	bool IsLocal() const;
	void RemoveClassFileFromOutputDirWhenInJavaFile();

	inline const char* ClassName();
	inline string getClassNameString();
	wstring qualifiedSourceName(wchar_t separate);
	wstring sourceName(wchar_t separate) {
		if (Anonymous())
			return readableName(separate);
		else
			return getNameString();
	}

    wstring readableName(wchar_t separate);

	void MarkConstructorMembersProcessed();
	bool ConstructorMembersProcessed() const;

	void MarkMethodMembersProcessed();
	bool MethodMembersProcessed() const;

	void MarkFieldMembersProcessed();
	bool FieldMembersProcessed() const;

	void MarkLocalClassProcessingCompleted();
	bool LocalClassProcessingCompleted() const;

    void MarkSourcePending() { status |= SOURCE_PENDING; }
    void MarkSourceNoLongerPending() { status &= ~ SOURCE_PENDING; }
    bool SourcePending() const { return (status & SOURCE_PENDING) != 0; }

    void MarkAnonymous() { status |= ANONYMOUS; }
    bool Anonymous() const { return (status & ANONYMOUS) != 0; }

    void MarkHeaderProcessed() { status |= HEADER_PROCESSED; }
    bool HeaderProcessed() const { return (status & HEADER_PROCESSED) != 0; }

    void MarkPrimitive() { status |= PRIMITIVE; }
    bool Primitive() const { return (status & PRIMITIVE) != 0; }

    void MarkDeprecated() { status |= DEPRECATED; }
    void ResetDeprecated() { status &= ~DEPRECATED; }
    bool IsDeprecated() const { return (status & DEPRECATED) != 0; }

    void MarkEnum() { status |= ENUM_TYPE; }
    void ResetEnum() { status &= ~ENUM_TYPE; }
    bool IsEnum() const { return (status & ENUM_TYPE) != 0; }



	void MarkBad();
    bool Bad() const { return (status & BAD) != 0; }

	void MarkCircular();
    void MarkNonCircular() { status &= ~ CIRCULAR; }
    bool Circular() const { return (status & CIRCULAR) != 0; }

    void ProcessNestedTypeSignatures(Semantic*, Token*);

    bool NestedTypesProcessed() { return nested_type_signatures == NULL; }

	unsigned NumNestedTypeSignatures();
	char* NestedTypeSignature(unsigned i);
	void AddNestedTypeSignature(const char* signature_, unsigned length);

     void SetSymbolTable(unsigned);
     BindingSymbolTable* Table();

    unsigned NumVariableSymbols();
    VariableSymbol* VariableSym(unsigned);

    unsigned NumMethodSymbols();
    MethodSymbol* MethodSym(unsigned);

    unsigned NumTypeSymbols();
    TypeSymbol* TypeSym(unsigned);

     TypeSymbol* InsertAnonymousTypeSymbol(NameSymbol*);
     TypeSymbol* InsertNestedTypeSymbol(NameSymbol*);
     TypeSymbol* FindTypeSymbol(const NameSymbol*);

     VariableSymbol* InsertVariableSymbol(const NameSymbol*);
     void InsertVariableSymbol(VariableSymbol*);
     VariableSymbol* FindVariableSymbol(const NameSymbol*);

     MethodSymbol* InsertMethodSymbol(const NameSymbol*);
     void InsertMethodSymbol(MethodSymbol*);
     MethodSymbol* FindMethodSymbol(const NameSymbol*);
    MethodSymbol* FindOverloadMethod(MethodSymbol*, AstMethodDeclarator*);

    inline void CompressSpace();
    void UnlinkFromParents();


	static TypeSymbol* TypeCast(Symbol* symbol)
	{
		return DYNAMIC_CAST<TypeSymbol*>(symbol->Kind() == Symbol::TYPE ? symbol : NULL);
	}

	static const TypeSymbol* TypeCast(const Symbol* symbol)
	{
		return DYNAMIC_CAST<const TypeSymbol*>(symbol->Kind() == Symbol::TYPE ? symbol : NULL);
	}

	// The interfaces that were declared in the header of the type.
	vector<TypeSymbol*>* interfaces;
	PackageSymbol* package;

	void availableMethods(vector<MethodSymbol*>& methods);
	void availableFields(vector<VariableSymbol*>& fields);

	Map<Symbol, Map<TypeSymbol, MethodSymbol> >* GetReadMethods() { return read_methods; };
	Map<VariableSymbol, Map<TypeSymbol, MethodSymbol> >* GetWriteMethods() { return write_methods; };
private:
    //
    // The fields hash_address and next_type are used in the class
    // TypeLookupTable to contruct a mapping from each fully_qualified name
    // into the type that it defines.
    //
    friend class TypeLookupTable;
    unsigned hash_address;
    TypeSymbol* next_type;

    const NameSymbol* external_name_symbol;

    BindingSymbolTable* table;
    SymbolMap* local_shadow_map;

    unsigned short status;

  
    string class_name;

    void SetClassName();

    MethodSymbol* class_literal_method;
    Utf8LiteralValue* class_literal_name;
    VariableSymbol* assert_variable;

    //
    // For a local type, when we first encounter an embedded call to one of
    // its constructors or a constructor of one of its inner types, either via
    // a ClassCreationExpression or an ExplicitConstructorInvocation, we record
    // it and resolve it after we have computed all necessary information
    // about the type and its inner types.
    //
    Tuple<SemanticEnvironment*>* local_constructor_call_environments;

    //
    // When an inner class tries to access a private member of one of its
    // enclosing classes, one (or two) access method(s) to read (and/or write)
    // the private member is (are) generated.
    //
    // The maps read_methods and write_methods are used to keep track of the
    // read and write method to which a member has been mapped.
    //
    Tuple<MethodSymbol*>* private_access_methods;
    Tuple<MethodSymbol*>* private_access_constructors;

     void MapSymbolToReadMethod(Symbol*, TypeSymbol*, MethodSymbol*);
     MethodSymbol* ReadMethod(Symbol*, TypeSymbol*);
     void MapSymbolToWriteMethod(VariableSymbol*, TypeSymbol*,
                                       MethodSymbol*);
     MethodSymbol* WriteMethod(VariableSymbol*, TypeSymbol*);

    Map<Symbol, Map<TypeSymbol, MethodSymbol> >* read_methods;
    Map<VariableSymbol, Map<TypeSymbol, MethodSymbol> >* write_methods;
    TypeSymbol* placeholder_type;

    //
    // For an accessible inner class the first element in this array
    // identifies the "this$0" pointer of the containing type. For a local
    // class, in addition to the this$0 pointer (if it is needed), all local
    // variables that are referred to in the local type are passed as argument
    // to the local type and copied in the constructor into a local field.
    // These local variables are stored in constructor_parameters.
    //
    // The array enclosing_instances is there for optimization purposes.
    // If this type is deeply nested within several other types and it makes
    // references to members in the enclosing types, then it might
    // be useful to keep a reference to each of these enclosing
    // instances in the form of this$0, this$1, this$2, ...
    //
    // The array class_identities is used to store static variables of type
    // Class that contain the proper value for a given type.
    //
    Tuple<VariableSymbol*>* constructor_parameters;
    VariableSymbol* enclosing_instance;
    Tuple<VariableSymbol*>* class_literals;

    Tuple<char*>* nested_type_signatures;
	public:
    //
    // The inner types that appear immediately within this type in the order
    // in which they should be processed (compiled).
    //
    vector<TypeSymbol*>* nested_types;
  
    // The anonymous types that were declared in this type.
    vector<TypeSymbol*>* anonymous_types;
	private:
    //
    // The arrays of this type that were declared.
    //
    Tuple<TypeSymbol*>* array;
	 unsigned NumArrays();
	 TypeSymbol* Array(unsigned i);
	 void AddArrayType(TypeSymbol* type_symbol);
};

inline unsigned TypeSymbol::NumLocalConstructorCallEnvironments()
{
	return local_constructor_call_environments
		? local_constructor_call_environments->Length() : 0;
}
inline SemanticEnvironment*& TypeSymbol::LocalConstructorCallEnvironment(unsigned i)
{
	return (*local_constructor_call_environments)[i];
}
inline void TypeSymbol::AddLocalConstructorCallEnvironment(SemanticEnvironment* environment)
{
	if (!local_constructor_call_environments)
		local_constructor_call_environments =
		new Tuple<SemanticEnvironment*>(8);
	local_constructor_call_environments->Next() = environment;
}

inline unsigned TypeSymbol::NumPrivateAccessMethods()
{
	return private_access_methods
		? private_access_methods->Length() : 0;
}
inline MethodSymbol*& TypeSymbol::PrivateAccessMethod(unsigned i)
{
	return (*private_access_methods)[i];
}
inline void TypeSymbol::AddPrivateAccessMethod(MethodSymbol* method_symbol)
{
	if (!private_access_methods)
		private_access_methods = new Tuple<MethodSymbol*>(8);
	private_access_methods->Next() = method_symbol;
}

inline unsigned TypeSymbol::NumPrivateAccessConstructors()
{
	return private_access_constructors
		? private_access_constructors->Length() : 0;
}
inline  MethodSymbol*& TypeSymbol::PrivateAccessConstructor(unsigned i)
{
	return (*private_access_constructors)[i];
}
inline void TypeSymbol::AddPrivateAccessConstructor(MethodSymbol* constructor_symbol)
{
	if (!private_access_constructors)
		private_access_constructors = new Tuple<MethodSymbol*>(8);
	private_access_constructors->Next() = constructor_symbol;
}

inline unsigned TypeSymbol::NumConstructorParameters()
{
	return constructor_parameters
		? constructor_parameters->Length() : 0;
}
inline VariableSymbol*& TypeSymbol::ConstructorParameter(unsigned i)
{
	return (*constructor_parameters)[i];
}
inline void TypeSymbol::AddConstructorParameter(VariableSymbol* variable_symbol)
{
	if (!constructor_parameters)
		constructor_parameters = new Tuple<VariableSymbol*>(8);
	constructor_parameters->Next() = variable_symbol;
}

inline VariableSymbol*& TypeSymbol::EnclosingInstance()
{
	return enclosing_instance;
}

inline unsigned TypeSymbol::NumClassLiterals()
{
	return class_literals ? class_literals->Length() : 0;
}
inline VariableSymbol*& TypeSymbol::ClassLiteral(unsigned i)
{
	return (*class_literals)[i];
}
inline void TypeSymbol::AddClassLiteral(VariableSymbol* literal_symbol)
{
	if (!class_literals)
		class_literals = new Tuple<VariableSymbol*>(8);
	class_literals->Next() = literal_symbol;
}

inline VariableSymbol* TypeSymbol::AssertVariable()
{
	return assert_variable;
}

inline unsigned TypeSymbol::NumNestedTypes()
{
	return nested_types ? nested_types->size() : 0;
}
inline TypeSymbol*& TypeSymbol::NestedType(unsigned i)
{
	return (*nested_types)[i];
}
inline void TypeSymbol::AddNestedType(TypeSymbol* type_symbol)
{
	if (!nested_types)
		nested_types = new vector<TypeSymbol*>();
	nested_types->push_back(type_symbol) ;
}

inline unsigned TypeSymbol::NumInterfaces() const
{
	return interfaces ? interfaces->size() : 0;
}
inline void TypeSymbol::ResetInterfaces()
{
	delete interfaces;
	interfaces = NULL;
}
inline TypeSymbol* TypeSymbol::Interface(unsigned i) const
{
	return (*interfaces)[i];
}
inline  void TypeSymbol::AddInterface(TypeSymbol* type_symbol)
{
	if (!interfaces)
		interfaces = new vector<TypeSymbol*>();
	interfaces->push_back(type_symbol);
}

inline unsigned TypeSymbol::NumAnonymousTypes()
{
	return anonymous_types ? anonymous_types->size() : 0;
}
inline TypeSymbol*& TypeSymbol::AnonymousType(unsigned i)
{
	return (*anonymous_types)[i];
}
inline void TypeSymbol::AddAnonymousType(TypeSymbol* type_symbol)
{
	if (!anonymous_types)
		anonymous_types = new vector<TypeSymbol*>();
	anonymous_types->push_back(type_symbol);
	if (!outermost_type->placeholder_type)
		outermost_type->placeholder_type = type_symbol;
}

inline const char*  TypeSymbol::Utf8Name() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned  TypeSymbol::Utf8NameLength() const
{
	return name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}


inline void  TypeSymbol::SetExternalIdentity(const NameSymbol* external_name_symbol_)
{
	external_name_symbol = external_name_symbol_;
}
inline const NameSymbol*  TypeSymbol::ExternalIdentity() const
{
	return external_name_symbol ? external_name_symbol : name_symbol;
}

	inline wstring TypeSymbol::ExternalNameString() const
	{
		auto _name = ExternalName();
		auto len = ExternalNameLength();
		if(_name)
		{
			return wstring(_name, _name + len);
		}
		return wstring();
	}

	inline const wchar_t*  TypeSymbol::ExternalName() const
{
	return external_name_symbol ? external_name_symbol->Name()
		: name_symbol->Name();
}
inline unsigned  TypeSymbol::ExternalNameLength() const
{
	return external_name_symbol ? external_name_symbol->NameLength()
		: name_symbol->NameLength();
}
inline const char*  TypeSymbol::ExternalUtf8Name() const
{
	return external_name_symbol
		? external_name_symbol->Utf8_literal->value
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->value : (char*)NULL;
}
inline unsigned  TypeSymbol::ExternalUtf8NameLength() const
{
	return external_name_symbol && external_name_symbol->Utf8_literal
		? external_name_symbol->Utf8_literal->length
		: name_symbol->Utf8_literal
		? name_symbol->Utf8_literal->length : 0;
}

inline  void TypeSymbol::AddArrayType(TypeSymbol* type_symbol)
{
	if (!array)
		array = new Tuple<TypeSymbol*>(4);
	array->Next() = type_symbol;
}

//
// Note that this test considers a class a subclass of itself, and also
// interfaces are a subclass of Object. See also IsSubtype.
//
inline bool TypeSymbol::IsSubclass(const TypeSymbol* super_class) const
{
	for (const TypeSymbol* type = this; type; type = type->super)
		if (type == super_class)
			return true;
	return false;
}

//
// Note that this test considers an interface a subtype of itself, but
// does not work for classes. See also IsSubtype.
//
inline bool TypeSymbol::IsSubinterface(const TypeSymbol* super_interface) const
{
	if (this == super_interface)
		return true;
	for (unsigned i = 0; i < NumInterfaces(); i++)
	{
		if (Interface(i)->IsSubinterface(super_interface))
			return true;
	}
	return false;
}

//
// This test works for classes, but not for interfaces. See also IsSubtype.
//
inline bool TypeSymbol::Implements(const TypeSymbol* inter) const
{
	for (unsigned i = 0; i < NumInterfaces(); i++)
	{
		if (Interface(i)->IsSubinterface(inter))
			return true;
	}
	return super && super->Implements(inter);
}

//
// The most generic subtype relation; returns true if this type is a
// subtype of the argument type. This correctly checks a class's
// superclasses and superinterfaces, an interfaces's superinterfaces and
// Object, and an array's compatible types (smaller dimensions of Object,
// Cloneable, Serializable, and all equal dimension arrays where the
// element type is a subtype). For simplicity, a type subtypes itself.
//
inline bool TypeSymbol::IsSubtype(const TypeSymbol* type) const
{
	if (ACC_INTERFACE())
		return (type->ACC_INTERFACE() && IsSubinterface(type)) ||
		super == type;
	if (num_dimensions)
	{
		const TypeSymbol* base =
			type->base_type ? type->base_type : type;
		return (num_dimensions > type->num_dimensions &&
			((base->ACC_INTERFACE() && Implements(base)) ||
				super == base)) ||
			(num_dimensions == type->num_dimensions &&
				base_type->IsSubtype(base));
	}
	return type->ACC_INTERFACE() ? Implements(type) : IsSubclass(type);
}

inline wchar_t* TypeSymbol::FileLoc()
{
	return file_location ? file_location->location : (wchar_t*)NULL;
}

inline TypeSymbol* TypeSymbol::ArraySubtype() const
{
	assert(num_dimensions);
	return base_type->Array(num_dimensions - 1);
}

inline void TypeSymbol::SetClassLiteralName(Utf8LiteralValue* class_literal_name_)
{
	class_literal_name = class_literal_name_;
}

inline bool TypeSymbol::IsInner() const
{
	assert((!IsLocal() && !Anonymous()) ||
		(IsNested() && !ACC_STATIC()));
	return IsNested() && !ACC_STATIC();
}


inline const char* TypeSymbol::ClassName()
{
	if (class_name.empty())
		SetClassName();
	return class_name.c_str();
}
inline string TypeSymbol::getClassNameString()
{
	ClassName();

	return class_name;
}

inline void TypeSymbol::MarkConstructorMembersProcessed()
{
	status |= CONSTRUCTOR_MEMBERS_PROCESSED;
}
inline bool TypeSymbol::ConstructorMembersProcessed() const
{
	return (status & CONSTRUCTOR_MEMBERS_PROCESSED) != 0;
}

inline void TypeSymbol::MarkMethodMembersProcessed()
{
	status |= METHOD_MEMBERS_PROCESSED;
}
inline bool TypeSymbol::MethodMembersProcessed() const
{
	return (status & METHOD_MEMBERS_PROCESSED) != 0;
}

inline void TypeSymbol::MarkFieldMembersProcessed()
{
	status |= FIELD_MEMBERS_PROCESSED;
}
inline bool TypeSymbol::FieldMembersProcessed() const
{
	return (status & FIELD_MEMBERS_PROCESSED) != 0;
}

inline void TypeSymbol::MarkLocalClassProcessingCompleted()
{
	status |= LOCAL_CLASS_PROCESSING_COMPLETED;
}
inline bool TypeSymbol::LocalClassProcessingCompleted() const
{
	return (status & LOCAL_CLASS_PROCESSING_COMPLETED) != 0;
}

inline void TypeSymbol::MarkBad()
{
	SetACC_PUBLIC();
	status |= (BAD | HEADER_PROCESSED | CONSTRUCTOR_MEMBERS_PROCESSED |
		METHOD_MEMBERS_PROCESSED | FIELD_MEMBERS_PROCESSED |
		LOCAL_CLASS_PROCESSING_COMPLETED);
	MarkSourceNoLongerPending();
}

inline void TypeSymbol::MarkCircular()
{
	status |= CIRCULAR;
	MarkBad();
}

inline unsigned TypeSymbol::NumNestedTypeSignatures()
{
	return nested_type_signatures
		? nested_type_signatures->Length() : 0;
}
inline char* TypeSymbol::NestedTypeSignature(unsigned i)
{
	return (*nested_type_signatures)[i];
}
inline void TypeSymbol::AddNestedTypeSignature(const char* signature_, unsigned length)
{
	char* signature = new char[length + 1];
	strncpy(signature, signature_, length);
	signature[length] = U_NULL;

	if (!nested_type_signatures)
		nested_type_signatures = new Tuple<char*>(8);
	nested_type_signatures->Next() = signature;
}


inline unsigned TypeSymbol::NumArrays()
{
	return array ? array->Length() : 0;
}
inline TypeSymbol* TypeSymbol::Array(unsigned i)
{
	return (*array)[i];
}




} // Close namespace Jikes block


#endif // symbol_INCLUDED

