#ifndef AstCharacterLiteral_INCLUDED
#define AstCharacterLiteral_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
namespace Jikes { // Open namespace Jikes block

	//
	// CharacterLiteral --> <CHARACTER_LITERAL, literal_token, value>
	//
	class CharLiteral : public Expression
	{
	public:
		static CharLiteral* CharacterLiteralCast(Ast* node)
		{
			return DYNAMIC_CAST<CharLiteral*>
				(node->kind == CHARACTER_LITERAL ? node : NULL);
		}
		Token* character_literal_token;

		CharLiteral(Token* token);

		~CharLiteral() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return character_literal_token; }
		virtual Token* RightToken() { return character_literal_token; }
	};



} // Close namespace Jikes block


#endif // AstCharacterLiteral_INCLUDED
