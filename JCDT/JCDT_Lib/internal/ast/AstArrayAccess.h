#ifndef AstArrayAccess_INCLUDED
#define AstArrayAccess_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/astExpression.h"
namespace Jikes { // Open namespace Jikes block



	//
	// ArrayAccess --> <ARRAY_ACCESS, Base, [_token, Expression, ]_token>
	//
	class ArrayReference : public Expression
	{
	public:
		static ArrayReference* ArrayAccessCast(Ast* node)
		{
			return DYNAMIC_CAST<ArrayReference*>(node->kind == ARRAY_ACCESS ? node : NULL);
		}
		Expression* base;
		Token* left_bracket_token;
		Expression* expression;
		Token* right_bracket_token;

		ArrayReference();

		~ArrayReference() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken()
		{
			if (base)
				return base->LeftToken();
			else
				return nullptr;
		}
		virtual Token* RightToken()
		{
			return right_bracket_token;
		}
	};



} // Close namespace Jikes block


#endif // AstArrayAccess_INCLUDED
