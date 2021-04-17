#ifndef RecoveredLocalVariable_def_INCLUDED
#define RecoveredLocalVariable_def_INCLUDED


namespace Jikes
{ // Open namespace Jikes block


	RecoveredLocalVariable::RecoveredLocalVariable(LocalDeclaration* localDeclaration, 
		RecoveredElement* parent, int bracketBalance): RecoveredStatement(localDeclaration,parent, bracketBalance)
	{
		localDeclaration = localDeclaration;
		//alreadyCompletedLocalInitialization = localDeclaration->initialization != nullptr;
	}

	/*
	 * Record an expression statement if local variable is expecting an initialization expression.
	 */
	RecoveredElement* RecoveredLocalVariable::add(AbstractStatement* stmt, int bracketBalanceValue)
	{
		if (alreadyCompletedLocalInitialization || !( dynamic_cast< Expression*>(stmt))) {
			return RecoveredStatement::add(stmt, bracketBalanceValue);
		}
		else {
			alreadyCompletedLocalInitialization = true;
		//	localDeclaration->initialization = (Expression)stmt;
		//	localDeclaration->RightToken()->EndLocation() = stmt->RightToken()->EndLocation();
		//	localDeclaration->declarationEnd = stmt->RightToken()->EndLocation();
			return this;
		}
	}

	

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredLocalVariable::parseTree()
	{
		return localDeclaration;
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredLocalVariable::sourceEnd()
	{
		return localDeclaration->RightToken()->EndLocation();
	}

	wstring RecoveredLocalVariable::toString(int tab)
	{
		return wstring();
	}

	AbstractStatement* RecoveredLocalVariable::updatedStatement()
	{
		return localDeclaration;
	}

	/*
	 * A closing brace got consumed, might have closed the current element,
	 * in which case both the currentElement is exited.
	 *
	 * Fields have no associated braces, thus if matches, then update parent.
	 */
	RecoveredElement* RecoveredLocalVariable::updateOnClosingBrace(int braceStart, int braceEnd)
	{
		if (bracketBalance > 0) { // was an array initializer
			bracketBalance--;
			if (bracketBalance == 0) alreadyCompletedLocalInitialization = true;
			return this;
		}
		if (parent != nullptr) {
			return parent->updateOnClosingBrace(braceStart, braceEnd);
		}
		return this;
	}

	/*
	 * An opening brace got consumed, might be the expected opening one of the current element,
	 * in which case the bodyStart is updated.
	 */
	RecoveredElement* RecoveredLocalVariable::updateOnOpeningBrace(int braceStart, int braceEnd)
	{

		//if (localDeclaration->RightToken()->EndLocation() == 0
		//	&& (localDeclaration->type instanceof ArrayTypeReference || localDeclaration->type instanceof ArrayQualifiedTypeReference)
		//	&& !alreadyCompletedLocalInitialization) {
		//	bracketBalance++;
		//	return nullptr; // no update is necessary	(array initializer)
		//}
		//// might be an array initializer
		//updateSourceEndIfNecessary(braceStart - 1, braceEnd - 1);
		//return parent->updateOnOpeningBrace(braceStart, braceEnd);


		return NULL;
	}

	void RecoveredLocalVariable::updateParseTree()
	{
		updatedStatement();
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredLocalVariable::updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
	{
	/*	if (localDeclaration->RightToken()->EndLocation() == 0) {
			localDeclaration->RightToken()->EndLocation() = bodyEnd;
			localDeclaration->declarationEnd = bodyEnd;
		}*/
	}
}; // Close namespace Jikes block


#endif // RecoveredLocalVariable_INCLUDED


