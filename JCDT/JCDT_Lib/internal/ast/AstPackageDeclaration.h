#ifndef AstPackageDeclaration_INCLUDED
#define AstPackageDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ImportReference.h"
namespace Jikes { // Open namespace Jikes block

				  //
				  // Represents the PackageDeclaration, including the annotations made possible
				  // in package-info.java by JSR 175.
				  //
	class AstPackageDeclaration : public ImportReference
	{
	public:
		static AstPackageDeclaration* PackageDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<AstPackageDeclaration*>
				(node->kind == PACKAGE ? node : NULL);
		}
		
		AstModifiers* modifiers_opt;
		Token* package_token;
	

		AstPackageDeclaration();

		~AstPackageDeclaration() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken();

		virtual Token* RightToken() { return semicolon_token; }

		bool onDemand() override;
	};






} // Close namespace Jikes block


#endif // AstPackageDeclaration_INCLUDED
