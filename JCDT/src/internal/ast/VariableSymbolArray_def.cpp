#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#ifndef VariableSymbolArray_def_INCLUDED
#define VariableSymbolArray_def_INCLUDED
#include "JCDT_Lib/internal/ast/VariableSymbolArray.h"

namespace Jikes { // Open namespace Jikes block


				  //
				  // Allocate another block of storage for the VariableSymbol array.
				  //
	void VariableSymbolArray::AllocateMoreSpace()
	{
		//
		//
		// The variable size always indicates the maximum number of
		// elements that has been allocated for the array.
		// Initially, it is set to 0 to indicate that the array is empty.
		// The pool of available elements is divided into segments of size
		// 2**log_blksize each. Each segment is pointed to by a slot in
		// the array base.
		//
		// By dividing size by the size of the segment we obtain the
		// index for the next segment in base. If base is full, it is
		// reallocated.
		//
		//
		size_t k = size >> log_blksize; /* which segment? */

										//
										// If the base is overflowed, reallocate it and initialize the new
										// elements to NULL.
										//
		if (k == base_size)
		{
			int old_base_size = base_size;
			T** old_base = base;

			base_size += base_increment;

			// There must be enough room to allocate base
			assert(base_size <= pool->Blksize());

			base = (T**)pool->Alloc(sizeof(T*) * base_size);

			if (old_base)
			{
				memcpy(base, old_base, old_base_size * sizeof(T*));
			}
			memset(&base[old_base_size], 0,
				(base_size - old_base_size) * sizeof(T*));
		}

		//
		// We allocate a new segment and place its adjusted address in
		// base[k]. The adjustment allows us to index the segment directly,
		// instead of having to perform a subtraction for each reference.
		// See operator[] below. There must be enough room to allocate block.
		//
		assert(Blksize() <= pool->Blksize());

		base[k] = (T*)pool->Alloc(sizeof(T) * Blksize());
		base[k] -= size;

		//
		// Finally, we update size.
		//
		size += Blksize();
	}


	VariableSymbolArray::VariableSymbolArray(AstStoragePool* p,
		unsigned estimate = 0)
		: pool(p)
	{
		// There must be enough space in the storage pool to move !!!
		assert(pool->Blksize() >= 256);

		if (estimate == 0)
			log_blksize = 6; // take a guess
		else
		{
			for (log_blksize = 1;
			((1U << log_blksize) < estimate) && (log_blksize < 31);
				log_blksize++)
				;
		}

		//
		// Increment a base_increment size that is big enough not to have to
		// be reallocated. Find a block size that is smaller that the block
		// size of the pool.
		//
		base_increment = (Blksize() > pool->Blksize()
			? Blksize() / pool->Blksize() : 1) * 2;
		while (Blksize() >= pool->Blksize())
			log_blksize--;

		base_size = 0;
		size = 0;
		top = 0;
		base = NULL;
	}


} // Close namespace Jikes block


#endif // VariableSymbolArray_def_INCLUDED
