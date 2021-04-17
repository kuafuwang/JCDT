
#include "JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block


    void AstAnonymousTypeDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
    {
        if (debug_unparse)
        os << "/*AstAnonymousTypeDeclaration:#" << id << "*/";
		class_body->Unparse(os, lex_stream);
    }

	Ast* AstAnonymousTypeDeclaration::Clone(AstStoragePool* ast_pool)
	{
		auto _type_clone =  ast_pool->GenAnonymousTypeDeclaration();
		_type_clone->class_body = reinterpret_cast<AstClassBody*>(class_body->Clone(ast_pool));
		return _type_clone;
	}


    void   AstAnonymousTypeDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
    {
		parent_scope = scope;
        if (visitor->visit(this,scope)) {
			class_body->traverse(visitor, this);
        }
        visitor->endVisit(this,scope);
    }
    void AstAnonymousTypeDeclaration::Print(Ostream& os,LexStream& lex_stream)
    {	
		os << '#' << id << " (AstAnonymousTypeDeclaration): ";
		class_body->Print(os, lex_stream);
    }
	Token* AstAnonymousTypeDeclaration::LeftToken()
	{
		return class_body->left_brace_token;
	}

	Token* AstAnonymousTypeDeclaration::RightToken()
	{
		return class_body->right_brace_token;
	}

	wstring AstAnonymousTypeDeclaration::getName()
	{
		return L"";
	}
} // Close namespace Jikes block

