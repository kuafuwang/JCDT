#ifndef AstBrackets_INCLUDED
#define AstBrackets_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block


				  //
				  // Represents one or more pairs of '[' ']'.
				  //
	class AstBrackets : public Ast
	{
	public:
		Token* left_bracket_token;
		Token* right_bracket_token;

		unsigned dims;
		static AstBrackets* BracketsCast(Ast *node)
		{
			return DYNAMIC_CAST<AstBrackets*>(node->kind == BRACKETS ? node : NULL);
		}

		AstBrackets(Token* l, Token* r);

		~AstBrackets() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return left_bracket_token; }
		virtual Token* RightToken() { return right_bracket_token; }
	};







} // Close namespace Jikes block


#endif // AstBrackets_INCLUDED
