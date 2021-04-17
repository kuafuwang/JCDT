#ifndef AstImportDeclaration_INCLUDED
#define AstImportDeclaration_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/ImportReference.h"

namespace Jikes { // Open namespace Jikes block

	//
	// ImportDeclaration --> <IMPORT, import_token, Name, *_token_opt, ;_token>
	//
	class AstImportDeclaration : public ImportReference
	{
	public:
		Token* import_token;
		Token* static_token_opt;
		Token* star_token_opt;
		
		static AstImportDeclaration* ImportDeclarationCast(Ast* node)
		{
			return DYNAMIC_CAST<AstImportDeclaration*>(node->kind == IMPORT ? node : NULL);
		}
		wstring getFullName() const;
		AstImportDeclaration();

		~AstImportDeclaration() {}

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);
		;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;

		virtual Token* LeftToken() { return import_token; }
		virtual Token* RightToken() { return semicolon_token; }


		bool onDemand() override;
	};

	inline AstImportDeclaration::AstImportDeclaration(): 
		ImportReference(Ast::IMPORT), import_token(nullptr), 
		static_token_opt(nullptr),  star_token_opt(nullptr)
	{}
} // Close namespace Jikes block


#endif // AstImportDeclaration_INCLUDED
