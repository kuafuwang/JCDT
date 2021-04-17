#ifndef AstFalseLiteral_INCLUDED
#define AstFalseLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // FalseLiteral --> <FALSE_LITERAL, Literal, value>
				  //
	class FalseLiteral : public Expression
	{
	public:

		static FalseLiteral* FalseLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<FalseLiteral*>
				(node->kind == FALSE_LITERAL ? node : NULL);
		}
		Token* false_literal_token;

		  FalseLiteral(Token* token)
			: Expression(FALSE_LITERAL)
			, false_literal_token(token)
		{}
		~FalseLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return false_literal_token; }
		virtual Token* RightToken() { return false_literal_token; }
	};


} // Close namespace Jikes block


#endif // AstFalseLiteral_INCLUDED
