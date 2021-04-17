#ifndef AstTryStatement_INCLUDED
#define AstTryStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block

	// TryStatement --> <TRY, Label_opt, try-token, Block CatchClauses,
	// FinallyClause_opt>
				  
	class TryStatement : public ReachComleteAbleStatement
	{
		AstStoragePool* pool;
		AstArray<AstCatchClause*>* catch_clauses;

	public:
		static TryStatement* TryStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<TryStatement*>(node->kind == TRY_KIND ? node : NULL);
		}
		Token* try_token;
		AstBlock* block;
		AstFinallyClause* finally_clause_opt;
		bool processing_try_block;

		TryStatement(AstStoragePool* p);

		~TryStatement() {}

		  AstCatchClause*& CatchClause(unsigned i)
		{
			return (*catch_clauses)[i];
		}
		  unsigned NumCatchClauses()
		{
			return catch_clauses ? catch_clauses->Length() : 0;
		}
		  void AllocateCatchClauses(unsigned estimate = 1);
		  void AddCatchClause(AstCatchClause*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return try_token; }

		virtual Token* RightToken();
	};


	inline    void TryStatement::AllocateCatchClauses(unsigned estimate)
	{
		assert(!catch_clauses);
		catch_clauses = new (pool) AstArray<AstCatchClause*>(pool, estimate);
	}

	inline    void TryStatement::AddCatchClause(AstCatchClause* catch_clause)
	{
		assert(catch_clauses);
		catch_clauses->Next() = catch_clause;
	}




} // Close namespace Jikes block


#endif // AstTryStatement_INCLUDED
