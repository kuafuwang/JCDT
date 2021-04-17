#pragma once

#ifndef AstDeclared_INCLUDED
#define AstDeclared_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AbstractStatement.h"

namespace Jikes { // Open namespace Jikes block


	//
	// This class adds some type safety. It represents all member declarations
	// in types. See DeclaredType, FieldDeclaration, MethodDeclaration,
	// ConstructorDeclaration, Initializer, and AstEnumConstant.
	//
	class AstDeclared : public AbstractStatement
	{
	public:
		AstModifiers* modifiers_opt;

		 AstDeclared(AstKind k)
			: AbstractStatement(k), modifiers_opt(nullptr)
		{}
		
		~AstDeclared() {}
	};




} // Close namespace Jikes block


#endif // AstDeclared_INCLUDED

