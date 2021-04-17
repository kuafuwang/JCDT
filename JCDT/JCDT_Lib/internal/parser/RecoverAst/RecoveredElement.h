#ifndef RecoveredElement_INCLUDED
#define RecoveredElement_INCLUDED

#include "RecoveryAst_Pre_declaration.h"
#include <string>
using std::wstring;

namespace Jikes {
	class ConstructionDeclaration;

	// Open namespace Jikes block


	class RecoveredElement {

	public:
		virtual ~RecoveredElement(){};

		RecoveredElement* parent;
		int bracketBalance;
		bool foundOpeningBrace;
		Parser* recoveringParser;

		RecoveredElement(RecoveredElement* parent, int bracketBalance)

			;

		RecoveredElement(RecoveredElement* parent, int bracketBalance, Parser* parser)

			;

		
		/*
		 *	Record a method declaration
		 */
		virtual RecoveredElement* add(AbstractMethodDeclaration* methodDeclaration, int bracketBalanceValue);
		/*
		 * Record a nested block declaration
		 */
		virtual RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)

			
		;

		/*
		 * Record a field declaration
		 */
		virtual RecoveredElement* add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)

			
		;

		/*
		 *	Record an import reference
		 */
		virtual RecoveredElement* add(ImportReference* importReference, int bracketBalanceValue)

			
		;

		/*
		 * Record a local declaration
		 */
		virtual RecoveredElement* add(LocalDeclaration* localDeclaration, int bracketBalanceValue)

			
		;

		/*
		 * Record a statement
		 */
		virtual RecoveredElement* add(AbstractStatement* statement, int bracketBalanceValue)

			
		;

		/*
		 *	Record a type declaration
		 */
		virtual RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue)

			
		;

		virtual void addBlockStatement(RecoveredBlock* recoveredBlock)

			;

	

		/*
		 * Answer the depth of this element, considering the parent link.
		 */
		virtual int depth()

			
		;

		/*
		 * Answer the enclosing method node, or null if none
		 */
		virtual RecoveredInitializer* enclosingInitializer()

			
		;

		/*
		 * Answer the enclosing method node, or null if none
		 */
		virtual RecoveredMethod* enclosingMethod()

			
		;

		/*
		 * Answer the enclosing type node, or null if none
		 */
		virtual RecoveredType* enclosingType()

			
		;

		/*
		 * Answer the closest specified parser
		 */
		virtual Parser* parser()

			
		;

		/*
		 * Answer the associated parsed structure
		 */
		virtual Ast* parseTree()

			
		;

	

		/*
		 * Iterate the enclosing blocks and tag them so as to preserve their content
		 */
		virtual void preserveEnclosingBlocks()

			;

		/*
		 * Answer the position of the previous line end if
		 * there is nothing but spaces in between it and the
		 * line end. Used to trim spaces on unclosed elements.
		 */
		virtual int previousAvailableLineEnd(int position)

			
		;

		/*
		 * Answer the very source end of the corresponding parse node
		 */
		virtual int sourceEnd()

			
		;

		virtual wstring tabString(int tab);

			
	
		/*
		 * Answer the top node
		 */
		virtual RecoveredElement* topElement()

			
		;

		virtual wstring toString()

			
		;

		virtual wstring toString(int tab)

			
		;

		/*
		 * Answer the enclosing type node, or null if none
		 */
		virtual RecoveredType* type()

			
		;

		/*
		 * Update the bodyStart of the corresponding parse node
		 */
		virtual void updateBodyStart(int bodyStart)

			;

		/*
		 * Update the corresponding parse node from parser state which
		 * is about to disappear because of restarting recovery
		 */
		virtual void updateFromParserState()

			// default implementation: do nothing
			;

		/*
		 * A closing brace got consumed, might have closed the current element,
		 * in which case both the currentElement is exited
		 */
		virtual RecoveredElement* updateOnClosingBrace(int braceStart, int braceEnd)

			
		;

		/*
		 * An opening brace got consumed, might be the expected opening one of the current element,
		 * in which case the bodyStart is updated.
		 */
		 /* RecoveredElement updateOnOpeningBrace(int braceEnd);*/
		virtual RecoveredElement* updateOnOpeningBrace(int braceStart, int braceEnd)

			
		;

		/*
		 * Final update the corresponding parse node
		 */
		virtual void updateParseTree()

			// default implementation: do nothing
			;

		/*
		 * Update the declarationSourceEnd of the corresponding parse node
		 */
		virtual void updateSourceEndIfNecessary(int braceStart, int braceEnd)

			// default implementation: do nothing
			;

		virtual void updateSourceEndIfNecessary(int sourceEnd)

			;
	};
} // Close namespace Jikes block


#endif // _INCLUDED


