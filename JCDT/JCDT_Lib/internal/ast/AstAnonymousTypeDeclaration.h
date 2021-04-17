#ifndef Jikes_AstAstAnonymousTypeDeclaration_INCLUDED
#define Jikes_AstAstAnonymousTypeDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
namespace Jikes { // Open namespace Jikes block


	//
	// AstAnonymousTypeDeclaration --> <>
	//
	class AstAnonymousTypeDeclaration : public TypeDeclaration
	{
	public:
		AstClassCreationExpression* create_expr;
		static  AstAnonymousTypeDeclaration* AstAnonymousTypeDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<AstAnonymousTypeDeclaration*>
				(node->kind == ANONYMOUS_DECLARATION ? node : NULL);
		}

		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return ANONYMOUS_TYPE_DECLARE;
		}
		  AstAnonymousTypeDeclaration()
			: TypeDeclaration(ANONYMOUS_DECLARATION), create_expr(nullptr)
		{
			_level = ANONYMOUS_LEVEL;
		}
		~AstAnonymousTypeDeclaration() {}

#ifdef JIKES_DEBUG
		 void Print(Ostream&, LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken();
		wstring getName() override;
	};

	
} // Close namespace Jikes block


#endif // AstEmptyDeclaration_INCLUDED
