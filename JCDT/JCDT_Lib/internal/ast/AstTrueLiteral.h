#ifndef AstTrueLiteral_INCLUDED
#define AstTrueLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block
		  
	// TrueLiteral --> <TRUE_LITERAL, Literal, value>			  
	class TrueLiteral : public Expression
	{
	public:
		static TrueLiteral* TrueLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<TrueLiteral*>(node->kind == TRUE_LITERAL ? node : NULL);
		}
		Token* true_literal_token;

		  TrueLiteral(Token* token)
			: Expression(TRUE_LITERAL)
			, true_literal_token(token)
		{}
		~TrueLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return true_literal_token; }
		virtual Token* RightToken() { return true_literal_token; }
	};



} // Close namespace Jikes block


#endif // AstTrueLiteral_INCLUDED
