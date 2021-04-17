#ifndef AstEmptyDeclaration_INCLUDED
#define AstEmptyDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
namespace Jikes { // Open namespace Jikes block


	//
	// EmptyDeclaration --> <EMPTY_DECLARATION, ;_token>
	//
	class EmptyTypeDeclaration : public TypeDeclaration
	{
	public:
		static  EmptyTypeDeclaration* EmptyDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<EmptyTypeDeclaration*>
				(node->kind == EMPTY_DECLARATION ? node : NULL);
		}
		static  EmptyTypeDeclaration* EmptyTypeDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<EmptyTypeDeclaration*>
				(node->kind == EMPTY_DECLARATION ? node : NULL);
		}

		Token* semicolon_token;
		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return EMPTY_TYPE_DECLARE;
		}
		  EmptyTypeDeclaration(Token* token)
			: TypeDeclaration(EMPTY_DECLARATION)
			, semicolon_token(token)
		{}
		~EmptyTypeDeclaration() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return semicolon_token; }
		virtual Token* RightToken() { return semicolon_token; }
	};


} // Close namespace Jikes block


#endif // AstEmptyDeclaration_INCLUDED
