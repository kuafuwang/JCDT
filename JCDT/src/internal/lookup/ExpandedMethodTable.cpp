#include <JCDT_Lib/internal/lookup/ExpandedMethodTable.h>
#include <JCDT_Lib/internal/lookup/MethodShadowSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>

namespace Jikes { // Open namespace Jikes block


	 void ExpandedMethodTable::CompressSpace()
	{
		hash_size = symbol_pool.Length();
		hash_size = hash_size <= 0 ? 1 : hash_size > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size;
		delete[] base;
		base = (MethodShadowSymbol**)
			memset(new MethodShadowSymbol*[hash_size], 0,
				hash_size * sizeof(MethodShadowSymbol*));

		MethodShadowSymbol** array = symbol_pool.Array();
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
		{
			array[i]->CompressSpace();

			const NameSymbol* name_symbol =
				array[i]->method_symbol->name_symbol;
			MethodShadowSymbol* base_shadow =
				FindMethodShadowSymbol(name_symbol);
			if (!base_shadow)
			{
				unsigned k = name_symbol->index % hash_size;
				array[i]->next = base[k];
				base[k] = array[i];
				array[i]->next_method = NULL;
			}
			else
			{
				array[i]->next_method = base_shadow->next_method;
				base_shadow->next_method = array[i];
			}
		}
	}

	 ExpandedMethodTable::ExpandedMethodTable(unsigned hash_size_ )
		: symbol_pool(10, 4)
	{
		hash_size = hash_size_ <= 0 ? 1 : hash_size_ > MAX_HASH_SIZE
			? (unsigned)MAX_HASH_SIZE : hash_size_;
		base = (MethodShadowSymbol**)
			memset(new MethodShadowSymbol*[hash_size], 0,
				hash_size * sizeof(MethodShadowSymbol*));
	}
	 ExpandedMethodTable::~ExpandedMethodTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}

	 MethodShadowSymbol* ExpandedMethodTable::FindMethodShadowSymbol(const NameSymbol* name_symbol)
	{
		MethodShadowSymbol* p;
		for (p = base[name_symbol->index % hash_size]; p; p = p->next)
			if (p->method_symbol->name_symbol == name_symbol)
				break;
		return p;
	}

	 MethodShadowSymbol* ExpandedMethodTable::InsertMethodShadowSymbol(MethodSymbol* method_symbol)
	{
		unsigned i = method_symbol->name_symbol->index % hash_size;
		MethodShadowSymbol* p = new MethodShadowSymbol(method_symbol);
		p->next_method = NULL;
		p->next = base[i];
		base[i] = p;
		symbol_pool.Next() = p;

		return p;
	}

	 void ExpandedMethodTable::Overload(MethodShadowSymbol* base_shadow,
		MethodSymbol* overload_method)
	{
		//
		// Insert the new overload as the second list element, to preserve
		// the existing base, while making Overload(MethodSymbol*) work.
		//
		MethodShadowSymbol* shadow = new MethodShadowSymbol(overload_method);
		symbol_pool.Next() = shadow;
		shadow->next_method = base_shadow->next_method;
		base_shadow->next_method = shadow;
	}

	 MethodShadowSymbol* ExpandedMethodTable::Overload(MethodSymbol* overload_method)
	{
		MethodShadowSymbol* base_shadow =
			FindMethodShadowSymbol(overload_method->name_symbol);
		if (!base_shadow)
			return InsertMethodShadowSymbol(overload_method);
		Overload(base_shadow, overload_method);
		//
		// Return the newly created overload; this relies on the current
		// behavior of Overload(MethodSymbol*, MethodSymbol*).
		//
		return base_shadow->next_method;
	}

	MethodShadowSymbol* ExpandedMethodTable::FindOverloadMethodShadow(MethodSymbol* overload_method,
		Semantic* sem, Token* tok)
	{
		if (!overload_method->IsTyped())
			overload_method->ProcessMethodSignature(sem, tok);

		MethodShadowSymbol* method_shadow;
		for (method_shadow = FindMethodShadowSymbol(overload_method ->
			name_symbol);
		method_shadow;
			method_shadow = method_shadow->next_method)
		{
			MethodSymbol* method = method_shadow->method_symbol;

			if (overload_method == method)
				return method_shadow;

			if (!method->IsTyped())
				method->ProcessMethodSignature(sem, tok);

			if (overload_method->NumFormalParameters() ==
				method->NumFormalParameters())
			{
				int i;
				for (i = method->NumFormalParameters() - 1; i >= 0; i--)
				{
					if (method->FormalParameter(i)->Type() !=
						overload_method->FormalParameter(i)->Type())
					{
						break;
					}
				}

				if (i < 0)
					return method_shadow;
			}
		}

		return method_shadow;
	}

} // Close namespace Jikes block



