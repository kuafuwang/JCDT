
#ifndef RecoveredBlock_INCLUDED
#define RecoveredBlock_INCLUDED
#include  "RecoveryAst_Pre_declaration.h"

#include <vector>
#include <set>
#include <string>

using std::vector;


namespace Jikes { // Open namespace Jikes block

	
	class RecoveredBlock :public RecoveredStatement
	{
	public:


		AstBlock* blockDeclaration;
		vector<RecoveredStatement*> statements;
		int statementCount;
		bool preserveContent =false;
		RecoveredLocalVariable* pendingArgument;


		RecoveredBlock(AstBlock* block, RecoveredElement* parent, int bracketBalance);
		
		/*
		 * Record a nested block declaration
		 */
		RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue);
		/*
		 * Record a local declaration
		 */
		RecoveredElement* add(LocalDeclaration* localDeclaration, int bracketBalanceValue);
		/*
		 * Record a local declaration
		 */
		RecoveredElement* add(LocalDeclaration* localDeclaration, int bracketBalanceValue,
			bool delegatedByParent);
		/*
		 * Record a statement declaration
		 */
		RecoveredElement* add(AbstractStatement* stmt, int bracketBalanceValue);

		/*
		 * Record a statement declaration
		 */
		RecoveredElement* add(AbstractStatement* stmt, int bracketBalanceValue,
			bool delegatedByParent);
		/*
		 * Addition of a type to an initializer (act like inside method body)
		 */
		RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue);
		/*
		 * Addition of a type to an initializer (act like inside method body)
		 */
		RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue,
			bool delegatedByParent);


		/*
		 * Attach a recovered statement
		 */
		void attach(RecoveredStatement* recoveredStatement);
		
		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree();
	
		
		AstBlock* updatedBlock();
		/*
		 * Rebuild a statement from the nested structure which is in scope
		 */
		AbstractStatement* updatedStatement();
		/*
		 * A closing brace got consumed, might have closed the current element,
		 * in which case both the currentElement is exited
		 */
		RecoveredElement* updateOnClosingBrace(int braceStart, int braceEnd);
		/*
		 * An opening brace got consumed, might be the expected opening one of the current element,
		 * in which case the bodyStart is updated.
		 */
		RecoveredElement* updateOnOpeningBrace(int braceStart, int braceEnd);
		/*
		 * Final update the corresponding parse node
		 */
		void updateParseTree();
		/*
		 * Rebuild a flattened block from the nested structure which is in scope
		 */
		AbstractStatement* updateStatement();

		/*
		 * Record a field declaration
		 */
		RecoveredElement* add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue);

	};

}// end namespace

#endif // _INCLUDED