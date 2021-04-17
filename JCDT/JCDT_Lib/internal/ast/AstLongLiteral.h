#ifndef AstLongLiteral_INCLUDED
#define AstLongLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // LongLiteral --> <LONG_LITERAL, long_literal_token, value>
				  //
	class LongLiteral : public Expression
	{
	public:
		static LongLiteral* LongLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<LongLiteral*>(node->kind == LONG_LITERAL ? node : NULL);
		}
		Token* long_literal_token;

		  LongLiteral(Token* token)
			: Expression(LONG_LITERAL)
			, long_literal_token(token)
		{}
		~LongLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return long_literal_token; }
		virtual Token* RightToken() { return long_literal_token; }
	};


} // Close namespace Jikes block


#endif // AstLongLiteral_INCLUDED
