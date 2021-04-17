#ifndef AstSynchronizedStatement_INCLUDED
#define AstSynchronizedStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // SynchronizedStatement --> <SYNCHRONIZED_STATEMENT, Label_opt,
				  // synchronized_token, Expression, Block>
				  //
	class SynchronizedStatement : public ReachComleteAbleStatement
	{
	public:
		static SynchronizedStatement* SynchronizedStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<SynchronizedStatement*>
				(node->kind == SYNCHRONIZED_STATEMENT ? node : NULL);
		}

		Token* synchronized_token;
		Expression* expression;
		AstBlock* block;

		 SynchronizedStatement()
			: ReachComleteAbleStatement(SYNCHRONIZED_STATEMENT),
			 synchronized_token(nullptr), expression(nullptr), block(nullptr)
		{}
		~SynchronizedStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return synchronized_token; }

		virtual Token* RightToken();
	};




} // Close namespace Jikes block


#endif // AstSynchronizedStatement_INCLUDED
