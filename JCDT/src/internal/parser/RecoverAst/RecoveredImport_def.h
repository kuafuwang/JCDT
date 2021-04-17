#ifndef RecoveredImport_def_INCLUDED
#define RecoveredImport_def_INCLUDED

#include <string>
using std::wstring;

namespace Jikes
{ // Open namespace Jikes block


	RecoveredImport::RecoveredImport(ImportReference* importReference_,
		RecoveredElement* parent, int bracketBalance): RecoveredElement(parent, bracketBalance)
	{
		importReference = importReference_;
	}

	/*
	 * Answer the associated parsed structure
	 */
	Ast* RecoveredImport::parseTree()
	{
		return importReference;
	}

	/*
	 * Answer the very source end of the corresponding parse node
	 */
	int RecoveredImport::sourceEnd()
	{
		return importReference->RightToken()->EndLocation();
	}

	wstring RecoveredImport::toString(int tab)
	{
		return wstring();
	}

	ImportReference* RecoveredImport::updatedImportReference()
	{
		return NULL;
	}

	void RecoveredImport::updateParseTree()
	{
		updatedImportReference();
	}

	/*
	 * Update the declarationSourceEnd of the corresponding parse node
	 */
	void RecoveredImport::updateSourceEndIfNecessary(int bodyStart, int bodyEnd)
	{
		if (importReference->RightToken()->EndLocation() == 0) {
			
		}
	}
}; // Close namespace Jikes block


#endif // _INCLUDED

