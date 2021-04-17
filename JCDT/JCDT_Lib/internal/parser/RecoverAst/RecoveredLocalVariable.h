#ifndef RecoveredLocalVariable_INCLUDED
#define RecoveredLocalVariable_INCLUDED

#include "RecoveryAst_Pre_declaration.h"

namespace Jikes
{ // Open namespace Jikes block


	class RecoveredLocalVariable :public RecoveredStatement
	{
	public:


		LocalDeclaration* localDeclaration;
		bool alreadyCompletedLocalInitialization;

		RecoveredLocalVariable(LocalDeclaration* localDeclaration, RecoveredElement* parent, int bracketBalance)
			;

		/*
		 * Record an expression statement if local variable is expecting an initialization expression.
		 */
		RecoveredElement* add(AbstractStatement* stmt, int bracketBalanceValue)
			;

		

		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree()
			;

		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int sourceEnd()
			;

		wstring toString(int tab)
			;

		AbstractStatement* updatedStatement()
			;

		/*
		 * A closing brace got consumed, might have closed the current element,
		 * in which case both the currentElement is exited.
		 *
		 * Fields have no associated braces, thus if matches, then update parent.
		 */
		RecoveredElement* updateOnClosingBrace(int braceStart, int braceEnd)
			;

		/*
		 * An opening brace got consumed, might be the expected opening one of the current element,
		 * in which case the bodyStart is updated.
		 */
		RecoveredElement* updateOnOpeningBrace(int braceStart, int braceEnd)
			;

		void updateParseTree()
			;

		/*
		 * Update the declarationSourceEnd of the corresponding parse node
		 */
		void updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
			;
	};
} // Close namespace Jikes block


#endif // RecoveredLocalVariable_INCLUDED


