#ifndef RecoveredInitializer_INCLUDED
#define RecoveredInitializer_INCLUDED

#include "RecoveryAst_Pre_declaration.h"

namespace Jikes
{ // Open namespace Jikes block

	class RecoveredInitializer :public RecoveredField
	{
	public:
		vector<RecoveredType*>* localTypes;
		int localTypeCount;

		RecoveredBlock* initializerBody;

		

		RecoveredInitializer(FieldDeclaration* fieldDeclaration,
			RecoveredElement* parent, int bracketBalance)
			;

		RecoveredInitializer(FieldDeclaration* fieldDeclaration,
			RecoveredElement* parent, int bracketBalance, Parser* parser)
			;

		/*
		 * Record a nested block declaration
		 */
		RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
			;

		/*
		 * Record a field declaration (act like inside method body)
		 */
		RecoveredElement* add(FieldDeclaration* newFieldDeclaration, int bracketBalanceValue)
			;

		/*
		 * Record a local declaration - regular method should have been created a block body
		 */
		RecoveredElement* add(LocalDeclaration* localDeclaration, int bracketBalanceValue)
			;
		/*
		 * Record a statement - regular method should have been created a block body
		 */
		RecoveredElement* add(AbstractStatement* statement, int bracketBalanceValue)
			;

		RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
			;

	
		wstring toString(int tab)
			;

		FieldDeclaration* updatedFieldDeclaration()
			;

		/*
		 * A closing brace got consumed, might have closed the current element,
		 * in which case both the currentElement is exited
		 */
		RecoveredElement* updateOnClosingBrace(int braceStart, int braceEnd)
			;

		/*
		 * An opening brace got consumed, might be the expected opening one of the current element,
		 * in which case the bodyStart is updated.
		 */
		RecoveredElement* updateOnOpeningBrace(int braceStart, int braceEnd)
			;

		/*
		 * Update the declarationSourceEnd of the corresponding parse node
		 */
		void updateSourceEndIfNecessary(int braceStart, int braceEnd)
			;
	};
} // Close namespace Jikes block


#endif // _INCLUDED


