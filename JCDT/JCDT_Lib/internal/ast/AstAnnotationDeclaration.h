#ifndef AstAnnotationDeclaration_INCLUDED
#define AstAnnotationDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
namespace Jikes { // Open namespace Jikes block


	//
	// Represents an annotation type, added by JSR 175.
	//
	class AnnotationTypeDeclaration : public TypeDeclaration
	{
	public:
		static AnnotationTypeDeclaration* AnnotationDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<AnnotationTypeDeclaration*>
				(node->kind == ANNOTATION_TYPE ? node : NULL);
		}

		Token* interface_token;
		virtual TypeDeclarationKind getTypeDeclarationKind()
		{
			return ANNOTATION_TYPE_DECL;
		}

		AnnotationTypeDeclaration(Token* t);

		~AnnotationTypeDeclaration() {}

#ifdef JIKES_DEBUG
		 void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		  void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken();

		virtual Token* RightToken();
		
	};



} // Close namespace Jikes block


#endif // AstAnnotationDeclaration_INCLUDED
