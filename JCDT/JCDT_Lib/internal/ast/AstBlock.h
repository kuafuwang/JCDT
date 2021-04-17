#ifndef AstBlock_INCLUDED
#define AstBlock_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "JCDT_Lib/internal/lookup/VariableSymbolArray.h"
#include "AstArray.h"
#include "AstNodeScope.h"

namespace Jikes { // Open namespace Jikes block

	//
	// Blocks represent both method blocks and compound statements. The parser
	// creates synthetic blocks around statements where blocks are optional (such
	// as if statement branches), and around loops.
	//
	class AstBlock : public ReachComleteAbleStatement, public AstNodeScope
	{
	public:
		AstStoragePool* pool;
		AstArray<ReachComleteAbleStatement*>* block_statements;
		VariableSymbolArray* defined_variables;


		enum BlockTag
		{
			NONE,
			TRY_CLAUSE_WITH_FINALLY,
			TRY_CLAUSE_WITH_CATCH,
			ABRUPT_TRY_FINALLY,
			FINALLY,
			SYNCHRONIZED,

			SWITCH,

		
			SWITCH_BLOCK_STATEMENT,

			FOREACH_STATEMENT_BLOCK,


			FOR_STATEMENT_BLOCK,

			IF_TRUE_STATEMENT_BLOCK,

			IF_FALSE_STATEMENT_BLOCK,

			DO_STATEMENT_BLOCK,

			WHILE_STATEMENT_BLOCK,

			METHOD_BODY_BLOCK
		};
		Ast*   owner_ast;
		BlockSymbol* block_symbol;
		unsigned nesting_level;

		Token* label_opt;
		Token* left_brace_token;
		Token* right_brace_token;

		bool no_braces;
		static AstBlock* BlockCast(Ast* node)
		{
			return DYNAMIC_CAST<AstBlock*>
				(node->kind == BLOCK || node->kind == METHOD_BODY || node->kind == SWITCH_BLOCK
					? node : NULL);
		}
		void Reset();
		AstBlock(AstStoragePool* p, AstKind k = BLOCK, bool reachable = false);

		~AstBlock() {}

		  BlockTag Tag() { return (BlockTag)other_tag; }
		  void SetTag(BlockTag tag) { other_tag = tag; }

		  ReachComleteAbleStatement*& GetStatement(unsigned i);

		  unsigned NumStatements();
		  void AllocateStatements(unsigned estimate = 1);
		  void AddStatement(ReachComleteAbleStatement*);
		  void AddStatement(AstBlock* block_statement);
		  VariableSymbol*& LocallyDefinedVariable(unsigned i);

		  unsigned NumLocallyDefinedVariables();
		  void AllocateLocallyDefinedVariables(unsigned estimate = 1);
		  void AddLocallyDefinedVariable(VariableSymbol*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken() { return left_brace_token; }
		virtual Token* RightToken() { return right_brace_token; }

	protected:
		void CloneBlock(AstStoragePool*, AstBlock*);
	public:
		int GetScopeKind() override{
			return BLOCK_SCOPE;
		}

		Symbol* GetSymbol() override;;
	};

	
	class SwitchBlock: public AstBlock
	{
	public:
		
		SwitchBlock(AstStoragePool* p, AstKind k = BLOCK, bool reachable = false)
			: AstBlock(p, k, reachable)
		{
			SetTag(SWITCH);
		}

	};

	inline  VariableSymbol*& AstBlock::LocallyDefinedVariable(unsigned i)
	{
		return (*defined_variables)[i];
	}

	inline    unsigned AstBlock::NumLocallyDefinedVariables()
	{
		return defined_variables ? defined_variables->Length() : 0;
	}

	inline void AstBlock::AllocateStatements(unsigned estimate)
	{
		assert(!block_statements);
		block_statements = new (pool) AstArray<ReachComleteAbleStatement*>(pool, estimate);
	}



	
	inline void AstBlock::AllocateLocallyDefinedVariables(unsigned estimate)
	{
		if (!defined_variables)
			defined_variables = pool->NewVariableSymbolArray(estimate);
	}

	inline void AstBlock::AddLocallyDefinedVariable(VariableSymbol* variable_symbol)
	{
		if (!defined_variables)
			AllocateLocallyDefinedVariables(1);
		defined_variables->Next() = variable_symbol;
	}
	inline ReachComleteAbleStatement*& AstBlock::GetStatement(unsigned i)
	{
		return (*block_statements)[i];
	}

	inline 	unsigned AstBlock::NumStatements()
	{
		return block_statements ? block_statements->Length() : 0;
	}
	inline void AstBlock::AddStatement(ReachComleteAbleStatement* statement)
	{
		assert(block_statements);
		block_statements->Next() = statement;
	}
	inline void AstBlock::AddStatement(AstBlock* block_statement)
	{
		assert(block_statements);
		block_statements->Next() = block_statement;
		if(block_statement)
			block_statement->owner_ast = this;
	}
} // Close namespace Jikes block


#endif // AstBlock_INCLUDED
