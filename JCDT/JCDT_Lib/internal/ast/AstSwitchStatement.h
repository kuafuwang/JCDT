#ifndef AstSwitchStatement_INCLUDED
#define AstSwitchStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


	//
	// SwitchStatement --> <SWITCH, Label_opt, switch_token, Expression, {_token,
	// SwitchBlockStatements, SwitchLabels_opt, }_token>
	//
	class SwitchStatement : public ReachComleteAbleStatement
	{
		AstStoragePool* pool;
		//
		// The sorted list of case label values. Index 0 is reserved for the
		// default case. Index 1 - size are for the case labels, and get sorted.
		//
		CaseElement** cases;
		unsigned num_cases;
#ifdef JIKES_DEBUG
		unsigned max_cases; // bounds check only when debugging
#endif // JIKES_DEBUG

	public:

		static SwitchStatement* SwitchStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<SwitchStatement*>(node->kind == SWITCH ? node : NULL);
		}
		Token* switch_token;
		Expression* expression;
		AstBlock* switch_block;

		SwitchStatement(AstStoragePool* p);

		~SwitchStatement() {}

		  CaseElement*& Case(unsigned i)
		{
			assert(i < num_cases);
			return cases[i + 1];
		}
		  CaseElement*& DefaultCase() { return cases[0]; }
		  unsigned NumCases() { return num_cases; }
		  void AllocateCases(unsigned estimate = 1);
		  void AddCase(CaseElement*);

		  AstSwitchBlockStatement* GetBlock(unsigned i);

		  unsigned NumBlocks();

		void SortCases();
		CaseElement* CaseForValue(i4 value);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return switch_token; }

		virtual Token* RightToken();
	};



	

inline  	  void SwitchStatement::AddCase(CaseElement* case_element)
	{
		assert(cases);
		cases[++num_cases] = case_element;
#ifdef JIKES_DEBUG
		assert(num_cases < max_cases);
#endif // JIKES_DEBUG
	}



} // Close namespace Jikes block


#endif // AstSwitchStatement_INCLUDED
