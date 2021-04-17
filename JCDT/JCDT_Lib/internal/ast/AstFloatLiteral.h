#ifndef AstFloatLiteral_INCLUDED
#define AstFloatLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block


	//
	// FloatLiteral --> <FLOAT_LITERAL, Literal, value>
	//
	class FloatLiteral : public Expression
	{
	public:
		static FloatLiteral* FloatLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<FloatLiteral*>
				(node->kind == FLOAT_LITERAL ? node : NULL);
		}
		Token* float_literal_token;

		  FloatLiteral(Token* token)
			: Expression(FLOAT_LITERAL)
			, float_literal_token(token)
		{}
		~FloatLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return float_literal_token; }
		virtual Token* RightToken() { return float_literal_token; }
	};





} // Close namespace Jikes block


#endif // AstFloatLiteral_INCLUDED
