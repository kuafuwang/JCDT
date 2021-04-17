
#ifndef RecoveredUnit_def_INCLUDED
#define RecoveredUnit_def_INCLUDED


namespace Jikes
{ // Open namespace Jikes block

	/*
	 *	Record a method declaration: should be attached to last type
	 */
	RecoveredElement* RecoveredUnit::add(AbstractMethodDeclaration* methodDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	/*
	 *	Record a field declaration: should be attached to last type
	 */
	RecoveredElement* RecoveredUnit::add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* add(ImportReference* importReference, int bracketBalanceValue)
	{
		return NULL;
	}

	RecoveredElement* RecoveredUnit::add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
	{
		return NULL;
	}

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredUnit::parseTree()
	{
		return NULL;
	}

	
		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int RecoveredUnit::sourceEnd()
			{
				return 0;
			}
		wstring RecoveredUnit::toString(int tab)
			{
				return wstring();
			}
		CompilationUnitDeclaration* RecoveredUnit::updatedCompilationUnitDeclaration()
			{
				return NULL;
			}
		void RecoveredUnit::updateParseTree()
			{
			}
		/*
		 * Update the sourceEnd of the corresponding parse node
		 */
		void RecoveredUnit::updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
			{
			}
	} // Close namespace Jikes block


#endif // RecoveredUnit_INCLUDED

