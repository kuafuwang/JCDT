#ifndef AstThrowStatement_INCLUDED
#define AstThrowStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // ThrowStatement --> <THROW, Label_opt, throw_token, Expression, ;_token>
				  //
	class ThrowStatement : public ReachComleteAbleStatement
	{
	public:
		static ThrowStatement* ThrowStatementCast(Ast* node)
		{
			return DYNAMIC_CAST<ThrowStatement*>(node->kind == THROW ? node : NULL);
		}
		Token* throw_token;
		Expression* expression;
		Token* semicolon_token;

		  ThrowStatement()
			: ReachComleteAbleStatement(THROW), throw_token(nullptr),
			  expression(nullptr), semicolon_token(nullptr)
		{}
		~ThrowStatement() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return throw_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};




} // Close namespace Jikes block


#endif // AstThrowStatement_INCLUDED
