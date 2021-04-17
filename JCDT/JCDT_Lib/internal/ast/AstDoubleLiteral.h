#ifndef AstDoubleLiteral_INCLUDED
#define AstDoubleLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block

	//
	// DoubleLiteral --> <DOUBLE_LITERAL, Literal, value>
	//
	class DoubleLiteral : public Expression
	{
	public:
		static DoubleLiteral* DoubleLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<DoubleLiteral*>
				(node->kind == DOUBLE_LITERAL ? node : NULL);
		}

		Token* double_literal_token;

		  DoubleLiteral(Token* token)
			: Expression(DOUBLE_LITERAL)
			, double_literal_token(token)
		{}
		~DoubleLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return double_literal_token; }
		virtual Token* RightToken() { return double_literal_token; }
	};




} // Close namespace Jikes block


#endif // AstDoubleLiteral_INCLUDED
