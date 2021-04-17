#ifndef RecoveredImport_INCLUDED
#define RecoveredImport_INCLUDED

#include <string>
using std::wstring;
#include "RecoveryAst_Pre_declaration.h"

namespace Jikes
{ // Open namespace Jikes block

	class RecoveredImport :public RecoveredElement
	{
		ImportReference* importReference;

		RecoveredImport(ImportReference* importReference, RecoveredElement* parent, int bracketBalance)
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

		ImportReference* updatedImportReference()
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


