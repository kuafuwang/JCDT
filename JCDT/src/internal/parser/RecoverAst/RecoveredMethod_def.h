#ifndef RecoveredMethod_def_INCLUDED
#define RecoveredMethod_def_INCLUDED


namespace Jikes
{ // Open namespace Jikes block


	RecoveredMethod::RecoveredMethod(AbstractMethodDeclaration* methodDeclaration,
		RecoveredElement* parent, int bracketBalance, Parser* parser): RecoveredElement(parent, bracketBalance, parser)
	{

	}

	/*
	 * Record a nested block declaration
	 */
	RecoveredMethod::RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	/*
	 * Record a field declaration
	 */
	RecoveredElement* RecoveredMethod::add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	/*
	 * Record a local declaration - regular method should have been created a block body
	 */
	RecoveredElement* RecoveredMethod::add(LocalDeclaration* localDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	/*
	 * Record a statement - regular method should have been created a block body
	 */
	RecoveredElement* RecoveredMethod::add(AbstractStatement* statement, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* RecoveredMethod::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	bool RecoveredMethod::bodyStartsAtHeaderEnd()
	{
		return true;
	}

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredMethod::parseTree()
	{
		return NULL;
	}

	void RecoveredMethod::resetPendingModifiers()
	{
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredMethod::sourceEnd()
	{
		return 0;
	}

	wstring RecoveredMethod::toString(int tab)
	{
		return wstring();
	}

	/*
	 * Update the bodyStart of the corresponding parse node
	 */
	void RecoveredMethod::updateBodyStart(int bodyStart)
	{
	}

	AbstractMethodDeclaration* RecoveredMethod::updatedMethodDeclaration()
	{
		return NULL;
	}

	/*
	 * Update the corresponding parse node from parser state which
	 * is about to disappear because of restarting recovery
	 */
	void RecoveredMethod::updateFromParserState()
	{
	}

	RecoveredElement* RecoveredMethod::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		return NULL;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	RecoveredElement* RecoveredMethod::updateOnOpeningBrace(int braceStart, int braceEnd)
	{
		return NULL;
	}

	void RecoveredMethod::updateParseTree()
	{
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredMethod::updateSourceEndIfNecessary(int braceStart, int braceEnd)
	{
	}


	void RecoveredMethod::attach(std::vector<AstTypeParameter*>* parameters, int startPos)
	{
	}

	
}; // Close namespace Jikes block


#endif // _INCLUDED


