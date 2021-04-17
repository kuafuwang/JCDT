#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/semantic/SemanticEnvironment.h>
#include <JCDT_Lib/internal/semantic/Semantic.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/lookup/ExpandedTypeTable.h>
#include <JCDT_Lib/internal/lookup/ExpandedFieldTable.h>
#include <JCDT_Lib/internal/lookup/ExpandedMethodTable.h>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lookup/BlockSymbol.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <boost/filesystem.hpp>
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/TypeHeaderInfo.h>

namespace Jikes { // Open namespace Jikes block


	 TypeSymbol* TypeSymbol::InsertAnonymousTypeSymbol(NameSymbol* name_symbol)
	{
		return Table()->InsertAnonymousTypeSymbol(name_symbol);
	}



	 TypeSymbol* TypeSymbol::InsertNestedTypeSymbol(NameSymbol* name_symbol)
	{
		return Table()->InsertTypeSymbol(name_symbol);
	}


	 void TypeSymbol::DeleteAnonymousTypes()
	{
		if(anonymous_types){
			for(auto it : *anonymous_types){
				it->CleanUpType();
			}
		}
		delete anonymous_types;
		anonymous_types = NULL;
		if (table)
			table->DeleteAnonymousTypes();
	}


	 TypeSymbol* TypeSymbol::FindTypeSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindTypeSymbol(name_symbol)
			: (TypeSymbol*)NULL;
	}


	 MethodSymbol* TypeSymbol::InsertMethodSymbol(const NameSymbol* name_symbol)
	{
		return Table()->InsertMethodSymbol(new MethodSymbol(name_symbol));
	}


	 void TypeSymbol::InsertMethodSymbol(MethodSymbol* method_symbol)
	{
		Table()->InsertMethodSymbol(method_symbol);
	}




	 MethodSymbol* TypeSymbol::FindMethodSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindMethodSymbol(name_symbol)
			: (MethodSymbol*)NULL;
	}


	 MethodSymbol* TypeSymbol::FindOverloadMethod(MethodSymbol* base_method,
		AstMethodDeclarator* method_declarator)
	{
		return table ? table->FindOverloadMethod(base_method, method_declarator)
			: (MethodSymbol*)NULL;
	}


	 VariableSymbol* TypeSymbol::InsertVariableSymbol(const NameSymbol* name_symbol)
	{
		return Table()->InsertVariableSymbol(name_symbol);
	}


	 void TypeSymbol::InsertVariableSymbol(VariableSymbol* variable_symbol)
	{
		Table()->InsertVariableSymbol(variable_symbol);
	}


	 VariableSymbol* TypeSymbol::FindVariableSymbol(const NameSymbol* name_symbol)
	{
		return table ? table->FindVariableSymbol(name_symbol)
			: (VariableSymbol*)NULL;
	}


	 void TypeSymbol::SetSymbolTable(unsigned estimate)
	{
		// If table was not yet allocated, allocate one based on the estimate
		if (!table)
			table = new BindingSymbolTable(estimate);
	}

	 BindingSymbolTable* TypeSymbol::Table()
	{
		return table ? table : table = new BindingSymbolTable();
	}


	 unsigned TypeSymbol::NumMethodSymbols()
	{
		return table ? table->NumMethodSymbols() : 0;
	}
	 MethodSymbol* TypeSymbol::MethodSym(unsigned i)
	{
		return table->MethodSym(i);
	}

	 unsigned TypeSymbol::NumTypeSymbols()
	{
		return table ? table->NumTypeSymbols() : 0;
	}
	 TypeSymbol* TypeSymbol::TypeSym(unsigned i)
	{
		return table->TypeSym(i);
	}

	 void TypeSymbol::CompressSpace()
	{
		if (table)
			table->CompressSpace();
	}


	 unsigned TypeSymbol::NumVariableSymbols()
	{
		return table ? table->NumVariableSymbols() : 0;
	}
	 VariableSymbol* TypeSymbol::VariableSym(unsigned i)
	{
		return table->VariableSym(i);
	}
	 bool TypeSymbol::IsOwner(TypeSymbol* type)
	{
		Symbol* sym = type->owner;
		while (!PackageSymbol::PackageCast(sym))
		{
			if (sym == this)
				return true;

			MethodSymbol* method = MethodSymbol::MethodCast(sym);
			sym = (method ? method->containing_type
				: ((TypeSymbol*)sym)->owner);
		}
		return false;
	}
	 TypeSymbol* TypeSymbol::ContainingType()
	{
		if (owner)
		{
			TypeSymbol* type = TypeSymbol::TypeCast(owner);
			if (type)
				return type;
			MethodSymbol* method = MethodSymbol::MethodCast(owner);
			if (method)
				return method->containing_type;
		}
		return NULL;
	}
	 const TypeSymbol* TypeSymbol::ContainingType() const
	{
		if (owner)
		{
			TypeSymbol* type = TypeSymbol::TypeCast(owner);
			if (type)
				return type;
			MethodSymbol* method = MethodSymbol::MethodCast(owner);
			if (method)
				return method->containing_type;
		}
		return NULL;
	}


	 bool TypeSymbol::IsLocal() const
	{
		for (Symbol* sym = owner;
		!PackageSymbol::PackageCast(sym);
			sym = ((TypeSymbol*)sym)->owner)
		{
			if (MethodSymbol::MethodCast(sym))
				return true;
		}
		return false;
	}

	void TypeSymbol::RemoveClassFileFromOutputDirWhenInJavaFile()
	{
		if(file_symbol){
			if(file_symbol->IsJava()){
				auto class_file_name = getClassNameString();
				boost::system::error_code ec;
				boost::filesystem::remove(class_file_name, ec);
			}
		}
	}
	

	wstring TypeSymbol::qualifiedSourceName(wchar_t separate)
	{
		if(IsArray())
		{
			wstring brackets;
			for(auto i = 0 ; i< num_dimensions; ++i)
			{
				brackets.append(L"[]");
			}
			return base_type->qualifiedSourceName(separate) + brackets;
		}
	
		auto  enclose_type = EnclosingType();
		if(enclose_type)
		{
			auto part_name = enclose_type->qualifiedSourceName(separate);
			part_name.push_back(separate);
			return	part_name + sourceName(separate);
		}
		else if(outermost_type && IsNested())
		{
			auto part_name = outermost_type->qualifiedSourceName(separate);
			part_name.push_back(separate);
			return	part_name + sourceName(separate);
		}
		else
		{
			return sourceName(separate);
		}
	}

	 wstring TypeSymbol::readableName(wchar_t separate)
	 {
		 if (IsLocal())
		 {
			 if (NumInterfaces() == 0)
				 return (L"<binding.subclass" + super->readableName(separate) + L">");
				 //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-1$
			 else
				 return (L"<binding.implementation" + Interface(0)->readableName(separate) + L">");
			 //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-1$
		 }
		 else if (EnclosingType())
		 {
			 auto part_name = EnclosingType()->readableName(separate);
			 part_name.push_back(separate);
			 return part_name + getNameString();
		 }
		 else
		 {
			 return getNameString();
		 }
	 }

	PackageSymbol* TypeSymbol::ContainingPackage() const
	{

		return PackageSymbol::PackageCast(outermost_type->owner);
	}
	// Returns the fully-qualified '/' separated package name.
	 const wchar_t* TypeSymbol::ContainingPackageName() const
	{

		return PackageSymbol::PackageCast(outermost_type->owner)->PackageName();
	}

	 const wchar_t* TypeSymbol::getCompoundName()
	{
		if (compoundName)
			return compoundName->c_str();
		compoundName = new wstring();
		wstring pkg = PackageSymbol::PackageCast(outermost_type->owner)->PackageName();

		if(!pkg.empty()){
			pkg += L"/";
		}

		pkg += qualifiedSourceName('/');
		compoundName->swap(pkg);
		return compoundName->c_str();
	}

	void TypeSymbol::ProcessTypeHeaders()
	{
		semantic_environment->sem->ProcessTypeHeaders(declaration);
	}

	void TypeSymbol::ProcessMembers()
	{
		semantic_environment->sem->ProcessMembers(declaration);
	}

	void TypeSymbol::CompleteSymbolTable()
	{
		semantic_environment->sem->CompleteSymbolTable(declaration);
	}


	void TypeSymbol::ProcessExecutableBodies()
	{
		
		semantic_environment->sem->ProcessExecutableBodies(declaration);
	}
	bool TypeSymbol::canBeSeenBy(TypeSymbol* receiverType, TypeSymbol* invocationType) {
		if (ACC_PUBLIC()) return true;

		if (invocationType == this && invocationType == receiverType) return true;

		if (ACC_PRIVATE()) {

			// answer true if the invocationType is the declaringClass or they are in the same package
			// OR the invocationType is a subclass of the declaringClass
			//    AND the invocationType is the invocationType or its subclass
			//    OR the type is a static method accessed directly through a type
			//    OR previous assertions are true for one of the enclosing type
			if (invocationType == this)
				return true;
			if (invocationType->package == package) return true;

			auto currentType = invocationType;
			auto  declaringClass = EnclosingType(); // protected types always have an enclosing one
			if (declaringClass == nullptr) return false; // could be null if incorrect top-level protected type
													  //int depth = 0;
			do {
				if (declaringClass == invocationType)
					return true;
				if (currentType->IsSubclass(declaringClass)) return true;

				//depth++;
				currentType = currentType->EnclosingType();
			} while (currentType != nullptr);
			return false;
		}

		if (ACC_PRIVATE()) {
			// answer true if the receiverType is the receiver or its enclosingType
			// AND the invocationType and the receiver have a common enclosingType
			if (!(receiverType == this || receiverType == EnclosingType())) return false;

			if (invocationType != this) {
				auto outerInvocationType = invocationType;
				auto temp = outerInvocationType->EnclosingType();
				while (temp != nullptr) {
					outerInvocationType = temp;
					temp = temp->EnclosingType();
				}

				auto outerDeclaringClass = this;
				temp = outerDeclaringClass->EnclosingType();
				while (temp != nullptr) {
					outerDeclaringClass = temp;
					temp = temp->EnclosingType();
				}
				if (outerInvocationType != outerDeclaringClass) return false;
			}
			return true;
		}

		// isDefault()
		if (invocationType->package != package) return false;

		auto type = receiverType;
		auto declaringClass = EnclosingType() == nullptr ? this : EnclosingType();
		do {
			if (declaringClass == type) return true;
			if (package != type->package) return false;
		} while ((type = type->super) != nullptr);
		return false;
	}

	void TypeSymbol::RemoveCompilationReferences()
	{
		if (semantic_environment)
		{
			semantic_environment = NULL;
			declaration = NULL;

			// TODO: What else needs to be reset?
			if (table)
			{
				unsigned i;
				for (i = 0; i < table->NumVariableSymbols(); i++)
					table->VariableSym(i)->declarator = NULL;
				for (i = 0; i < table->NumMethodSymbols(); i++)
					table->MethodSym(i)->declaration = NULL;
				for (i = 0; i < table->NumTypeSymbols(); i++)
					table->TypeSym(i)->declaration = NULL;
				for (i = 0; i < table->NumAnonymousSymbols(); i++)
					table->AnonymousSym(i)->declaration = NULL;
			}
		}
	}


	TypeSymbol* TypeSymbol::GetArrayType(Semantic* sem, unsigned dims)
	{
		if (dims == num_dimensions)
			return this;
		if (num_dimensions)
			return base_type->GetArrayType(sem, dims);
		if (!dims || Bad())
			return this;
		if (dims < NumArrays())
			return Array(dims);

		if (NumArrays() == 0)
			AddArrayType(this);
		TypeSymbol* previous_array_type = Array(array->Length() - 1);
		wchar_t* name = new wchar_t[ExternalNameLength() + (dims * 2) + 1];
		wcscpy(name, previous_array_type->ExternalName());

		for (unsigned num = array->Length(),
			len = previous_array_type->ExternalNameLength() + 2;
			num <= dims;
			num++, len = len + 2)
		{
			wcscat(name, StringConstant::US_LB_RB);
			NameSymbol* name_sym = sem->lookup_env.nameEnv->FindOrInsertName(name, len);
			TypeSymbol* type = new TypeSymbol(name_sym);

			type->MarkHeaderProcessed();
			type->MarkConstructorMembersProcessed();
			type->MarkMethodMembersProcessed();
			type->MarkFieldMembersProcessed();
			type->MarkLocalClassProcessingCompleted();
			type->MarkSourceNoLongerPending();
			type->outermost_type = type;

			//
			// An array type has the same accessibility as its component.
			//
			if (ACC_PUBLIC())
				type->SetACC_PUBLIC();
			else if (ACC_PROTECTED())
				type->SetACC_PROTECTED();
			else if (ACC_PRIVATE())
				type->SetACC_PRIVATE();
			type->SetACC_FINAL();

			type->super = sem->lookup_env.Object();
			//
			// All arrays implement the interfaces java.io.Serializable and
			// java.io.Cloneable
			//
			
			//type->AddInterface(sem->lookup_env.Serializable());

			//type->AddInterface(sem->lookup_env.Cloneable());
			type->base_type = this;
			type->num_dimensions = num;
			type->SetOwner(ContainingPackage());
			// only 2 elements will be added to this table
			type->table = new BindingSymbolTable(2);
			type->SetSignature(sem->lookup_env);

			MethodSymbol* method =
				type->InsertMethodSymbol(sem->lookup_env.nameEnv->clone_name_symbol);
			method->SetType(sem->lookup_env.Object());
			method->SetContainingType(type);
			method->SetFlags(ACCESS_PUBLIC | ACCESS_FINAL);
			// the associated symbol table will remain empty
			method->SetBlockSymbol(new BlockSymbol(1));
			method->SetSignature(sem->lookup_env);

			VariableSymbol* symbol =
				type->InsertVariableSymbol(sem->lookup_env.nameEnv->length_name_symbol);
			symbol->SetFlags(ACCESS_PUBLIC | ACCESS_FINAL);
			symbol->SetOwner(type);
			symbol->SetType(sem->lookup_env.int_type);
			symbol->MarkComplete();
			symbol->MarkInitialized();

			type->CompressSpace(); // space optimization
			AddArrayType(type);
		}

		delete[] name;
		return Array(dims);
	}

	void TypeSymbol::SetLocation()
	{
		if (!declaration )// 匿名类没有标识
			file_location = new FileLocation(file_symbol);
		else
		{
			file_location =
				new FileLocation(semantic_environment->sem->lex_stream,
					declaration->identifier_token);
		}
	}

	TypeSymbol* TypeSymbol::leafComponentType()
	{
		if(!base_type || base_type == this)
		{
			return this;
		}
		else 
			return base_type->leafComponentType();
	}

	void TypeSymbol::SetSignature(LookupEnvironment& control)
	{
		if (num_dimensions > 0)
		{
			char* type_signature;
			TypeSymbol* subtype = ArraySubtype();
			// +1 for '['
			int signature_len = strlen(subtype->SignatureString()) + 1;
			type_signature = new char[signature_len + 1];
			type_signature[0] = U_LEFT_BRACKET;
			strcpy(type_signature + 1, subtype->SignatureString());
			signature = control.nameEnv->Utf8_pool.FindOrInsert(type_signature,
				signature_len);
			fully_qualified_name = signature;
			delete[] type_signature;
		}
		else
		{
			const wchar_t* package_name = ContainingPackageName();
			const wchar_t* type_name = ExternalName();

			// +1 for 'L' +1 for '/' +1 for ';' +1 for '\0'
			int len = ContainingPackage()->PackageNameLength() +
				ExternalNameLength() + 4;
			wchar_t* type_signature = new wchar_t[len];
			type_signature[0] = U_L;
			type_signature[1] = U_NU;
			if (ContainingPackage()->PackageNameLength() > 0 &&
				wcscmp(package_name, StringConstant::US_DOT) != 0)
			{
				wcscat(type_signature, package_name);
				wcscat(type_signature, StringConstant::US_SL);
			}
			wcscat(type_signature, type_name);
			// +1 to skip the initial L'L'
			fully_qualified_name =
				control.nameEnv->ConvertUnicodeToUtf8(type_signature + 1);

			wcscat(type_signature, StringConstant::US_SC);
			signature = control.nameEnv->ConvertUnicodeToUtf8(type_signature);

			delete[] type_signature;

			if (!(Anonymous() || IsLocal()))
				control.type_table.InsertType(this);
		}
	}




	TypeSymbol::TypeSymbol(const NameSymbol* name_symbol_)
		: semantic_environment(NULL),
		declaration(NULL),
		file_symbol(NULL),
		file_location(NULL),
		name_symbol(name_symbol_),
		owner(NULL),
		outermost_type(NULL), compoundName(nullptr),
		  super(NULL),
		base_type(NULL),
		index(TypeCycleChecker::OMEGA),
		unit_index(TypeCycleChecker::OMEGA),
		incremental_index(TypeCycleChecker::OMEGA),
		local(NULL),
		non_local(NULL),
		supertypes_closure(NULL),
		subtypes(new SymbolSet),
		subtypes_closure(NULL),
		innertypes_closure(NULL),
		dependents(new SymbolSet()),
		parents(new SymbolSet()),
		static_parents(new SymbolSet()),
		dependents_closure(NULL),
		parents_closure(NULL), pool_index(0),
		signature(NULL),
		fully_qualified_name(NULL),
		expanded_type_table(NULL),
		expanded_field_table(NULL),
		expanded_method_table(NULL),
		num_dimensions(0),
		instance_initializer_method(NULL),
		static_initializer_method(NULL), hash_address(0), next_type(nullptr),
		external_name_symbol(NULL),
		table(NULL),
		local_shadow_map(NULL),
		status(0),
		package(NULL),
		class_name(),
		class_literal_method(NULL),
		class_literal_name(NULL),
		assert_variable(NULL),
		local_constructor_call_environments(NULL),
		private_access_methods(NULL),
		private_access_constructors(NULL),
		read_methods(NULL),
		write_methods(NULL),
		placeholder_type(NULL),
		constructor_parameters(NULL),
		enclosing_instance(NULL),
		class_literals(NULL),
		nested_type_signatures(NULL),
		nested_types(NULL),
		interfaces(NULL),
		anonymous_types(NULL),
		array(NULL)
	{
		Symbol::_kind = TYPE;
	}

	unsigned TypeSymbol::NumLocalTypes()
	{
		return local ? local->Size() : 0;
	}

	void TypeSymbol::CleanUpType()
	{
		
		this->DeleteAnonymousTypes();
		for (unsigned i = 0; i < this->NumNestedTypes(); i++){
			auto nest_type = this->NestedType(i);
			nest_type->CleanUpType();
		}
		UnlinkFromParents();

		this->CompressSpace(); // space optimization

		for (unsigned j = 0; j < this->NumMethodSymbols(); j++)
			this->MethodSym(j)->CleanUp();

		delete this->local;

		this->local = NULL;

		delete this->non_local;
		this->non_local = NULL;

		delete this->semantic_environment;
		this->semantic_environment = NULL;

		this->declaration = NULL;
	}

	void TypeSymbol::getTypesInfo(set<wstring>& path_set, vector<TypeHeaderInfo*>&type_header_infos)
	{
		if(!file_symbol || !file_symbol->IsJava()){
			return;
		}

		if (anonymous_types) {
			for (auto it : *anonymous_types) {
				it->getTypesInfo(path_set, type_header_infos);
			}
		}
		if(nested_types){
			for (auto it : *nested_types) {
				it->getTypesInfo(path_set, type_header_infos);
			}
		}

		path_set.insert( CharOperation::ConvertUtf8ToUnicode(getClassNameString()));
	
		wstring pkg = ContainingPackageName();
		boost::replace_all(pkg, L"/", L".");
		wstring typeName = qualifiedSourceName('.');
		TypeHeaderInfo* info = new TypeHeaderInfo(pkg, typeName);
		type_header_infos.emplace_back(info);
		if(super)
		{
			pkg = super->ContainingPackageName();
			boost::replace_all(pkg, L"/", L".");
			typeName = super->qualifiedSourceName('.');
			info->superName = new std::pair<wstring, wstring>(pkg, typeName);
		}
		else
		{
			pkg = L"java.lang";
			typeName = L"Object";
			info->superName = new std::pair<wstring, wstring>(pkg, typeName);
		}
		if(interfaces)
		{
			info->interfaceNames = new std::vector<std::pair<wstring, wstring> >();
			for(auto it : *interfaces)
			{
				pkg = it->ContainingPackageName();
				boost::replace_all(pkg, L"/", L".");
				typeName = it->qualifiedSourceName('.');
				info->interfaceNames->emplace_back(make_pair(pkg, typeName));
			}
		}
	}

	TypeSymbol::~TypeSymbol()
	{
		unsigned i;
		delete read_methods;
		delete write_methods;
		delete semantic_environment;
		delete local;
		delete non_local;
		delete supertypes_closure;
		delete subtypes;
		delete subtypes_closure;
		delete innertypes_closure;
		delete dependents;
		delete parents;
		delete static_parents;
		delete table;
		delete local_shadow_map;
		delete expanded_type_table;
		delete expanded_field_table;
		delete expanded_method_table;
		delete file_location;
		
		for (i = 1; i < NumArrays(); i++)
			delete Array(i);
		for (i = 0; i < NumNestedTypeSignatures(); i++)
			delete[] NestedTypeSignature(i);
		delete nested_type_signatures;

		delete local_constructor_call_environments;
		delete private_access_methods;
		delete private_access_constructors;
		delete constructor_parameters;
		delete class_literals;
		delete nested_types;
		delete interfaces;
		delete anonymous_types;
		delete array;
		delete compoundName;
	}


	void TypeSymbol::UnlinkFromParents()
	{

		set<TypeSymbol*> type_trash_set;
		if (super)
		{
			super->subtypes->RemoveElement(this);
			super->dependents->RemoveElement(this);
			super = nullptr;
			type_trash_set.insert(super);
		}
		if (interfaces)
		{
			for (unsigned i = 0; i < NumInterfaces(); ++i)
			{
				TypeSymbol* interf = Interface(i);
				interf->subtypes->RemoveElement(this);
				interf->dependents->RemoveElement(this);
				type_trash_set.insert(interf);
			}
			
			interfaces->clear();
			
		}
		if(static_parents)
		{
			TypeSymbol* parent;
			for (parent = (TypeSymbol*)static_parents->FirstElement();
				parent;
				parent = (TypeSymbol*)static_parents->NextElement()){
					parent->dependents->RemoveElement(this);
					parent->subtypes->RemoveElement(this);
			}
			 static_parents->SetEmpty();
			
		}
	
		if(parents)
		{
			TypeSymbol* parent;
			for (parent = (TypeSymbol*)parents->FirstElement();
				parent;
				parent = (TypeSymbol*)parents->NextElement())
			{
				
				parent->dependents->RemoveElement(this);
				parent->subtypes->RemoveElement(this);
				
			}
			parents->SetEmpty();
		}
		
	}

	void TypeSymbol::availableMethods(vector<MethodSymbol*>& methods)
	{
		//if( table ? table->NumMethodSymbols() : 0;
		auto len = NumMethodSymbols();
		methods.reserve(len);
		for(auto i = 0 ; i   < len ; ++ i)
		{
			methods.push_back( table->MethodSym(i) );
		}
	}

	void TypeSymbol::availableFields(vector<VariableSymbol*>& fields)
	{
		auto len = NumVariableSymbols();
		fields.reserve(len);
		for (auto i = 0; i < len; ++i){
			fields.push_back(table->VariableSym(i));
		}
	}

	bool TypeSymbol::IsNestedIn(TypeSymbol* type)
	{
		for (SemanticEnvironment* env = semantic_environment;
		env; env = env->previous)
		{
			if (env->Type() == type)
				return true;
		}
		return false;
	}


	//
	// Return the type of an enclosing instance, if this is an inner class
	// which is not in a static context. For anonymous and local classes, the
	// compiler necessarily built them from source, so enclosing_instance will
	// be properly set. Non-static nested classes, however, could have been
	// read from a .class file, hence the need for the second half of the ||.
	//
	TypeSymbol* TypeSymbol::EnclosingType()
	{
		if (enclosing_instance || (IsInner() && !Anonymous() && !IsLocal()))
		{
			assert(ContainingType());
			return ContainingType();
		}
		return NULL;
	}


	//
	// Check if this type has access to an enclosing instance of the named type.
	// If exact is true, the enclosing instance must be the specified type,
	// otherwise it is the innermost instance which is a subclass of type.
	//
	bool TypeSymbol::HasEnclosingInstance(TypeSymbol* type, bool exact)
	{
		assert(semantic_environment);
		for (SemanticEnvironment* env = semantic_environment;
		env; env = env->previous)
		{
			if (exact ? (env->Type() == type)
				: (env->Type()->IsSubclass(type)))
			{
				//
				// We found the innermost candidate type, now see if it is an
				// enclosing type that is fully initialized.
				//
				return !env->StaticRegion();
			}
			if (env->Type()->ACC_STATIC()) // No more enclosing levels exist.
				return false;
		}
		return false; // The requested type does not enclose this type.
	}


	//
	// Given two types T and T2 in different packages, the type T can access
	// protected members of T2 iff T or any class in which T is lexically enclosed
	// is a subclass of T2 or of some other type T3 that lexically encloses T2.
	//
	// Of course, T2 and all its enclosing classes, if any, must have been declared
	// either public or protected, otherwise they could not be eligible as a
	// superclass candidate. We do not need to check for that condition here.
	//
	bool TypeSymbol::HasProtectedAccessTo(TypeSymbol* target_type)
	{
		assert(semantic_environment && !target_type->IsArray());

		// Loop through T and enclosing classes.
		for (SemanticEnvironment* env = semantic_environment;
		env; env = env->previous)
		{
			TypeSymbol* main_type = env->Type();
			// Loop through T2 and enclosing classes.
			for (TypeSymbol* type = target_type;
							type;
					type = TypeSymbol::TypeCast(type->owner) )
			{
				if (main_type->IsSubclass(type))
					return true;
			}
		}
		return false;
	}


	TypeSymbol* TypeSymbol::BoxedType(LookupEnvironment& control)
	{
		if (!Primitive())
			return this;
		if (this == control.int_type)
			return control.Integer();
		if (this == control.boolean_type)
			return control.Boolean();
		if (this == control.byte_type)
			return control.Byte();
		if (this == control.short_type)
			return control.Short();
		if (this == control.char_type)
			return control.Character();
		if (this == control.long_type)
			return control.Long();
		if (this == control.float_type)
			return control.Float();
		if (this == control.double_type)
			return control.Double();
		assert(this == control.void_type);
		return control.Void();
	}


	TypeSymbol* TypeSymbol::UnboxedType(LookupEnvironment& control)
	{
		if (ContainingPackage() != control.LangPackage())
			return this;
		if (this == control.Integer())
			return control.int_type;
		if (this == control.Boolean())
			return control.boolean_type;
		if (this == control.Byte())
			return control.byte_type;
		if (this == control.Short())
			return control.short_type;
		if (this == control.Character())
			return control.char_type;
		if (this == control.Long())
			return control.long_type;
		if (this == control.Float())
			return control.float_type;
		if (this == control.Double())
			return control.double_type;
		if (this == control.Void())
			return control.void_type;
		return this;
	}


	VariableSymbol* TypeSymbol::InsertThis0()
	{
		assert(IsInner() && ContainingType() &&
			!semantic_environment->previous->StaticRegion());

		LookupEnvironment& control = semantic_environment->sem->lookup_env;

		// No local shadows and no this$0 yet.
		assert(NumConstructorParameters() == 0 && !enclosing_instance);

		//
		// Create a this0 pointer for an inner class.
		//
		VariableSymbol* variable_symbol =
			InsertVariableSymbol(control.nameEnv->this_name_symbol);
		variable_symbol->SetType(ContainingType());
		variable_symbol->SetFlags(ACCESS_FINAL | ACCESS_SYNTHETIC);
		variable_symbol->SetOwner(this);
		variable_symbol->MarkComplete();
		variable_symbol->MarkInitialized();

		enclosing_instance = variable_symbol;
		return variable_symbol;
	}


	TypeSymbol* TypeSymbol::FindOrInsertClassLiteralClass()
	{
		//
		// Normally, the place-holder type for invoking private constructors can
		// be any type, because we just pass null along, avoiding static
		// initialization. But if we use the place-holder type to store the
		// class$() method, we must ensure it is a subclass of Object.
		//
		if (placeholder_type && (placeholder_type->super !=
			semantic_environment->sem->lookup_env.Object()))
			placeholder_type = NULL;
		return GetPlaceholderType();
	}


	MethodSymbol* TypeSymbol::FindOrInsertClassLiteralMethod(LookupEnvironment& control)
	{
		assert(!ACC_INTERFACE());
		if (!class_literal_method)
		{
			//
			// Note that max_variable_index is initialized to 2 (instead of 1),
			// even though the class literal method is static. The reason is that
			// in generating code for this method, a try statement with a catch
			// will be used. Therefore, an extra "local" slot is required for the
			// local Exception parameter of the catch clause. We do not fill in
			// the body of this method here, because bytecode.cpp can do a much
			// more optimal job later. The method has the signature:
			//
			// /*synthetic*/ static Class class$(String name, boolean array);
			//
			BlockSymbol* block_symbol = new BlockSymbol(2);
			block_symbol->max_variable_index = 2;

			class_literal_method = InsertMethodSymbol(control.nameEnv->class_name_symbol);
			class_literal_method->SetType(control.Class());
			class_literal_method->SetFlags(ACCESS_STATIC | ACCESS_SYNTHETIC);
			// No need to worry about strictfp, since this method avoids fp math
			class_literal_method->SetContainingType(this);
			class_literal_method->SetBlockSymbol(block_symbol);

			VariableSymbol* variable_symbol =
				block_symbol->InsertVariableSymbol(control.MakeParameter(1));
			variable_symbol->SetACC_SYNTHETIC();
			variable_symbol->SetType(control.String());
			variable_symbol->SetOwner(class_literal_method);
			variable_symbol->SetLocalVariableIndex(block_symbol ->
				max_variable_index++);
			variable_symbol->MarkComplete();
			class_literal_method->AddFormalParameter(variable_symbol);

			variable_symbol =
				block_symbol->InsertVariableSymbol(control.MakeParameter(2));
			variable_symbol->SetACC_SYNTHETIC();
			variable_symbol->SetType(control.boolean_type);
			variable_symbol->SetOwner(class_literal_method);
			variable_symbol->SetLocalVariableIndex(block_symbol ->
				max_variable_index++);
			variable_symbol->MarkComplete();
			class_literal_method->AddFormalParameter(variable_symbol);

			class_literal_method->SetSignature(control);
			semantic_environment->sem->AddDependence(this, control.Class());
		}
		return class_literal_method;
	}


	Utf8LiteralValue* TypeSymbol::FindOrInsertClassLiteralName(LookupEnvironment& control)
	{
		if (!class_literal_name)
		{
			int length = fully_qualified_name->length;
			char* slashed_name = fully_qualified_name->value;
			char* name = new char[length + 1];
			for (int i = 0; i < length; i++)
				name[i] = (slashed_name[i] == U_SLASH ? (wchar_t)U_DOT
					: slashed_name[i]);
			name[length] = U_NULL;
			class_literal_name = control.nameEnv->Utf8_pool.FindOrInsert(name, length);
			delete[] name;
		}
		return class_literal_name;
	}


	VariableSymbol* TypeSymbol::FindOrInsertClassLiteral(TypeSymbol* type)
	{
		assert(!type->Primitive() && !type->Anonymous());
		assert(!Primitive() && !IsArray());

		Semantic* sem = semantic_environment->sem;
		LookupEnvironment& control = sem->lookup_env;

		//
		// We must be careful that we do not initialize the class literal in
		// question, or any enclosing types. True inner classes can defer to their
		// enclosing class (since code in the inner class cannot be run without
		// the enclosing class being initialized), but static nested types get
		// their own class$ method and cache variables. Interfaces cannot have
		// non-public members, so if the innermost non-local type is an interface,
		// we use the placeholder class to hold the class$ magic.
		//
		TypeSymbol* owner = this;
		while (owner->IsInner())
			owner = owner->ContainingType();
		if (owner->ACC_INTERFACE())
			owner = outermost_type->FindOrInsertClassLiteralClass();
		owner->FindOrInsertClassLiteralMethod(control);

		NameSymbol* name_symbol = NULL;
		const char* signature = type->SignatureString();
		if (signature[0] == U_LEFT_BRACKET) // an array?
		{
			int array_length = control.nameEnv->array_name_symbol->NameLength();
			int length = strlen(signature) + array_length;
			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->array_name_symbol->Name());
			int i;
			int k;
			for (i = 0, k = array_length; signature[i] == U_LEFT_BRACKET; i++, k++)
				name[k] = U_DOLLAR;
			// Leave leading 'L', since there can be conflicts with primitive
			// array types otherwise
			for (wchar_t ch = signature[i++]; ch && ch != U_SEMICOLON;
			ch = signature[i++])
			{
				name[k++] = (ch == U_SLASH ? (wchar_t)U_DOLLAR : ch);
			}
			name[k] = U_NULL;
			name_symbol = control.nameEnv->FindOrInsertName(name, k);
			delete[] name;
		}
		else
		{
			assert(signature[0] == U_L); // a reference type
			int class_length = control.nameEnv->class_name_symbol->NameLength();
			int length = strlen(signature) + class_length;

			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->class_name_symbol->Name());
			int i = 1; // skip leading 'L'
			int k = class_length;
			name[k++] = U_DOLLAR;
			for (wchar_t ch = signature[i++]; ch && ch != U_SEMICOLON;
			ch = signature[i++])
			{
				name[k++] = (ch == U_SLASH ? (wchar_t)U_DOLLAR : ch);
			}
			name[k] = U_NULL;
			name_symbol = control.nameEnv->FindOrInsertName(name, k);
			delete[] name;
		}

		VariableSymbol* variable_symbol = owner->FindVariableSymbol(name_symbol);
		if (!variable_symbol)
		{
			//
			// Generate a caching variable (no need to make it private, so that
			// nested classes of interfaces can share it easily).
			//
			// Foo.Bar.class is cached in:
			//     /*synthetic*/ static Class class$Foo$Bar;
			// int[][].class is cached in:
			//     /*synthetic*/ static Class array$$I;
			// Blah[].class is cached in:
			//     /*synthetic*/ static Class array$LBlah;
			//
			variable_symbol = owner->InsertVariableSymbol(name_symbol);
			variable_symbol->SetType(control.Class());
			variable_symbol->SetFlags(ACCESS_STATIC | ACCESS_SYNTHETIC);
			variable_symbol->SetOwner(owner);
			variable_symbol->MarkComplete();

			owner->AddClassLiteral(variable_symbol);
		}
		return variable_symbol;
	}


	VariableSymbol* TypeSymbol::FindOrInsertAssertVariable()
	{
		if (!assert_variable)
		{
			assert(!(Primitive() || ACC_INTERFACE() || IsArray()));

			Semantic* sem = semantic_environment->sem;
			LookupEnvironment& control = sem->lookup_env;

			assert_variable = InsertVariableSymbol(control.nameEnv->assert_name_symbol);
			assert_variable->SetType(control.boolean_type);
			assert_variable->SetFlags(ACCESS_PRIVATE | ACCESS_STATIC |
				ACCESS_FINAL | ACCESS_SYNTHETIC);
			assert_variable->SetOwner(this);
			assert_variable->MarkComplete();
			assert_variable->MarkInitialized();

			//
			// We'll create the field initializer later in bytecode.cpp, but we
			// create the static initializer that will contain the field
			// initializer now, if it was not already created.
			//
			sem->GetStaticInitializerMethod();
		}
		return assert_variable;
	}


	VariableSymbol* TypeSymbol::FindOrInsertLocalShadow(VariableSymbol* local)
	{
		assert(IsLocal() && local->IsLocal());

		LookupEnvironment& control = semantic_environment->sem->lookup_env;
		VariableSymbol* variable = NULL;
		if (local_shadow_map)
			variable = (VariableSymbol*)local_shadow_map->Image(local);

		//
		// For a local/anonymous class, if it does not yet have a shadow for a
		// local variable that it needs access to, create one.
		//
		// For example:
		// class Outer {
		//   static void foo(final int i) {
		//     class Local {
		//       Local(int k) { k = i; }
		//     }
		//     new Local(1);
		//   }
		// }
		//
		// expands to:
		// class Outer {
		//   static void foo(final int i) {
		//     new Outer$1Local(1, i);
		//   }
		// }
		// class Outer$1Local {
		//   /*synthetic*/ final int val$i;
		//   Outer$1Local(int k, int i) {
		//     val$i = i;
		//     super();
		//     k = val$i;
		//   }
		// }
		//
		// This method creates Outer$1Local.val$i in the above example.  Notice
		// that JVMS 4.9.4 permits initialization of synthetic fields BEFORE the
		// explicit constructor invocation, even though it would not normally be
		// valid Java; this is necessary for the case when the superconstructor
		// calls a polymorphic method which references i.
		//
		// Note that we must mangle the shadow with val$, because of this case:
		// void foo(final int i) {
		//   class Local { int j = i; }
		//   new Local() { int i; };
		// }
		//
		// In 1.5 and later, we use the prefix "-" instead of "val$".
		//
		if (!variable)
		{
			int length = control.nameEnv->val_name_symbol->NameLength() +
				local->NameLength();
			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->val_name_symbol->Name());
			wcscat(name, local->Name());
			NameSymbol* name_symbol = control.nameEnv->FindOrInsertName(name, length);

			variable = InsertVariableSymbol(name_symbol);
			variable->SetType(local->Type());
			variable->SetFlags(ACCESS_FINAL | ACCESS_SYNTHETIC);
			variable->SetOwner(this);
			variable->MarkComplete();
			variable->MarkInitialized();

			if (ContainingType() == local->ContainingType())
				variable->accessed_local = local;
			else
			{
				assert(Anonymous() && !EnclosingType());
				variable->accessed_local = semantic_environment->sem ->
					FindLocalVariable(local, ContainingType());
			}
			AddConstructorParameter(variable);

			delete[] name;

			if (!local_shadow_map)
				local_shadow_map = new SymbolMap();
			local_shadow_map->Map(local, variable);
		}

#ifdef JIKES_DEBUG
		VariableSymbol* accessed;
		for (accessed = variable->accessed_local;
		accessed && accessed != local;
			accessed = accessed->accessed_local);
			assert(accessed);
#endif // JIKES_DEBUG
		return variable;
	}


	 void TypeSymbol::MapSymbolToReadMethod(Symbol* symbol,
		TypeSymbol* base_type,
		MethodSymbol* method)
	{
		if (!read_methods)
			// default size
			read_methods = new Map<Symbol, Map<TypeSymbol, MethodSymbol> >();

		Map<TypeSymbol, MethodSymbol>* map = read_methods->Image(symbol);
		if (!map)
		{
			map = new Map<TypeSymbol, MethodSymbol>(1); // small size
			read_methods->Add(symbol, map);
		}

		map->Add(base_type, method);
	}



	 void TypeSymbol::MapSymbolToWriteMethod(VariableSymbol* symbol,
		TypeSymbol* base_type,
		MethodSymbol* method)
	{
		if (!write_methods)
			write_methods = new Map<VariableSymbol,
			Map<TypeSymbol, MethodSymbol> >(); // default size

		Map<TypeSymbol, MethodSymbol>* map = write_methods->Image(symbol);
		if (!map)
		{
			map = new Map<TypeSymbol, MethodSymbol>(1); // small size
			write_methods->Add(symbol, map);
		}

		map->Add(base_type, method);
	}

	 MethodSymbol* TypeSymbol::WriteMethod(VariableSymbol* symbol,
		TypeSymbol* base_type)
	{
		if (write_methods)
		{
			Map<TypeSymbol, MethodSymbol>* map = write_methods->Image(symbol);
			if (map)
				return map->Image(base_type);
		}
		return NULL;
	}

	MethodSymbol* TypeSymbol::GetReadAccessMethod(MethodSymbol* member,
		TypeSymbol* base_type)
	{
		// accessing a method
		assert(member->Identity() !=
			semantic_environment->sem->lookup_env.nameEnv->init_name_symbol);

		TypeSymbol* containing_type = member->containing_type;
		if (!base_type)
			base_type = this;

		assert((member->ACC_PRIVATE() && this == containing_type) ||
			(member->ACC_PROTECTED() &&
				!semantic_environment->sem->ProtectedAccessCheck(containing_type)) ||
			(base_type == super && !member->ACC_STATIC()));

		MethodSymbol* read_method = ReadMethod(member, base_type);

		if (!read_method)
		{
			//
			// BaseType is the qualifying type of we are accessing.  If the method
			// is private, BaseType should be this type, but for protected
			// variables, BaseType should be a superclass or subclass of this type
			// that is not in this package.
			//
			// To access
			// "static Type name(Type1 p1, Type2 p2, ...) throws Exception;",
			// expand to:
			//
			// /*synthetic*/ static Type access$<num>(Type1 p1, Type2 p2, ...)
			// throws Exception
			// {
			//     return BaseType.name(p1, p2, ...);
			// }
			//
			// If we are accessing
			// "void name(Type1 p1, Type2 p2, ...) throws Throwable;",
			// expand to:
			//
			// /*synthetic*/ static void access$<num>(BaseType $0, Type1 p1,
			//                                        Type2 p2, ...)
			// throws Throwable
			// {
			//     $0.name(p1, p2, ...);
			//     return;
			// }
			//
			// In 1.5 and later, we use the prefix "-" instead of "access$".
			//
			Semantic* sem = semantic_environment->sem;
			assert(sem);

			LookupEnvironment& control = sem->lookup_env;
			AstStoragePool* ast_pool = sem->compilation_unit->ast_pool;

			IntToWstring value(NumPrivateAccessMethods());

			int length = control.nameEnv->access_name_symbol->NameLength() +
				value.Length();
			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->access_name_symbol->Name());
			wcscat(name, value.String());

			//
			// Use the location of the class name for all elements of this method.
			//
			auto loc = declaration->identifier_token;

			unsigned parameter_count = member->NumFormalParameters();

			//
			// Add the method instead of inserting it, so it is not an overload
			// candidate.
			//
			read_method = new MethodSymbol(control.nameEnv->FindOrInsertName(name, length));
			Table()->AddMethodSymbol(read_method);
			read_method->SetType(member->Type());
			read_method->SetFlags(ACCESS_STATIC | ACCESS_SYNTHETIC);
			if (member->ACC_STRICTFP())
				read_method->SetACC_STRICTFP();
			if (member->ACC_FINAL() || ACC_FINAL())
				read_method->SetACC_FINAL();
			read_method->SetContainingType(this);

			//
			// A read access method for a method has a formal parameter per
			// parameter of the member in question, plus one more if it is not
			// static.
			//
			BlockSymbol* block_symbol =
				new BlockSymbol(parameter_count +
					(member->ACC_STATIC() ? 0 : 1));
			block_symbol->max_variable_index = 0;
			read_method->SetBlockSymbol(block_symbol);
			for (unsigned j = 0; j < member->NumThrows(); j++)
				read_method->AddThrows(member->Throws(j));

			Expression* base;
			if (!member->ACC_STATIC() && base_type == super)
			{
				//
				// Special case - for Outer.super.m() where m() is an instance
				// method, we mark the field access as a super access, to
				// make sure we emit invokespecial instead of invokevirtual in
				// bytecode.cpp.  Notice that in this case,
				// ((Super) Outer.this).m() cannot generate an accessor method
				// (either m() is public or in the same package and thus already
				// accessible, or m is protected in a different package and
				// therefore inaccessible), so we don't have to worry about a
				// conflict in accessor methods for the same base type.
				//
				base = ast_pool->GenSuperExpression(loc);
			}
			else base = ast_pool->GenName(loc);

			AstArguments* args = ast_pool->GenArguments(loc, loc);
			args->AllocateArguments(parameter_count);

			AstMethodInvocation* method_invocation =
				ast_pool->GenMethodInvocation(loc);
			method_invocation->base_opt = base;
			method_invocation->arguments = args;
			method_invocation->symbol = member;

			AstMethodDeclarator* method_declarator =
				ast_pool->GenMethodDeclarator();
			method_declarator->identifier_token = loc;
			method_declarator->left_parenthesis_token = loc;
			method_declarator->right_parenthesis_token = loc;

			if (member->ACC_STATIC())
			{
				method_declarator->AllocateFormalParameters(parameter_count);
				base->symbol = base_type;
			}
			else
			{
				method_declarator->AllocateFormalParameters(parameter_count + 1);
				NameSymbol* instance_name = control.MakeParameter(1);

				VariableSymbol* instance =
					block_symbol->InsertVariableSymbol(instance_name);
				instance->SetACC_SYNTHETIC();
				instance->SetType(base_type == super ? this : base_type);
				instance->SetOwner(read_method);
				instance->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				instance->MarkComplete();
				read_method->AddFormalParameter(instance);
				base->symbol = (base_type == super
					? (Symbol*)super : (Symbol*)instance);
			}

			for (unsigned i = 0; i < parameter_count; i++)
			{
				VariableSymbol* parm = block_symbol ->
					InsertVariableSymbol(member->FormalParameter(i)->Identity());
				parm->SetACC_SYNTHETIC();
				parm->SetType(member->FormalParameter(i)->Type());
				parm->SetOwner(read_method);
				parm->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				parm->MarkComplete();
				if (control.IsDoubleWordType(parm->Type()))
					block_symbol->max_variable_index++;
				read_method->AddFormalParameter(parm);

				AstName* simple_name = ast_pool->GenName(loc);
				simple_name->symbol = parm;
				args->AddArgument(simple_name);
			}
			read_method->SetSignature(control);

			ReturnStatement* return_statement =
				ast_pool->GenReturnStatement();
			return_statement->return_token = loc;
			return_statement->semicolon_token = loc;
			return_statement->is_reachable = true;

			AstMethodBody* block = ast_pool->GenMethodBody();
			block->left_brace_token = loc;
			block->right_brace_token = loc;
			// the symbol table associated with this block will contain no element
			block->block_symbol = new BlockSymbol(0);
			block->is_reachable = true;

			if (member->Type() == control.void_type)
			{
				AstExpressionStatement* expression_statement =
					ast_pool->GenExpressionStatement();
				expression_statement->expression = method_invocation;
				expression_statement->semicolon_token_opt = loc;
				expression_statement->is_reachable = true;
				expression_statement->can_complete_normally = true;

				block->AllocateStatements(2);
				block->AddStatement(expression_statement);
			}
			else
			{
				return_statement->expression_opt = method_invocation;
				block->AllocateStatements(1);
			}
			block->AddStatement(return_statement);

			MethodDeclaration* method_declaration =
				ast_pool->GenMethodDeclaration();
			method_declaration->method_symbol = read_method;
			method_declaration->method_declarator = method_declarator;
			method_declaration->method_body_opt = block;
			block->owner_ast = method_declaration;

			read_method->declaration = method_declaration;
			read_method->accessed_member = member;
			MapSymbolToReadMethod(member, base_type, read_method);
			AddPrivateAccessMethod(read_method);

			delete[] name;
		}
		return read_method;
	}

	MethodSymbol* TypeSymbol::GetReadAccessConstructor(MethodSymbol* ctor)
	{
		//
		// Protected superconstructors are always accessible, and class instance
		// creation expressions can only invoke a protected constructor in the
		// current package, where an accessor is not needed. Also, anonymous
		// classes never have a private constructor.
		//
		assert((ctor->Identity() ==
			semantic_environment->sem->lookup_env.nameEnv->init_name_symbol) &&
			ctor->ACC_PRIVATE() && this == ctor->containing_type &&
			!Anonymous());

		MethodSymbol* read_method = ReadMethod(ctor, this);

		if (!read_method)
		{
			//
			// There are two cases for accessing a private constructor.  First, as
			// a superclass:
			//
			// class Outer {
			//     private Outer(Type1 $1, Type2 $2, ...) {}
			//     static class Inner extends Outer {
			//         Inner() { super(expr1, expr2, ...); }
			//     }
			// }
			//
			// We must create a synthetic place-holder class, and expand this to:
			// (TODO: can someone come up with a way to do this without a
			// placeholder class?)
			//
			// class Outer {
			//     private Outer(Type1 $1, Type2 $2, ...) {}
			//     /*synthetic*/ Outer(Outer$ $0, Type1 $1, Type2 $2, ...)
			//     {
			//         this($1, $2, ...);
			//     }
			// }
			// /*synthetic*/ class Outer$ {} // placeholder only
			// class Outer$Inner extends Outer {
			//     Outer$Inner() { super((Outer$) null, expr1, expr2, ...); }
			// }
			//
			// The other use is in class instance creation expressions (recall
			// that the default constructor for a private class is private):
			//
			// class Outer {
			//     private class Inner {}
			//     Inner i = new Inner();
			// }
			//
			// Here again, we create a place-holder class for now.  TODO:
			// alternatives have been proposed, such as using a static generator
			// method instead of an alternate constructor.
			//
			// class Outer {
			//     Outer$Inner i = new Outer$Inner(this, (Outer$) null);
			// }
			// /*synthetic*/ class Outer$ {} // placeholder only
			// class Outer$Inner {
			//     private final Outer this$0;
			//     private Outer$Inner(Outer $0) { super(); this$0 = $0; }
			//     /*synthetic*/ Outer$Inner(Outer $0, Outer$ $1) { this($0); }
			// }
			//
			Semantic* sem = semantic_environment->sem;
			assert(sem);
			//
			// A clone situation exists only when trying to determine a final
			// value for a field. As obtaining a final value does not need an
			// access method, we delay creating the accessor until out of the
			// clone (otherwise, the placeholder type might be incorrect).
			//
			if (sem->error && sem->error->InClone())
				return ctor;

		

			LookupEnvironment& control = sem->lookup_env;
			AstStoragePool* ast_pool = sem->compilation_unit->ast_pool;

			// +3 to allow for dummy parameter, local variable shadows
			BlockSymbol* block_symbol =
				new BlockSymbol(ctor->NumFormalParameters() + 3);

			//
			// Add the method instead of inserting it, so it is not an overload
			// candidate.
			//
			read_method = new MethodSymbol(control.nameEnv->init_name_symbol);
			Table()->AddMethodSymbol(read_method);
			read_method->SetType(this);
			read_method->SetContainingType(this);
			read_method->SetBlockSymbol(block_symbol);
			read_method->SetACC_SYNTHETIC();
			if (ctor->ACC_STRICTFP())
				read_method->SetACC_STRICTFP();

			for (unsigned j = 0; j < ctor->NumThrows(); j++)
				read_method->AddThrows(ctor->Throws(j));

			block_symbol->max_variable_index = 1;
			read_method->SetExternalIdentity(ctor->Identity());

			Ast* declaration = ctor->declaration;
			AstMethodDeclarator* declarator =
				((ConstructorDeclaration*)declaration)->method_declarator;
			assert(declarator);
			auto loc = declarator->identifier_token;

			AstMethodDeclarator* method_declarator =
				ast_pool->GenMethodDeclarator();
			method_declarator->identifier_token = loc;
			method_declarator->left_parenthesis_token =
				declarator->LeftToken();
			method_declarator->right_parenthesis_token =
				declarator->RightToken();

			AstArguments* args = ast_pool->GenArguments(loc, loc);
			args->AllocateArguments(ctor->NumFormalParameters());

			AstThisCall* this_call = ast_pool->GenThisCall();
			this_call->this_token = loc;
			this_call->arguments = args;
			this_call->semicolon_token = loc;
			this_call->symbol = ctor;

			VariableSymbol* this0_variable = NULL;
			if (EnclosingType())
			{
				this0_variable = block_symbol ->
					InsertVariableSymbol(control.nameEnv->this_name_symbol);
				this0_variable->SetACC_SYNTHETIC();
				this0_variable->SetType(ContainingType());
				this0_variable->SetOwner(read_method);
				this0_variable->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				this0_variable->MarkComplete();
			}

			//
			// Since private_access_constructors will be compiled (see
			// body.cpp), we must create valid ast_simple_names for its
			// parameters.
			//
			VariableSymbol* parm;
			for (unsigned i = 0; i < ctor->NumFormalParameters(); i++)
			{
				parm = block_symbol->InsertVariableSymbol(ctor->FormalParameter(i)->Identity());
				parm->SetACC_SYNTHETIC();
				parm->SetType(ctor->FormalParameter(i)->Type());
				parm->SetOwner(read_method);
				parm->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				parm->MarkComplete();
				if (control.IsDoubleWordType(parm->Type()))
					block_symbol->max_variable_index++;
				read_method->AddFormalParameter(parm);

				AstVariableDeclaratorId* variable_declarator_name =
					declarator->FormalParameter(i)->formal_declarator ->
					variable_declarator_name;
				AstName* simple_name = ast_pool ->
					GenName(variable_declarator_name->identifier_token);
				simple_name->symbol = parm;
				args->AddArgument(simple_name);
			}

			//
			// Any local variable shadow parameters will be taken care of later,
			// possibly changing this signature.
			//
			read_method->SetSignature(control,
				outermost_type->GetPlaceholderType());

			ReturnStatement* return_statement =
				ast_pool->GenReturnStatement();
			return_statement->return_token = loc;
			return_statement->semicolon_token = loc;
			return_statement->is_reachable = true;

			AstMethodBody* constructor_block = ast_pool->GenMethodBody();
			// This symbol table will be empty.
			constructor_block->block_symbol = new BlockSymbol(0);
			constructor_block->block_symbol->max_variable_index =
				block_symbol->max_variable_index;
			constructor_block->left_brace_token = loc;
			constructor_block->right_brace_token = loc;
			constructor_block->AllocateStatements(1);
			constructor_block->AddStatement(return_statement);
			constructor_block->explicit_constructor_opt = this_call;

			ConstructorDeclaration* constructor_declaration =
				ast_pool->GenConstructorDeclaration();
			constructor_declaration->method_declarator = method_declarator;
			constructor_declaration->constructor_body = constructor_block;
			constructor_block->owner_ast = constructor_declaration;

			constructor_declaration->method_symbol = read_method;
			read_method->declaration = constructor_declaration;

			AddPrivateAccessConstructor(read_method);

			read_method->accessed_member = ctor;
			MapSymbolToReadMethod(ctor, this, read_method);
		}
		return read_method;
	}


	MethodSymbol* TypeSymbol::GetReadAccessMethod(VariableSymbol* member,
		TypeSymbol* base_type)
	{
		TypeSymbol* containing_type =  TypeSymbol::TypeCast(member->owner);
		if (!base_type)
			base_type = this;

		assert((member->ACC_PRIVATE() && this == containing_type) ||
			(member->ACC_PROTECTED() &&
				(!semantic_environment->sem->ProtectedAccessCheck(containing_type) ||
					(base_type == super && !member->ACC_STATIC()))));

		MethodSymbol* read_method = ReadMethod(member, base_type);

		if (!read_method)
		{
			//
			// BaseType is the qualifying type of we are accessing.  If the
			// variable is private, BaseType should be this type, but for
			// protected variables, BaseType should be a superclass or subclass
			// of this type that is not in this package.
			//
			// If we are accessing "static Type name;", expand to:
			//
			// /*synthetic*/ static Type access$<num>()
			// {
			//     return BaseType.name;
			// }
			//
			// If we are accessing "Type name;", expand to:
			//
			// /*synthetic*/ static Type access$<num>(BaseType $1)
			// {
			//     return $1.name;
			// }
			//
			// In 1.5 and later, we use the prefix "-" instead of "access$".
			//
			Semantic* sem = semantic_environment->sem;
			assert(sem);

			LookupEnvironment& control = sem->lookup_env;
			AstStoragePool* ast_pool = sem->compilation_unit->ast_pool;

			IntToWstring value(NumPrivateAccessMethods());

			int length = control.nameEnv->access_name_symbol->NameLength() +
				value.Length();
			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->access_name_symbol->Name());
			wcscat(name, value.String());

			//
			// Use the location of the class name for all elements of this method.
			//
			auto loc = declaration->identifier_token;

			//
			// Add the method instead of inserting it, so it is not an overload
			// candidate.
			//
			read_method = new MethodSymbol(control.nameEnv->FindOrInsertName(name, length));
			Table()->AddMethodSymbol(read_method);
			read_method->SetType(member->Type());
			read_method->SetFlags(ACCESS_STATIC | ACCESS_SYNTHETIC);
			if (ACC_STRICTFP())
				read_method->SetACC_STRICTFP();
			if (ACC_FINAL())
				read_method->SetACC_FINAL();
			read_method->SetContainingType(this);

			//
			// A read access method for a field has 1 formal parameter if the
			// member in question is not static
			//
			BlockSymbol* block_symbol =
				new BlockSymbol(member->ACC_STATIC() ? 0 : 1);
			block_symbol->max_variable_index = 0;
			read_method->SetBlockSymbol(block_symbol);

			Expression* base;
			if (!member->ACC_STATIC() && base_type == super)
			{
				//
				// Special case - for Outer.super.i where i is an instance field,
				// we mark the field access as a super access, to make sure we use
				// the correct qualifying instance.  Notice that in this case,
				// ((Super) Outer.this).i cannot generate an accessor method
				// (either i is public or in the same package and thus already
				// accessible, or i is protected in a different package and
				// therefore inaccessible), so we don't have to worry about a
				// conflict in accessor methods for the same base type.
				//
				base = ast_pool->GenSuperExpression(loc);
			}
			else base = ast_pool->GenName(loc);

			FieldReference* field_access = ast_pool->GenFieldAccess();
			field_access->base = base;
			field_access->identifier_token = loc;
			field_access->symbol = member;

			AstMethodDeclarator* method_declarator =
				ast_pool->GenMethodDeclarator();
			method_declarator->identifier_token = loc;
			method_declarator->left_parenthesis_token = loc;
			method_declarator->right_parenthesis_token = loc;

			if (member->ACC_STATIC())
			{
				base->symbol = base_type;
			}
			else
			{
				method_declarator->AllocateFormalParameters(1);

				NameSymbol* instance_name = control.MakeParameter(1);

				VariableSymbol* instance =
					block_symbol->InsertVariableSymbol(instance_name);
				instance->SetACC_SYNTHETIC();
				instance->SetType(base_type == super ? this : base_type);
				instance->SetOwner(read_method);
				instance->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				instance->MarkComplete();
				read_method->AddFormalParameter(instance);
				base->symbol = (base_type == super
					? (Symbol*)super : (Symbol*)instance);
			}

			// A read access method has no throws clause !
			read_method->SetSignature(control);

			ReturnStatement* return_statement =
				ast_pool->GenReturnStatement();
			return_statement->return_token = loc;
			return_statement->expression_opt = field_access;
			return_statement->semicolon_token = loc;
			return_statement->is_reachable = true;

			AstMethodBody* block = ast_pool->GenMethodBody();
			block->left_brace_token = loc;
			block->right_brace_token = loc;
			block->block_symbol = new BlockSymbol(0);
			block->is_reachable = true;
			block->AllocateStatements(1);
			block->AddStatement(return_statement);

			MethodDeclaration* method_declaration =
				ast_pool->GenMethodDeclaration();
			method_declaration->method_symbol = read_method;
			method_declaration->method_declarator = method_declarator;
			method_declaration->method_body_opt = block;
			block->owner_ast = method_declaration;

			read_method->declaration = method_declaration;
			read_method->accessed_member = member;
			MapSymbolToReadMethod(member, base_type, read_method);
			AddPrivateAccessMethod(read_method);

			delete[] name;
		}
		return read_method;
	}


	MethodSymbol* TypeSymbol::GetWriteAccessMethod(VariableSymbol* member,
		TypeSymbol* base_type)
	{
		TypeSymbol* containing_type =  TypeSymbol::TypeCast(member->owner);
		if (!base_type)
			base_type = this;

		assert((member->ACC_PRIVATE() && this == containing_type) ||
			(member->ACC_PROTECTED() &&
				(!semantic_environment->sem->ProtectedAccessCheck(containing_type) ||
					(base_type == super && !member->ACC_STATIC()))));

		MethodSymbol* write_method = WriteMethod(member, base_type);

		if (!write_method)
		{
			//
			// BaseType is the qualifying type of we are accessing.  If the
			// variable is private, BaseType should be this type, but for
			// protected variables, BaseType should be a superclass or subclass
			// of this type that is not in this package.
			//
			// If we are accessing "static Type name;", expand to:
			//
			// /*synthetic*/ static void access$<num>(Type name)
			// {
			//     BaseType.name = name;
			//     return;
			// }
			//
			// If we are accessing "Type name;", expand to:
			//
			// /*synthetic*/ static void access$<num>(BaseType $1, Type name)
			// {
			//     $1.name = name;
			//     return;
			// }
			//
			// In 1.5 and later, we use the prefix "-" instead of "access$".
			//
			Semantic* sem = semantic_environment->sem;
			assert(sem);

			LookupEnvironment& control = sem->lookup_env;
			AstStoragePool* ast_pool = sem->compilation_unit->ast_pool;

			IntToWstring value(NumPrivateAccessMethods());

			int length = control.nameEnv->access_name_symbol->NameLength() +
				value.Length();
			wchar_t* name = new wchar_t[length + 1]; // +1 for '\0';
			wcscpy(name, control.nameEnv->access_name_symbol->Name());
			wcscat(name, value.String());

			//
			// Use the location of the class name for all elements of this method.
			//
			auto loc = declaration->identifier_token;

			//
			// Add the method instead of inserting it, so it is not an overload
			// candidate.
			//
			write_method =
				new MethodSymbol(control.nameEnv->FindOrInsertName(name, length));
			Table()->AddMethodSymbol(write_method);
			write_method->SetType(sem->lookup_env.void_type);
			write_method->SetFlags(ACCESS_STATIC | ACCESS_SYNTHETIC);
			if (ACC_STRICTFP())
				write_method->SetACC_STRICTFP();
			if (ACC_FINAL())
				write_method->SetACC_FINAL();
			write_method->SetContainingType(this);

			BlockSymbol* block_symbol =
				new BlockSymbol(member->ACC_STATIC() ? 1 : 2);
			block_symbol->max_variable_index = 0;
			write_method->SetBlockSymbol(block_symbol);

			Expression* base;
			if (!member->ACC_STATIC() && base_type == super)
			{
				//
				// Special case - for Outer.super.i where i is an instance field,
				// we mark the field access as a super access, to make sure we use
				// the correct qualifying instance.  Notice that in this case,
				// ((Super) Outer.this).i cannot generate an accessor method
				// (either i is public or in the same package and thus already
				// accessible, or i is protected in a different package and
				// therefore inaccessible), so we don't have to worry about a
				// conflict in accessor methods for the same base type.
				//
				base = ast_pool->GenSuperExpression(loc);
			}
			else base = ast_pool->GenName(loc);

			FieldReference* left_hand_side = ast_pool->GenFieldAccess();
			left_hand_side->base = base;
			left_hand_side->identifier_token = loc;
			left_hand_side->symbol = member;

			AstMethodDeclarator* method_declarator =
				ast_pool->GenMethodDeclarator();
			method_declarator->identifier_token = loc;
			method_declarator->left_parenthesis_token = loc;
			method_declarator->right_parenthesis_token = loc;

			if (member->ACC_STATIC())
			{
				method_declarator->AllocateFormalParameters(1);
				base->symbol = base_type;
			}
			else
			{
				method_declarator->AllocateFormalParameters(2);

				NameSymbol* instance_name = control.MakeParameter(1);

				VariableSymbol* instance =
					block_symbol->InsertVariableSymbol(instance_name);
				instance->SetACC_SYNTHETIC();
				instance->SetType(base_type == super ? this : base_type);
				instance->SetOwner(write_method);
				instance->SetLocalVariableIndex(block_symbol ->
					max_variable_index++);
				instance->MarkComplete();
				write_method->AddFormalParameter(instance);
				base->symbol = (base_type == super
					? (Symbol*)super : (Symbol*)instance);
			}

			VariableSymbol* symbol =
				block_symbol->InsertVariableSymbol(member->Identity());
			symbol->SetACC_SYNTHETIC();
			symbol->SetType(member->Type());
			symbol->SetOwner(write_method);
			symbol->SetLocalVariableIndex(block_symbol->max_variable_index++);
			symbol->MarkComplete();

			if (control.IsDoubleWordType(member->Type()))
				block_symbol->max_variable_index++;
			write_method->AddFormalParameter(symbol);
			// A write access method has no throws clause !
			write_method->SetSignature(control);

			AstName* simple_name = ast_pool->GenName(loc);
			simple_name->symbol = symbol;

			AstAssignmentExpression* assignment_expression = ast_pool ->
				GenAssignmentExpression(AssignmentExpressionEnum::SIMPLE_EQUAL,
					loc);
			assignment_expression->left_hand_side = left_hand_side;
			assignment_expression->expression = simple_name;

			AstExpressionStatement* expression_statement =
				ast_pool->GenExpressionStatement();
			expression_statement->expression = assignment_expression;
			expression_statement->semicolon_token_opt = loc;
			expression_statement->is_reachable = true;
			expression_statement->can_complete_normally = true;

			ReturnStatement* return_statement =
				ast_pool->GenReturnStatement();
			return_statement->return_token = loc;
			return_statement->semicolon_token = loc;
			return_statement->is_reachable = true;

			AstMethodBody* block = ast_pool->GenMethodBody();
			block->left_brace_token = loc;
			block->right_brace_token = loc;
			block->block_symbol = new BlockSymbol(0);
			block->is_reachable = true;
			block->AllocateStatements(2);
			block->AddStatement(expression_statement);
			block->AddStatement(return_statement);

			MethodDeclaration* method_declaration =
				ast_pool->GenMethodDeclaration();
			method_declaration->method_symbol = write_method;
			method_declaration->method_declarator = method_declarator;
			method_declaration->method_body_opt = block;
			block->owner_ast = method_declaration;

			write_method->declaration = method_declaration;
			write_method->accessed_member = member;
			MapSymbolToWriteMethod(member, base_type, write_method);
			AddPrivateAccessMethod(write_method);

			delete[] name;
		}
		return write_method;
	}


	MethodSymbol* TypeSymbol::GetWriteAccessFromReadAccess(MethodSymbol* read_method)
	{
		assert(read_method && read_method->ACC_SYNTHETIC() &&
			read_method->containing_type == this);
		VariableSymbol* variable =
			DYNAMIC_CAST<VariableSymbol*>(read_method->accessed_member);
		MethodDeclaration* method_declaration =
			DYNAMIC_CAST<MethodDeclaration*>(read_method->declaration);
		AstMethodBody* block = method_declaration->method_body_opt;
		ReturnStatement* return_statement =
			DYNAMIC_CAST<ReturnStatement*>(block->GetStatement(0));
		FieldReference* field_access =
			DYNAMIC_CAST<FieldReference*>(return_statement->expression_opt);
		return GetWriteAccessMethod(variable, field_access->base->Type());
	}


	//
	// Create a new placeholder type in order to create a unique parameter in
	// accessor constructors. The first anonymous type created in an outer class
	// can be used as the placeholder.
	//
	TypeSymbol* TypeSymbol::GetPlaceholderType()
	{
		assert(outermost_type == this);
		if (!placeholder_type)
		{
			//
			// Use the location of the class name for all elements of the
			// placeholder.
			//
			Semantic* sem = semantic_environment->sem;
			sem->state_stack.Push(semantic_environment);
			auto loc = declaration->identifier_token;
			LookupEnvironment& control = sem->lookup_env;
			AstStoragePool* ast_pool = sem->compilation_unit->ast_pool;

			AstClassBody* class_body = ast_pool->GenClassBody();
			class_body->left_brace_token = loc;
			class_body->right_brace_token = loc;
		
			AstName* ast_type = ast_pool->GenName(loc);

			AstClassCreationExpression* class_creation =
				ast_pool->GenClassCreationExpression();
			class_creation->new_token = loc;
			class_creation->class_type = ast_pool->GenTypeName(ast_type);
			class_creation->arguments = ast_pool->GenArguments(loc, loc);

			auto anonymous_type_opt = reinterpret_cast<AstAnonymousTypeDeclaration*>(
					ast_pool->GenAnonymousTypeDeclaration());
			anonymous_type_opt->create_expr = class_creation;
			anonymous_type_opt->class_body = class_body;
			class_creation->anonymous_type_opt = anonymous_type_opt;
			class_body->identifier_token = class_creation->class_type->IdentifierToken();


			sem->GetAnonymousType(class_creation, control.Object());
			sem->state_stack.Pop();
			assert(placeholder_type);
			placeholder_type->SetACC_SYNTHETIC();
		}
		return placeholder_type;
	}

	void TypeSymbol::SetClassName()
	{
		size_t length;

		if (file_symbol->OutputDirectory()->DirectoryName())
		{
			DirectorySymbol* output_directory = file_symbol->OutputDirectory();
			int directory_length = output_directory->DirectoryNameLength();
			auto directory_name = output_directory->DirectoryName();
			length = directory_length + ExternalUtf8NameLength() +
				FileSymbol::class_suffix_length + 1; // +1 for /
		
			class_name.assign(directory_name, directory_name+  directory_length);

			if (directory_name[directory_length - 1] != U_SLASH)
				class_name.append( StringConstant::U8S_SL);
		}
		else
		{
			const char* file_name =
				file_symbol->lex_stream->FileName();
			int n;
			for (n = file_symbol->lex_stream ->
				FileNameLength() - 1;
				n >= 0; n--)
			{
				if (file_name[n] == U_SLASH)
					break;
			}
			n++;

			length =
				n + ExternalUtf8NameLength() + FileSymbol::class_suffix_length;
			
			class_name.assign(file_name, file_name + n);
	
		}
		class_name.append( ExternalUtf8Name() );
		class_name.append(FileSymbol::class_suffix);
		
		assert(class_name.size() <= length);
	}


	void TypeSymbol::ProcessNestedTypeSignatures(Semantic* sem, Token* tok)
	{
		for (unsigned i = 0; i < NumNestedTypeSignatures(); i++)
		{
			NameSymbol* name_symbol = sem ->
				lookup_env.nameEnv->ConvertUtf8ToUnicode(NestedTypeSignature(i),
					strlen(NestedTypeSignature(i)));
			delete[] NestedTypeSignature(i);
			sem->ProcessNestedType(this, name_symbol, tok);
		}

		delete nested_type_signatures;
		nested_type_signatures = NULL;
	}

	MethodSymbol* TypeSymbol::ReadMethod(Symbol* symbol,
		TypeSymbol* base_type)
	{
		if (read_methods)
		{
			Map<TypeSymbol, MethodSymbol>* map = read_methods->Image(symbol);
			if (map)
				return map->Image(base_type);
		}
		return NULL;
	}

} // Close namespace Jikes block





