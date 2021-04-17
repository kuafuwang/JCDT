#ifndef AstMemberValuePair_INCLUDED
#define AstMemberValuePair_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block



				  //
				  // MemberValuePair is added by JSR 175. This covers MemberValuePair and
				  // SingleMemberAnnotation in the grammar.
				  //
	class AstMemberValuePair : public Ast
	{
	public:
		Token* identifier_token_opt;
		AstMemberValue* member_value;
		static AstMemberValuePair* MemberValuePairCast(Ast* node) 
		{
			return DYNAMIC_CAST<AstMemberValuePair*>
				(node->kind == MEMBER_VALUE_PAIR ? node : NULL);
		}
		MethodSymbol* name_symbol; // The annotation method this value maps to.

		AstMemberValuePair();

		~AstMemberValuePair() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken();

		virtual Token* RightToken();
	};

	inline AstMemberValuePair::AstMemberValuePair(): Ast(MEMBER_VALUE_PAIR), identifier_token_opt(nullptr), member_value(nullptr), name_symbol(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstMemberValuePair_INCLUDED
