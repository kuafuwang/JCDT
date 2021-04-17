
#ifndef SemanticEnvironment_INCLUDED
#define SemanticEnvironment_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/ast/ast.h"

#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>


#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/util/set.h"
#include "SemanticStack.h"
#include "SymbolTableStack.h"
#include "BlockStack.h"
#include "DefiniteBlockStack.h"
#include "DefiniteFinalAssignmentStack.h"


namespace Jikes { // Open namespace Jikes block


	//
	// This class stores the semantic environment associated with the current type
	// being processed. A stack of these objects is maintained, so that when an
	// inner or anonymous class is encountered, a new environment is used.
	//
	// This class can also be cloned. A local class must update constructors to
	// add val$name shadows of final local variables, but may encounter the use of
	// local variables after the constructor was encountered. So, the clone serves
	// as a snapshot of the state in effect at the time the constructor is first
	// called, to reuse that state when the constructor call is later updated.
	//
	class SemanticEnvironment
	{
	public:

		// The semantic processor, should be identical for all environments on the
		// state stack.
		Semantic* sem;
		SemanticEnvironment* previous; // Environment of the enclosing class, if any

									   // The current class member being compiled.
		MethodSymbol* this_method;
		VariableSymbol* this_variable;
		ReachComleteAbleStatement* explicit_constructor;

		// The arguments that need updating if local shadow variables are found.
		AstArguments* args;

		// Stacks used within the current method.
		SymbolTableStack symbol_table;
		SemanticStack<SymbolSet*> try_exception_table_stack;
		SemanticStack<TryStatement*> try_statement_stack;
		SemanticStack<AstBlock*> breakable_statement_stack;
		SemanticStack<AstBlock*> continuable_statement_stack;
		SemanticStack<unsigned> abrupt_finally_stack;
		BlockStack block_stack;

		//
		// This set of fields is used in definite assignment analysis; they should
		// not need to be cloned.
		//
		DefinitePair* definitely_assigned_variables;
		DefinitePair* universe;
		BitSet* blank_finals;
		BitSet* reachable_assignments;
		DefiniteBlockStack* definite_block_stack;
		DefiniteFinalAssignmentStack* definite_final_assignment_stack;
		Tuple<VariableSymbol*>* final_fields;
		bool processing_simple_assignment;

		SemanticEnvironment(Semantic* sem_, TypeSymbol* type_,
			SemanticEnvironment* previous_ = NULL)
			: sem(sem_),
			previous(previous_),
			this_method(NULL),
			this_variable(NULL),
			explicit_constructor(NULL),
			args(NULL),
			definitely_assigned_variables(NULL),
			universe(NULL),
			blank_finals(NULL),
			reachable_assignments(NULL),
			definite_block_stack(NULL),
			definite_final_assignment_stack(NULL),
			final_fields(NULL),
			processing_simple_assignment(false),
			type(type_),
			next(NULL)
		{}


		~SemanticEnvironment()
		{
			delete next; // if there was any clone, get rid of it
		}

		//
		// Clone the immediate environment of "this" Semantic environment. The
		// immediate environment consists primarily of the stack of symbol tables
		// that are necessary for looking up local variables in the immediate
		// environment.
		//
		SemanticEnvironment* GetEnvironment(AstArguments* args)
		{
			SemanticEnvironment* clone = new SemanticEnvironment(sem, type);
			clone->this_method = this_method;
			clone->this_variable = this_variable;
			clone->explicit_constructor = explicit_constructor;
			clone->args = args;
			for (int i = 0; i < symbol_table.Size(); i++)
				clone->symbol_table.Push(symbol_table[i]);
			clone->next = next;
			next = clone;

			return clone;
		}

		TypeSymbol* Type() { return type; }

		//
		// Are we in a region with no current instance? This applies to all
		// static initializers, static methods, and explicit constructor
		// invocations, including initializers in interfaces.
		//
		inline bool StaticRegion()
		{
			return ((this_variable && this_variable->ACC_STATIC()) ||
				(this_method && this_method->ACC_STATIC()) ||
				explicit_constructor);
		}


	private:

		TypeSymbol* type;
		SemanticEnvironment* next; // use to link an environment to its clones.
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

