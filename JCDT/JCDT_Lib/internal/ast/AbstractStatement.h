#pragma once

#ifndef AbstractStatement_INCLUDED
#define AbstractStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block


	//
	// This class represents statements interface
	//
	class AbstractStatement : public Ast
	{
	public:
		AbstractStatement(AstKind k, AstTag t = NO_TAG) :Ast(k, t)
		{		
		}
		virtual ~AbstractStatement() {}
	};




} // Close namespace Jikes block


#endif // AbstractStatement_INCLUDED

