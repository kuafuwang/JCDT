#include <JCDT_Lib/internal/ast/AstBlock.h>
#include <JCDT_Lib/internal/ast/AstExpression.h>
#ifndef AstSwitchStatement_def_INCLUDED
#define AstSwitchStatement_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstSwitchStatement.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

#include <JCDT_Lib/internal/ast/CaseElement.h>
namespace Jikes { // Open namespace Jikes block



void SwitchStatement::Unparse(Ostream& os, LexStream* lex_stream)
{
    if (debug_unparse)
        os << "/*SwitchStatement:#" << id << "*/";
    // What about the label_opt??
    os << lex_stream -> NameString(switch_token->index) << '(';
    expression -> Unparse(os, lex_stream);
    os << ')';
    switch_block -> Unparse(os, lex_stream);
    if (debug_unparse)
        os << "/*:SwitchStatement#" << id << "*/";
}
	AstSwitchBlockStatement* SwitchStatement::GetBlock(unsigned i)
	{
		return (AstSwitchBlockStatement*)switch_block->GetStatement(i);
	}

	unsigned SwitchStatement::NumBlocks()
	{ return switch_block->NumStatements(); }

	//
				  // This procedure uses a quick sort algorithm to sort the cases in a switch
				  // statement. Element 0 is not sorted, because it is the default case (and
				  // may be NULL).
				  //
	void SwitchStatement::SortCases()
	{
		int lower;
		int upper;
		int lostack[32];
		int histack[32];
		int top = 0;
		int i;
		int j;
		CaseElement pivot;
		CaseElement temp;

		lostack[top] = 1;
		histack[top] = num_cases;

		while (top >= 0)
		{
			lower = lostack[top];
			upper = histack[top];
			top--;

			while (upper > lower)
			{
				//
				// The array is most-likely almost sorted. Therefore,
				// we use the middle element as the pivot element.
				//
				i = (lower + upper) >> 1;
				pivot = *cases[i];
				*cases[i] = *cases[lower];

				//
				// Split the array section indicated by LOWER and UPPER
				// using ARRAY(LOWER) as the pivot.
				//
				i = lower;
				for (j = lower + 1; j <= upper; j++)
					if (*cases[j] < pivot)
					{
						temp = *cases[++i];
						*cases[i] = *cases[j];
						*cases[j] = temp;
					}
				*cases[lower] = *cases[i];
				*cases[i] = pivot;

				top++;
				if ((i - lower) < (upper - i))
				{
					lostack[top] = i + 1;
					histack[top] = upper;
					upper = i - 1;
				}
				else
				{
					histack[top] = i - 1;
					lostack[top] = lower;
					lower = i + 1;
				}
			}
		}
	}


	//
	// Performs a binary search to locate the correct case (including the
	// default case) for a constant expression value. Returns NULL if the switch
	// is a no-op for this constant.
	//
	CaseElement* SwitchStatement::CaseForValue(i4 value)
	{
		unsigned lower = 1;
		unsigned upper = num_cases;
		while (lower <= upper)
		{
			unsigned mid = (lower + upper) >> 1;
			CaseElement* elt = cases[mid];
			if (elt->value == value)
				return elt;
			if (elt->value > value)
				upper = mid - 1;
			else
				lower = mid + 1;
		}
		return cases[0];
	}


Ast* SwitchStatement::Clone(AstStoragePool* ast_pool)
{
    SwitchStatement* clone = ast_pool -> GenSwitchStatement();
    clone -> switch_token = switch_token;
    clone -> expression = (Expression*) expression -> Clone(ast_pool);
    clone -> switch_block = (AstBlock*) switch_block -> Clone(ast_pool);
    clone -> AllocateCases(NumCases());
    if (DefaultCase())
    {
        clone -> DefaultCase() = ast_pool -> GenCaseElement(0, 0);
        *clone -> DefaultCase() = *DefaultCase();
    }
    for (unsigned i = 0; i < NumCases(); i++)
    {
        CaseElement* elt = ast_pool -> GenCaseElement(0, 0);
        *elt = *Case(i);
        clone -> AddCase(elt);
    }
    return clone;
}


	Token* SwitchStatement::RightToken()
	{
		return switch_block->right_brace_token;
	}

	void   SwitchStatement::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
	
		expression->traverse(visitor, scope);
		switch_block->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}
void SwitchStatement::Print(Ostream& os,LexStream& lex_stream)
{
    os << '#' << id << " (SwitchStatement):  "
            << lex_stream.NameString(switch_token->index)
            << " ( #" << expression -> id << " ) #" << switch_block -> id
            << endl;
    for (unsigned i = 0; i <= num_cases; i++)
    {
        os << " case index: " << i;
        if (cases[i])
            os << "  block: " << cases[i] -> block_index
                    << "  label: " << cases[i] -> case_index
                    << "  value: " << cases[i] -> value << endl;
        else os << "(none)" << endl;
    }
    expression -> Print(os, lex_stream);
    switch_block -> Print(os, lex_stream);
}

	SwitchStatement::SwitchStatement(AstStoragePool* p)
		: ReachComleteAbleStatement(SWITCH) , pool(p), cases(nullptr),
		num_cases(0), max_cases(0), switch_token(nullptr), 
		expression(nullptr), switch_block(nullptr)
	{}

	void SwitchStatement::AllocateCases(unsigned estimate)
{
	//
	// Add one to the estimate to save room for the default case in element 0.
	//
	assert(!cases);
	cases = new (pool->Alloc((estimate + 1) * sizeof(CaseElement*)))
		CaseElement*[estimate + 1];
#ifdef JIKES_DEBUG
	max_cases = estimate + 1;
#endif // JIKES_DEBUG
}

} // Close namespace Jikes block


#endif // AstSwitchStatement_def_INCLUDED
