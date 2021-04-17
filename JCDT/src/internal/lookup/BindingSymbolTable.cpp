
#include <JCDT_Lib/internal/lookup/BindingSymbolTable.h>
#include <JCDT_Lib/internal/util/tuple.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lookup/BlockSymbol.h>


#include <JCDT_Lib/internal/impl/NameSymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/LabelSymbol.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/lookup/PathSymbol.h>

namespace Jikes { // Open namespace Jikes block
	
	 void BindingSymbolTable::AddTypeSymbol(TypeSymbol* symbol)
	{
		symbol->pool_index = NumTypeSymbols();
		if (!type_symbol_pool)
			type_symbol_pool = new Tuple<TypeSymbol*>(256);
		type_symbol_pool->Next() = symbol;
		Hash(symbol);
	}
	 void BindingSymbolTable::AddMethodSymbol(MethodSymbol* symbol)
	{
		symbol->pool_index = NumMethodSymbols();
		if (!method_symbol_pool)
			method_symbol_pool = new ConvertibleArray<MethodSymbol*>(256);
		method_symbol_pool->Next() = symbol;
		// not hashed, because of method overloading
	}
	 void BindingSymbolTable::AddVariableSymbol(VariableSymbol* symbol)
	{
		symbol->pool_index = NumVariableSymbols();
		if (!variable_symbol_pool)
			variable_symbol_pool = new ConvertibleArray<VariableSymbol*>(256);
		variable_symbol_pool->Next() = symbol;
		Hash(symbol);
	}

	 PackageSymbol* BindingSymbolTable::FindPackageSymbol(const NameSymbol* name_symbol)
	{
		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && PackageSymbol::PackageCast(symbol))
				return (PackageSymbol*)symbol;
		}
		return NULL;
	}


	 TypeSymbol* BindingSymbolTable::InsertAnonymousTypeSymbol(NameSymbol* name_symbol)
	{
		TypeSymbol* symbol = new TypeSymbol(name_symbol);
		AddAnonymousSymbol(symbol);
		return symbol;
	}


	 TypeSymbol* BindingSymbolTable::InsertTypeSymbol(NameSymbol* name_symbol)
	{
		assert(base);
		TypeSymbol* symbol = new TypeSymbol(name_symbol);
		AddTypeSymbol(symbol);
		return symbol;
	}


	 void BindingSymbolTable::DeleteTypeSymbol(TypeSymbol* type)
	{
		assert(base);
		unsigned k = type->name_symbol->index % hash_size;
		if (type == base[k])
			base[k] = type->next;
		else
		{
			Symbol* previous = base[k];
			for (Symbol* symbol = previous->next;
			symbol != type; previous = symbol, symbol = symbol->next)
				;
			previous->next = type->next;
		}

		unsigned last_index = NumTypeSymbols() - 1;
		if (type->pool_index != last_index)
		{
			// Move last element to position previously occupied by element being
			// deleted
			TypeSym(last_index)->pool_index = type->pool_index;
			TypeSym(type->pool_index) = TypeSym(last_index);
		}

		type_symbol_pool->Reset(last_index); // remove last slot in symbol_pool
		delete type;
	}



	 void BindingSymbolTable::DeleteAnonymousTypes()
	{
		for (unsigned i = 0; i < NumAnonymousSymbols(); i++)
		{
			TypeSymbol* symbol = AnonymousSym(i);
			symbol->UnlinkFromParents();
			delete symbol;
		}
		delete anonymous_symbol_pool;
		anonymous_symbol_pool = NULL;
	}


	 TypeSymbol* BindingSymbolTable::FindTypeSymbol(const NameSymbol* name_symbol)
	{
		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && TypeSymbol::TypeCast(symbol))
				return (TypeSymbol*)symbol;
		}
		return NULL;
	}


	 MethodSymbol* BindingSymbolTable::InsertMethodSymbol(MethodSymbol* symbol)
	{
		assert(base);
		AddMethodSymbol(symbol);
		const NameSymbol* name_symbol = symbol->Identity();
		MethodSymbol* base_method = NULL;
		Symbol* candidate;
		for (candidate = base[name_symbol->index % hash_size];
		candidate; candidate = candidate->next)
		{
			if (name_symbol == candidate->Identity() &&
				MethodSymbol::MethodCast(candidate))
			{
				base_method = (MethodSymbol*)candidate;
				break;
			}
		}
		if (base_method)
		{
			symbol->next = symbol; // mark method as overloaded
			symbol->next_method = base_method->next_method;
			base_method->next_method = symbol;
		}
		else Hash(symbol);
		return symbol;
	}



	 MethodSymbol* BindingSymbolTable::FindMethodSymbol(const NameSymbol* name_symbol)
	{
		if (!name_symbol)
			return NULL;

		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && MethodSymbol::MethodCast(symbol))
				return (MethodSymbol*)symbol;
		}
		return NULL;
	}


	 VariableSymbol* BindingSymbolTable::InsertVariableSymbol(const NameSymbol* name_symbol)
	{
		assert(base);
		VariableSymbol* symbol = new VariableSymbol(name_symbol);
		AddVariableSymbol(symbol);
		return symbol;
	}



	 VariableSymbol* BindingSymbolTable::InsertVariableSymbol(VariableSymbol* symbol)
	{
		assert(base);
		AddVariableSymbol(symbol);
		return symbol;
	}



	 VariableSymbol* BindingSymbolTable::FindVariableSymbol(const NameSymbol* name_symbol)
	{
		if(!name_symbol)
			return NULL;

		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && VariableSymbol::VariableCast(symbol))
				return (VariableSymbol*)symbol;
		}
		return NULL;
	}

	 FileSymbol* BindingSymbolTable::InsertFileSymbol(const NameSymbol* name_symbol)
	{
		assert(base);
		FileSymbol* symbol = new FileSymbol(name_symbol);
		AddOtherSymbol(symbol);
		Hash(symbol);
		return symbol;
	}

	void BindingSymbolTable::InsertFileSymbol(FileSymbol* symbol)
	{
		assert(base);
		AddOtherSymbol(symbol);
		Hash(symbol);	
	}


	FileSymbol* BindingSymbolTable::FindFileSymbol(const NameSymbol* name_symbol)
	{
		if (!name_symbol)
			return NULL;

		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && FileSymbol::FileCast(symbol))
				return (FileSymbol*)symbol;
		}
		return NULL;
	}


	  LabelSymbol* BindingSymbolTable::InsertLabelSymbol(NameSymbol* name_symbol)
	 {
		 assert(base);
		 LabelSymbol* symbol = new LabelSymbol(name_symbol);
		 AddOtherSymbol(symbol);
		 Hash(symbol);
		 return symbol;
	 }


	  LabelSymbol* BindingSymbolTable::FindLabelSymbol(const NameSymbol* name_symbol)
	 {
		 if (!name_symbol)
			 return NULL;

		 assert(base);
		 for (Symbol* symbol = base[name_symbol->index % hash_size];
		 symbol; symbol = symbol->next)
		 {
			 if (name_symbol == symbol->Identity() && LabelSymbol::LabelCast(symbol))
				 return (LabelSymbol*)symbol;
		 }
		 return NULL;
	 }

	  BlockSymbol* BindingSymbolTable::InsertBlockSymbol(unsigned hash_size)
	 {
		 BlockSymbol* symbol = new BlockSymbol(hash_size);
		 AddOtherSymbol(symbol);
		 return symbol;
	 }



	 PackageSymbol* BindingSymbolTable::InsertPackageSymbol(NameSymbol* name_symbol,
		PackageSymbol* owner)
	{
		assert(base);
		PackageSymbol* symbol = new PackageSymbol(name_symbol, owner);
		AddOtherSymbol(symbol);
		Hash(symbol);
		return symbol;
	}

	 DirectorySymbol* BindingSymbolTable::FindDirectorySymbol(const NameSymbol* name_symbol)
	{
		if (!name_symbol)
			return NULL;

		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			if (name_symbol == symbol->Identity() && DirectorySymbol::DirectoryCast(symbol))
				return (DirectorySymbol*)symbol;
		}
		return NULL;
	}

	 PathSymbol* BindingSymbolTable::InsertPathSymbol(NameSymbol* name_symbol,
		DirectorySymbol* directory_symbol)
	{
		assert(base);

		PathSymbol* symbol = new PathSymbol(name_symbol);
		directory_symbol->owner = symbol;
		symbol->root_directory = directory_symbol;
		AddOtherSymbol(symbol);
		Hash(symbol);
		return symbol;
	}


	 PathSymbol* BindingSymbolTable::FindPathSymbol(const NameSymbol* name_symbol)
	{
		if (!name_symbol)
			return NULL;

		assert(base);
		for (Symbol* symbol = base[name_symbol->index % hash_size];
		symbol; symbol = symbol->next)
		{
			/* if (name_symbol == symbol -> Identity() && symbol -> PathCast())
			return (PathSymbol*) symbol;*/
			if (name_symbol == symbol->Identity() && PathSymbol::PathCast(symbol))
				return (PathSymbol*)symbol;
		}
		return NULL;
	}


	 DirectorySymbol* BindingSymbolTable::InsertDirectorySymbol(const NameSymbol* name_symbol,
		Symbol* owner,
		bool source_path)
	{
		assert(base);
		DirectorySymbol* symbol = new DirectorySymbol(name_symbol, owner,
			source_path);
		AddOtherSymbol(symbol);
		Hash(symbol);
		return symbol;
	}

	MethodSymbol* BindingSymbolTable::FindOverloadMethod(MethodSymbol* base_method,
		AstMethodDeclarator* method_declarator)
	{
		for (MethodSymbol* method = base_method; method;
		method = method->next_method)
		{
			assert(method->IsTyped());

			if (method->NumFormalParameters() ==
				method_declarator->NumFormalParameters())
			{
				int i;
				for (i = method->NumFormalParameters() - 1; i >= 0; i--)
				{
					AstFormalParameter* parameter =
						method_declarator->FormalParameter(i);
					if (method->FormalParameter(i)->Type() !=
						parameter->formal_declarator->symbol->Type())
					{
						break;
					}
				}
				if (i < 0)
					return method;
			}
		}
		return NULL;
	}

	unsigned SymbolTable::primes[] = { DEFAULT_HASH_SIZE, 101, 401, MAX_HASH_SIZE };

	void BindingSymbolTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (Symbol**)memset(new Symbol*[hash_size], 0,
			hash_size * sizeof(Symbol*));

		unsigned k;
		for (k = 0; k < NumTypeSymbols(); k++)
		{
			TypeSymbol* symbol = TypeSym(k);
			int i = symbol->name_symbol->index % hash_size;
			symbol->next = base[i];
			base[i] = symbol;
		}

		for (k = 0; k < NumMethodSymbols(); k++)
		{
			MethodSymbol* symbol = MethodSym(k);
			if (symbol->next != symbol) // not an overload
			{
				int i = symbol->name_symbol->index % hash_size;
				symbol->next = base[i];
				base[i] = symbol;
			}
		}

		for (k = 0; k < NumVariableSymbols(); k++)
		{
			VariableSymbol* symbol = VariableSym(k);
			int i = symbol->name_symbol->index % hash_size;
			symbol->next = base[i];
			base[i] = symbol;
		}

		for (k = 0; k < NumOtherSymbols(); k++)
		{
			Symbol* symbol = OtherSym(k);

			if (!BlockSymbol::BlockCast( symbol ))
			{
				int i = symbol->Identity()->index % hash_size;
				symbol->next = base[i];
				base[i] = symbol;
			}
		}
	}


	BindingSymbolTable::BindingSymbolTable(unsigned hash_size_)
		: SymbolTable(hash_size_),
		type_symbol_pool(NULL)
		, anonymous_symbol_pool(NULL)
		, method_symbol_pool(NULL)
		, variable_symbol_pool(NULL)
		, other_symbol_pool(NULL)
		  
	{
		
	}

	BindingSymbolTable::~BindingSymbolTable()
	{
		unsigned i;
		for (i = 0; i < NumAnonymousSymbols(); i++)
			delete AnonymousSym(i);
		delete anonymous_symbol_pool;
		for (i = 0; i < NumTypeSymbols(); i++)
			delete TypeSym(i);
		delete type_symbol_pool;
		for (i = 0; i < NumMethodSymbols(); i++)
			delete MethodSym(i);
		delete method_symbol_pool;
		for (i = 0; i < NumVariableSymbols(); i++)
			delete VariableSym(i);
		delete variable_symbol_pool;
		for (i = 0; i < NumOtherSymbols(); i++)
			delete OtherSym(i);
		delete other_symbol_pool;
		
	}

	

} // Close namespace Jikes block




