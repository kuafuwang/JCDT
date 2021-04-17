#ifndef AstForStatement_INCLUDED
#define AstForStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "AstArray.h"
namespace Jikes { // Open namespace Jikes block
	// Represents the traditional for statement. The parser has already enclosed
	// the overall for statement in its own block, as well as the enclosed
	// statement.
	class ForStatement : public ReachComleteAbleStatement
	{
		AstStoragePool* pool;
		AstArray<ReachComleteAbleStatement*>* for_init_statements;
		AstArray<AstExpressionStatement*>* for_update_statements;

	public:
		static ForStatement* ForStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<ForStatement*>(node->kind == FOR ? node : NULL);
		}
		Token* for_token;
		Expression* end_expression_opt;
		AstBlock* statement;

		ForStatement(AstStoragePool* p);

		~ForStatement() {}

		  ReachComleteAbleStatement*& ForInitStatement(unsigned i);

		  unsigned NumForInitStatements();
		  void AllocateForInitStatements(unsigned estimate = 1);
		  void AddForInitStatement(ReachComleteAbleStatement*);

		  AstExpressionStatement*& ForUpdateStatement(unsigned i);

		  unsigned NumForUpdateStatements();
		  void AllocateForUpdateStatements(unsigned estimate = 1);
		  void AddForUpdateStatement(AstExpressionStatement*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;


		virtual Token* LeftToken() { return for_token; }

		virtual Token* RightToken();
	};

	inline void ForStatement::AllocateForInitStatements(unsigned estimate)
	{
		assert(!for_init_statements);
		for_init_statements = new (pool) AstArray<ReachComleteAbleStatement*>(pool, estimate);
	}

	inline void ForStatement::AddForInitStatement(ReachComleteAbleStatement* statement)
	{
		assert(for_init_statements);
		for_init_statements->Next() = statement;
	}

	inline void ForStatement::AllocateForUpdateStatements(unsigned estimate)
	{
		assert(!for_update_statements);
		for_update_statements =
			new (pool) AstArray<AstExpressionStatement*>(pool, estimate);
	}

	inline void ForStatement::AddForUpdateStatement(AstExpressionStatement* statement)
	{
		assert(for_update_statements);
		for_update_statements->Next() = statement;
	}



} // Close namespace Jikes block


#endif // AstForStatement_INCLUDED
