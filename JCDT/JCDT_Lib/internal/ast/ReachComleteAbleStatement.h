#pragma once

#ifndef ReachComleteAbleStatement_INCLUDED
#define ReachComleteAbleStatement_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AbstractStatement.h"
namespace Jikes { // Open namespace Jikes block

	// This class represents statements.
	class ReachComleteAbleStatement :  public AbstractStatement
	{
	public:
		bool is_reachable;
		bool can_complete_normally;

		static ReachComleteAbleStatement* StatementCast(Ast * node)
		{
			return DYNAMIC_CAST<ReachComleteAbleStatement*>(node->class_tag == STATEMENT ? node : NULL);
		}

		  ReachComleteAbleStatement(AstKind k, bool reachable = false,
			bool can_complete = false)
			: AbstractStatement(k, STATEMENT)
			, is_reachable(reachable)
			, can_complete_normally(can_complete)
		{}
		~ReachComleteAbleStatement() {}
	};






} // Close namespace Jikes block


#endif // AstStatement_INCLUDED

