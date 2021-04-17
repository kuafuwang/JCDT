
#ifndef jcdt_codegen_MethodStack_INCLUDED_INCLUDED
#define jcdt_codegen_MethodStack_INCLUDED_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "Label.h"
#include  <JCDT_Lib/internal/lookup/VariableSymbol.h>
namespace Jikes { // Open namespace Jikes block

	class MethodStack
	{
	public:

		void Push(AstBlock* block)
		{
			/*assert(block->nesting_level < stack_size &&
				(top_index == 0 ||
					(block->nesting_level > nesting_level[top_index - 1])));
*/
			if( block->nesting_level < stack_size &&
				(top_index == 0 ||
				(block->nesting_level > nesting_level[top_index - 1])) )
			{
				nesting_level[top_index] = block->nesting_level;
				break_labels[block->nesting_level].uses.Reset();
				continue_labels[block->nesting_level].uses.Reset();
				finally_labels[block->nesting_level].uses.Reset();
				handler_range_start[block->nesting_level].Reset();
				handler_range_end[block->nesting_level].Reset();
				blocks[block->nesting_level] = block;
				if (size)
					memset(local_variables_start_pc[block->nesting_level],
						0xFF, size * sizeof(u2));
				top_index++;
			}

			
		}

		void Pop()
		{
			if (top_index > 0)
			{
				top_index--;
#ifdef JIKES_DEBUG
				unsigned level = nesting_level[top_index];

				nesting_level[top_index] = 0;
				break_labels[level].Reset();
				continue_labels[level].Reset();
				finally_labels[level].Reset();
				handler_range_start[level].Reset();
				handler_range_end[level].Reset();
				blocks[level] = NULL;
				if (size)
					memset(local_variables_start_pc[level], 0xFF,
						size * sizeof(u2));
#endif // ! JIKES_DEBUG
			}
			else assert(false);
		}

		unsigned Size() { return top_index; }

#ifdef JIKES_DEBUG
		void AssertIndex(unsigned k)
		{
			for (unsigned i = 0; i < top_index; i++)
				if (nesting_level[i] == k)
					return;
			assert(false && "missing method stack level");
		}
#else
# define AssertIndex(x) /* nop */
#endif // ! JIKES_DEBUG

		unsigned TopNestingLevel()
		{
			assert(top_index > 0);
			return nesting_level[top_index - 1];
		}
		unsigned NestingLevel(unsigned i)
		{
			assert(i < top_index);
			return nesting_level[i];
		}

		Label& TopBreakLabel() { return break_labels[TopNestingLevel()]; }
		Label& BreakLabel(unsigned i) { AssertIndex(i); return break_labels[i]; }

		Label& TopContinueLabel() { return continue_labels[TopNestingLevel()]; }
		Label& ContinueLabel(unsigned i)
		{
			AssertIndex(i);
			return continue_labels[i];
		}

		Label& TopFinallyLabel() { return finally_labels[TopNestingLevel()]; }
		Label& FinallyLabel(unsigned i)
		{
			AssertIndex(i);
			return finally_labels[i];
		}

		Tuple<u2>& TopHandlerRangeStart()
		{
			return handler_range_start[TopNestingLevel()];
		}
		Tuple<u2>& HandlerRangeStart(unsigned i)
		{
			AssertIndex(i);
			return handler_range_start[i];
		}

		Tuple<u2>& TopHandlerRangeEnd()
		{
			return handler_range_end[TopNestingLevel()];
		}
		Tuple<u2>& HandlerRangeEnd(unsigned i)
		{
			AssertIndex(i);
			return handler_range_end[i];
		}

		AstBlock* TopBlock() { return blocks[TopNestingLevel()]; }
		AstBlock* GetBlock(unsigned i) { AssertIndex(i); return blocks[i]; }

		u2* TopLocalVariablesStartPc()
		{
			return (u2*)local_variables_start_pc[TopNestingLevel()];
		}
		u2& StartPc(VariableSymbol* variable)
		{
			assert(variable->LocalVariableIndex() >= 0 &&
				variable->LocalVariableIndex() < (int)size);
			return TopLocalVariablesStartPc()[variable->LocalVariableIndex()];
		}

		MethodStack(unsigned stack_size_, unsigned size_)
			: stack_size(stack_size_),
			size(size_),
			top_index(0)
		{
			nesting_level = new unsigned[stack_size];
			break_labels = new Label[stack_size];
			continue_labels = new Label[stack_size];
			finally_labels = new Label[stack_size];
			handler_range_start = new Tuple<u2>[stack_size];
			handler_range_end = new Tuple<u2>[stack_size];
			blocks = new AstBlock*[stack_size];

			local_variables_start_pc = new u2*[stack_size];
			for (unsigned i = 0; i < stack_size; i++)
				local_variables_start_pc[i] = new u2[size];
		}
		~MethodStack()
		{
			delete[] nesting_level;
			delete[] break_labels;
			delete[] continue_labels;
			delete[] finally_labels;
			delete[] handler_range_start;
			delete[] handler_range_end;
			delete[] blocks;

			for (unsigned i = 0; i < stack_size; i++)
				delete[] local_variables_start_pc[i];
			delete[] local_variables_start_pc;
		}

	private:
		unsigned* nesting_level;

		Label* break_labels;
		Label* continue_labels;
		Label* finally_labels;
		Tuple<u2>* handler_range_start;
		Tuple<u2>* handler_range_end;

		AstBlock** blocks; // block symbols for current block

		u2** local_variables_start_pc;
		unsigned stack_size;
		unsigned size;
		unsigned top_index;
	};


	

} // Close namespace Jikes block


#endif // _INCLUDED

