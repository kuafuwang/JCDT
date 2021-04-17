#pragma once

#ifndef AstArray_INCLUDED
#define AstArray_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "AstStoragePool.h"

namespace Jikes { // Open namespace Jikes block


	//
	// This AstArray template class can be used to construct a bounds-checking
	// array of Ast objects. The size of the array must be known up front, as
	// it is allocated contiguously from a AstStoragePool (preferably the pool that
	// also owns the Ast object which contains this array).
	//
	template <typename T>
	class AstArray
	{
		unsigned size;
		unsigned top;
		T* array;
		AstStoragePool* pool;
	public:
		//
		// Return length of the Ast array.
		//
		unsigned Length() { return top; }

		//
		// Return a reference to the ith element of the Ast array.
		//
		T& operator[](unsigned i)
		{
			assert(i < top);
			return array[i];
		}

		//
		// Add an element to the Ast array and return a reference to
		// that new element.
		//
		/*T& Next()
		{
			assert(top < size);
			return array[top++];

		}*/
		T& Next()
		{
			if (top >= size)
			{
				if(size)
				{
					unsigned old_size = size;

					size = old_size * 2;

					T* oldArry = array;

					array = new (pool->Alloc(size * sizeof(T))) T[size];
					array = (T*)memcpy(array, oldArry,
						old_size * sizeof(T));
				}
				else
				{
					size = 1;
					array = new (pool->Alloc(size * sizeof(T))) T[size];
				}


			}
			return array[top++];
		}
		//
		// Constructor of an Ast array.
		//
		AstArray(AstStoragePool*, unsigned);

		//
		// Ast arrays should not be destroyed. Rather, delete the AstStoragePool
		// that was passed to the constructor.
		//
		~AstArray() { assert(false && "Use the associated AstStoragePool"); }

		//
		// Ast arrays must be created via a AstStoragePool, and there are no
		// AstArray[].
		//
		  void* operator new(size_t, AstStoragePool*);
		  void operator delete(void*, AstStoragePool*){ assert(false); };
	private:
		void* operator new[](size_t, void* p) { assert(false); return p; }
		void  operator delete[](void* p, size_t, AstStoragePool*) { assert(false); return; }
	};


	template <typename T>
	  void* AstArray<T>::operator new(size_t size, AstStoragePool* pool)
	{
		return pool->Alloc(size);
	}
	//
	// Constructor of an Ast array.
	//
	template <typename T>
	AstArray<T>::AstArray(AstStoragePool* _pool, unsigned estimate)
		: size(estimate), pool(_pool)
	{
		//
		// This bit of code is a compile-time assertion that only Ast* are stuck
		// in an AstArray.
		//
#if defined JIKES_DEBUG && defined HAVE_STATIC_CAST
		assert(true || static_cast<Ast*> (T()));
#endif // JIKES_DEBUG

		if (estimate)
			array = new (pool->Alloc(size * sizeof(T))) T[size];
	}


} // Close namespace Jikes block


#endif // AstArray_INCLUDED

