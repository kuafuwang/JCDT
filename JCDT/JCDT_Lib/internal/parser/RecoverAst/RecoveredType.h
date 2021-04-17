#ifndef RecoveredType_INCLUDED
#define RecoveredType_INCLUDED

#include "RecoveryAst_Pre_declaration.h"


namespace Jikes
{ // Open namespace Jikes block


	class RecoveredType :public RecoveredStatement
	{
	public:
		static const int MAX_TYPE_DEPTH = 256;

		TypeDeclaration* typeDeclaration;

		std::vector<RecoveredType*> memberTypes;
		int memberTypeCount;
		std::vector<RecoveredField*> fields;
		int fieldCount;
		std::vector<RecoveredMethod*> methods;
		int methodCount;

		bool preserveContent;
		int m_bodyEnd;

		bool insideEnumConstantPart;

		std::vector<AstTypeParameter*> pendingTypeParameters;
		int pendingTypeParametersStart;

		RecoveredType(TypeDeclaration* typeDeclaration, RecoveredElement* parent, int bracketBalance);

		RecoveredElement* add(AbstractMethodDeclaration* methodDeclaration, int bracketBalanceValue);

		RecoveredElement* add(AstBlock* nestedBlockDeclaration, int bracketBalanceValue);

		RecoveredElement* add(FieldDeclaration* fieldDeclaration, int bracketBalanceValue);

		RecoveredElement* add(TypeDeclaration* memberTypeDeclaration, int bracketBalanceValue);

		void add(vector<AstTypeParameter*>& parameters, int startPos);

		/*
		 * Answer the body end of the corresponding parse node
		 */
		int bodyEnd();

		bool bodyStartsAtHeaderEnd();
		/*
		 * Answer the enclosing type node, or null if none
		 */
		RecoveredType* enclosingType();

	
		wstring name();
		/*
		 * Answer the associated parsed structure
		 */
		Ast* parseTree();
	
		/*
		 * Answer the very source end of the corresponding parse node
		 */
		int sourceEnd();
		wstring toString(int tab);
		/*
		 * Update the bodyStart of the corresponding parse node
		 */
		void updateBodyStart(int bodyStart);
		AbstractStatement* updatedStatement();
		TypeDeclaration* updatedTypeDeclaration();
		/*
		 * Update the corresponding parse node from parser state which
		 * is about to disappear because of restarting recovery
		 */
		void updateFromParserState();
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

		void updateParseTree();


			/*
			 * Update the declarationSourceEnd of the corresponding parse node
			 */
			void updateSourceEndIfNecessary(int start, int end);


		
	
	};
} // Close namespace Jikes block


#endif // RecoveredType_INCLUDED


