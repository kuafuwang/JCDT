#ifndef RecoveredUnit_INCLUDED
#define RecoveredUnit_INCLUDED
#include "RecoveryAst_Pre_declaration.h"
#include <vector>
#include <string>
using std::wstring;


namespace Jikes
{ // Open namespace Jikes block

	class RecoveredUnit :public RecoveredElement
	{
	public:
		CompilationUnitDeclaration* unitDeclaration;

		vector<RecoveredImport*>* imports;
		int importCount;
		vector<RecoveredType*>* types;

		int typeCount;


		RecoveredUnit(CompilationUnitDeclaration* unitDeclaration, int bracketBalance,
		              Parser* parser);
		
		/*
		 *	Record a method declaration: should be attached to last type
		 */
		RecoveredElement* add(AbstractMethodDeclaration* methodDeclaration,
		                      int bracketBalanceValue);
		/*
		 *	Record a field declaration: should be attached to last type
		 */
		RecoveredElement* add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue);
		RecoveredElement* add(ImportReference* importReference, int bracketBalanceValue);
		RecoveredElement* add(TypeDeclaration* typeDeclaration, int bracketBalanceValue);
		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree();
		
		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int sourceEnd();

		wstring toString(int tab);
		CompilationUnitDeclaration* updatedCompilationUnitDeclaration();
		void updateParseTree();
		/*
		 * Update the sourceEnd of the corresponding parse node
		 */
		void updateSourceEndIfNecessary(int bodyStart, int bodyEnd);
	};
} // Close namespace Jikes block


#endif // RecoveredUnit_INCLUDED


