
#ifndef DefiniteBlockStack_INCLUDED
#define DefiniteBlockStack_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/ast/ast.h"

#include "JCDT_Lib/internal/util/set.h"

namespace Jikes { // Open namespace Jikes block



	//
	// A stack of blocks, and related data for definite assignment analysis.
	//
	class DefiniteBlockStack
	{
	public:

		void Push(AstBlock* block_)
		{
			break_pairs[top_index].SetUniverse();
			continue_pairs[top_index].SetUniverse();

			block[top_index++] = block_;
		}

		void Pop()
		{
			if (top_index > 0)
				top_index--;
			else assert(false);
		}

		int Size() { return top_index; }
		AstBlock* GetBlock(int i) { return block[i]; }
		AstBlock* TopBlock()
		{
			assert(top_index > 0);
			return block[top_index - 1];
		}

		DefinitePair& BreakPair(int i)
		{
			return break_pairs[i];
		}
		DefinitePair& ContinuePair(int i)
		{
			return continue_pairs[i];
		}

		DefinitePair& TopBreakPair()
		{
			assert(top_index > 0);
			return break_pairs[top_index - 1];
		}
		DefinitePair& TopContinuePair()
		{
			assert(top_index > 0);
			return continue_pairs[top_index - 1];
		}

		DefinitePair& ReturnPair()
		{
			return return_pair;
		}

		DefiniteBlockStack(int stack_size_, int set_size)
			: stack_size(stack_size_),
			top_index(0),
			return_pair(set_size, BitSet::UNIVERSE)
		{
			block = new AstBlock*[stack_size];
			break_pairs = new DefinitePair[stack_size];
			continue_pairs = new DefinitePair[stack_size];

			for (int i = 0; i < stack_size; i++)
			{
				break_pairs[i].Resize(set_size);
				continue_pairs[i].Resize(set_size);
			}
		}

		~DefiniteBlockStack()
		{
			delete[] block;
			delete[] break_pairs;
			delete[] continue_pairs;
		}

	private:
		int stack_size,
			top_index;
		AstBlock** block;

		DefinitePair* break_pairs;
		DefinitePair* continue_pairs;
		DefinitePair return_pair;
	};



	

} // Close namespace Jikes block


#endif // _INCLUDED

