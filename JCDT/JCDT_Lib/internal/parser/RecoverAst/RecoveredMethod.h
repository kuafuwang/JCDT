#ifndef RecoveredMethod_INCLUDED
#define RecoveredMethod_INCLUDED

#include "RecoveryAst_Pre_declaration.h"
#include <vector>
namespace Jikes
{ // Open namespace Jikes block

	using namespace std;

	class RecoveredMethod :public RecoveredElement
	{
	public:

		AbstractMethodDeclaration* methodDeclaration;

		vector<RecoveredType*> localTypes;
		int localTypeCount;

		RecoveredBlock* methodBody;
		bool discardBody;


		RecoveredMethod(AbstractMethodDeclaration* methodDeclaration,
			RecoveredElement* parent, int bracketBalance, Parser* parser)
			;

		/*
		 * Record a nested block declaration
		 */
		RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue)
			;

		/*
		 * Record a field declaration
		 */
		RecoveredElement* add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue)
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

		bool bodyStartsAtHeaderEnd()
			;

		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree()
			;

		void resetPendingModifiers()
			;

		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int sourceEnd()
			;
		wstring toString(int tab)
			;

		/*
		 * Update the bodyStart of the corresponding parse node
		 */
		void updateBodyStart(int bodyStart)
			;

		AbstractMethodDeclaration* updatedMethodDeclaration()
			;

		/*
		 * Update the corresponding parse node from parser state which
		 * is about to disappear because of restarting recovery
		 */
		void updateFromParserState()
			;

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
		void updateSourceEndIfNecessary(int braceStart, int braceEnd)
			;

		
		void attach(vector<AstTypeParameter*>* parameters, int startPos)
			;
	
	};
} // Close namespace Jikes block


#endif // _INCLUDED


