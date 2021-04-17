#ifndef AstModifierKeyword_INCLUDED
#define AstModifierKeyword_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block



				  // Represents a single modifier keyword ('public', 'protected', 'private',
				  // 'static', 'abstract', 'final', 'native', 'synchronized', 'transient',
				  // 'volatile', and 'strictfp').
				  //
	class AstModifierKeyword : public Ast
	{
	public:
		Token* modifier_token;
		static AstModifierKeyword* ModifierKeywordCast(Ast* node)
		{
			return DYNAMIC_CAST<AstModifierKeyword*>
				(node->kind == MODIFIER_KEYWORD ? node : NULL);
		}
		AstModifierKeyword(Token* token)
			: Ast(MODIFIER_KEYWORD)
			, modifier_token(token)
		{}
		~AstModifierKeyword() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return modifier_token; }
		virtual Token* RightToken() { return modifier_token; }
	};



} // Close namespace Jikes block


#endif // AstModifierKeyword_INCLUDED
