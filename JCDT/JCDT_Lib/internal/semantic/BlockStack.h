
#ifndef BlockStack_INCLUDED
#define BlockStack_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/util/tuple.h"


namespace Jikes { // Open namespace Jikes block


				  //
				  // A stack of blocks, and their associated data.
				  //
	class BlockStack
	{
	public:
		unsigned max_size;

		void Push(AstBlock* block_)
		{
			block.Next() = block_;
			index.Next() = 0;
			if (block.Length() > max_size)
				max_size = block.Length();
		}

		void Pop()
		{
			unsigned len = block.Length();
			if (len)
			{
				block.Reset(len - 1);
				index.Reset(len - 1);
			}
		}

		unsigned Size() { return block.Length(); }
		AstBlock* TopBlock()
		{
			return (AstBlock*)(block.Length() > 0
				? block[block.Length() - 1] : NULL);
		}

		AstBlock* operator[](const unsigned i) { return block[i]; }

		int& TopMaxEnclosedVariableIndex()
		{
			assert(index.Length());
			return index[index.Length() - 1];
		}

		BlockStack() : max_size(0) {}

	private:
		Tuple<AstBlock*> block;
		Tuple<int> index;
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

