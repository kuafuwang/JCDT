#ifndef RecoveredStatement_INCLUDED
#define RecoveredStatement_INCLUDED

#include "RecoveryAst_Pre_declaration.h"

#include <set>

namespace Jikes
{ // Open namespace Jikes block


	using  namespace std;

	class RecoveredStatement :public RecoveredElement
	{
	public:
		AbstractStatement* statement;

		RecoveredStatement(AbstractStatement* statement, 
			RecoveredElement* parent, int bracketBalance);

	
		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree();

		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int sourceEnd()
			;

		wstring toString(int tab)
			;

		AbstractStatement* updatedStatement()
			;

		void updateParseTree()
			;

		/*
		 * Update the declarationSourceEnd of the corresponding parse node
		 */
		void updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
			;

		RecoveredElement* updateOnClosingBrace(int braceStart, int braceEnd)
			;
	};
} // Close namespace Jikes block


#endif // RecoveredStatement_INCLUDED


