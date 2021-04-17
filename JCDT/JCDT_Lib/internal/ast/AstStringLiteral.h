#ifndef AstStringLiteral_INCLUDED
#define AstStringLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // StringLiteral --> <STRING_LITERAL, Literal, value>
				  //
	class StringLiteral : public Expression
	{
	public:
		static StringLiteral* StringLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<StringLiteral*>
				(node->kind == STRING_LITERAL ? node : NULL);
		}
		Token* string_literal_token;

		  StringLiteral(Token* token)
			: Expression(STRING_LITERAL)
			, string_literal_token(token)
		{}
		~StringLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return string_literal_token; }
		virtual Token* RightToken() { return string_literal_token; }
	};



} // Close namespace Jikes block


#endif // AstStringLiteral_INCLUDED
