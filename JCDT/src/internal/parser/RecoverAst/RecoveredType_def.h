#ifndef RecoveredType_def_INCLUDED
#define RecoveredType_def_INCLUDED


namespace Jikes
{ // Open namespace Jikes block


	RecoveredType::RecoveredType(TypeDeclaration* typeDeclaration, RecoveredElement* parent, int bracketBalance)
		: RecoveredStatement(NULL,parent, bracketBalance)
	{
	}

	RecoveredElement* RecoveredType::add(AbstractMethodDeclaration* methodDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* RecoveredType::add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* RecoveredType::add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* RecoveredType::add(TypeDeclaration* memberTypeDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	void RecoveredType::add(std::vector<AstTypeParameter*>& parameters, int startPos)
	{
	}

	

	/*
	 * Answer the body end of the corresponding parse node
	 */
	int RecoveredType::bodyEnd()
	{
		return 0;
	}

	bool RecoveredType::bodyStartsAtHeaderEnd()
	{
		return true;
	}

	/*
	 * Answer the enclosing type node, or null if none
	 */
	RecoveredType* RecoveredType::enclosingType()
	{
		return NULL;
	}

	

	wstring RecoveredType::name()
	{
		return wstring();
	}

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredType::parseTree()
	{
		return NULL;
	}

	

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredType::sourceEnd()
	{
		return 0;
	}

	wstring RecoveredType::toString(int tab)
	{
		return wstring();
	}

	/*
	 * Update the bodyStart of the corresponding parse node
	 */
	void RecoveredType::updateBodyStart(int bodyStart)
	{
	}

	
	/*
	 * Update the corresponding parse node from parser state which
	 * is about to disappear because of restarting recovery
	 */
	void RecoveredType::updateFromParserState()
	{
	}

	/*
	 * A closing brace got consumed, might have closed the current element,
	 * in which case both the currentElement is exited
	 */
	RecoveredElement* RecoveredType::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		return NULL;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	RecoveredElement* RecoveredType::updateOnOpeningBrace(int braceStart, int braceEnd)
	{
		return NULL;
	}

	void RecoveredType::updateParseTree()
	{
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredType::updateSourceEndIfNecessary(int start, int end)
	{
	}


} // Close namespace Jikes block


#endif // RecoveredType_INCLUDED


