#ifndef RecoveredStatement_def_INCLUDED
#define RecoveredStatement_def_INCLUDED


namespace Jikes
{ // Open namespace Jikes block


	RecoveredStatement::RecoveredStatement(AbstractStatement* statement_, RecoveredElement* parent, int bracketBalance)
		: RecoveredElement(parent, bracketBalance)
	{
		statement = statement_;
	}

	

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredStatement::parseTree()
	{
		return statement;
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredStatement::sourceEnd()
	{
		return statement->RightToken()->EndLocation();
	}

	wstring toString(int tab)
	{
		return wstring();
	}

	AbstractStatement* RecoveredStatement::updatedStatement()
	{
		return statement;
	}

	void RecoveredStatement::updateParseTree()
	{
		updatedStatement();
	
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredStatement::updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
	{
	}

}; // Close namespace Jikes block


#endif // RecoveredStatement_INCLUDED


