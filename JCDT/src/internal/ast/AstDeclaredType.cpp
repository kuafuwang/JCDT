
#ifndef Jikes_AstDeclaredType_def_INCLUDED
#define Jikes_AstDeclaredType_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/lex/Token.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
namespace Jikes { // Open namespace Jikes block

	TypeDeclaration::TypeDeclaration(AstKind k): AstDeclared(k), _level(TOP_LEVEL), class_body(nullptr), symbol(nullptr)
	{}

	bool TypeDeclaration::IsValid()
	{
		return class_body && class_body->semantic_environment;
	}

	wstring TypeDeclaration::getName()
	{
		return class_body->identifier_token->getName();
	}


	Symbol* TypeDeclaration::GetSymbol()
	{
		return symbol;
	}
} // Close namespace Jikes block


#endif // AstWildcard_def_INCLUDED
