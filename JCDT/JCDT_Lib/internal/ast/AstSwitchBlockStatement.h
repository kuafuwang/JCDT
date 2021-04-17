#ifndef AstSwitchBlockStatement_INCLUDED
#define AstSwitchBlockStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstBlock.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // SwitchBlockStatement --> <SWITCH_BLOCK, SwitchLabels, BlockStatements>
				  //
	class AstSwitchBlockStatement : public AstBlock
	{
		AstArray<AstSwitchLabel*>* switch_labels;

	public:
		static AstSwitchBlockStatement* SwitchBlockStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<AstSwitchBlockStatement*>
				(node->kind == SWITCH_BLOCK ? node : NULL);
		}
		  AstSwitchBlockStatement(AstStoragePool* p)
			: AstBlock(p, SWITCH_BLOCK), switch_labels(nullptr)
		{
			no_braces = true;
			SetTag(SWITCH_BLOCK_STATEMENT);
		}
		~AstSwitchBlockStatement() {}

		  AstSwitchLabel*& SwitchLabel(unsigned i)
		{
			return (*switch_labels)[i];
		}

		  unsigned NumSwitchLabels();
		  void AllocateSwitchLabels(unsigned estimate = 1);
		  void AddSwitchLabel(AstSwitchLabel*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;


		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		virtual Token* LeftToken();
		// Inherited RightToken() is adequate.
	};



	inline    void AstSwitchBlockStatement::AllocateSwitchLabels(unsigned estimate)
	{
		assert(!switch_labels);
		switch_labels = new (pool) AstArray<AstSwitchLabel*>(pool, estimate);
	}

	inline    void AstSwitchBlockStatement::AddSwitchLabel(AstSwitchLabel* case_label)
	{
		assert(switch_labels);
		switch_labels->Next() = case_label;
	}

} // Close namespace Jikes block


#endif // AstSwitchBlockStatement_INCLUDED
