#ifndef AstIntegerLiteral_INCLUDED
#define AstIntegerLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // Represents an int literal.
				  //
	class IntegerLiteral : public Expression
	{
	public:
		static IntegerLiteral* IntegerLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<IntegerLiteral*>
				(node->kind == INTEGER_LITERAL ? node : NULL);
		}
		Token* integer_literal_token;

		  IntegerLiteral(Token* token)
			: Expression(INTEGER_LITERAL)
			, integer_literal_token(token)
		{}
		~IntegerLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return integer_literal_token; }
		virtual Token* RightToken() { return integer_literal_token; }
	};


} // Close namespace Jikes block


#endif // AstIntegerLiteral_INCLUDED
