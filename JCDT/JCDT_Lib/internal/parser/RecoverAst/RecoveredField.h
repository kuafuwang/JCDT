#ifndef RecoveredField_INCLUDED
#define RecoveredField_INCLUDED

#include "RecoveryAst_Pre_declaration.h"
#include <string>
using std::wstring;

namespace Jikes{
 // Open namespace Jikes block

	class RecoveredField :public RecoveredElement
	{	
	public:

		FieldDeclaration* fieldDeclaration;
		bool alreadyCompletedFieldInitialization;


		vector<RecoveredType*>* anonymousTypes;

		int anonymousTypeCount;

		RecoveredField(FieldDeclaration* fieldDeclaration, RecoveredElement* parent, int bracketBalance)
		
		;

		RecoveredField(FieldDeclaration* fieldDeclaration,
		               RecoveredElement* parent, int bracketBalance, Parser* parser)
		
		;

	

		/*
		 * Record an expression statement if field is expecting an initialization expression,
		 * used for completion inside field initializers.
		 */
		RecoveredElement* add(AbstractStatement* statement, int bracketBalanceValue)
		
			
		;

		/*
		 * Record a type declaration if this field is expecting an initialization expression
		 * and the type is an anonymous type.
		 * Used for completion inside field initializers.
		 */
		RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)
		
			
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

		FieldDeclaration* updatedFieldDeclaration()
		
			
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


#endif // _INCLUDED


