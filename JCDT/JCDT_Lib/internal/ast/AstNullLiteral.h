#ifndef AstNullLiteral_INCLUDED
#define AstNullLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // NullLiteral --> <NULL_EXPRESSION, null_token>
				  //
	class NullLiteral : public Expression
	{
	public:
		static NullLiteral* NullLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<NullLiteral*>(node->ANNOTATION == NULL_LITERAL ? node : NULL);
		}
		Token* null_token;

		  NullLiteral(Token* token)
			: Expression(NULL_LITERAL)
			, null_token(token)
		{}
		~NullLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return null_token; }
		virtual Token* RightToken() { return null_token; }
	};



} // Close namespace Jikes block


#endif // AstNullLiteral_INCLUDED
