#pragma once

#ifndef VariableSymbolArray_INCLUDED
#define VariableSymbolArray_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include  <assert.h>

namespace Jikes { // Open namespace Jikes block



	class VariableSymbolArray
	{
		typedef VariableSymbol* T;

		T** base;
		size_t base_size;
		unsigned top;
		unsigned size;
		AstStoragePool* pool;
		unsigned short log_blksize;
		unsigned short base_increment;

		inline size_t Blksize() { return 1 << log_blksize; }

		//
		// Allocate another block of storage for the VariableSymbol array.
		//
		void AllocateMoreSpace();

	public:
		//
		// This function is used to reset the size of a VariableSymbol array
		// without allocating or deallocting space. It may be invoked with an
		// integer argument n which indicates the new size or with no argument
		// which indicates that the size should be reset to 0.
		//
		void Reset(unsigned n = 0)
		{
			assert(n <= size);
			top = n;
		}

		//
		// Return length of the VariableSymbol array.
		//
		unsigned Length() { return top; }

		//
		// Return a reference to the ith element of the VariableSymbol array.
		//
		// Note that no check is made here to ensure that 0 <= i < top.
		// Such a check might be useful for debugging and a range exception
		// should be thrown if it yields true.
		//
		T& operator[](unsigned i) { return base[i >> log_blksize][i]; }

		//
		// Add an element to the VariableSymbol array and return the top index.
		//
		unsigned NextIndex()
		{
			unsigned i = top++;
			if (i == size)
				AllocateMoreSpace();
			return i;
		}

		//
		// Add an element to the VariableSymbol array and return a reference to
		// that new element.
		//
		T& Next()
		{
			unsigned i = NextIndex();
			return base[i >> log_blksize][i];
		}

		//
		// Constructor of a VariableSymbol array.
		//
		VariableSymbolArray(AstStoragePool*, unsigned);

		//
		// Destructor of an VariableSymbol array.
		//
		~VariableSymbolArray() { assert(false); }
};



} // Close namespace Jikes block


#endif // VariableSymbolArray_INCLUDED

