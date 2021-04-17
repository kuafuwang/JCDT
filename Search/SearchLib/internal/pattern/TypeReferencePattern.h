#ifndef  TypeReferencePattern_JIKES_SERACH_INCLUDE
#define TypeReferencePattern_JIKES_SERACH_INCLUDE
#include "MultipleSearchPattern.h"

namespace Jikes{
	class AstImportDeclaration;
	class AstArrayType;
	class AstTypeName;
	class AstName;
	class AstType;
}

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class TypeReferencePattern:public MultipleSearchPattern
	{
	public:
		 std::wstring qualification;
		 std::wstring simpleName;

	
		TypeReferencePattern(
			std::wstring _qualification,
			std::wstring _simpleName,
			int _matchMode,
			bool _isCaseSensitive);


		/**
		* @see SearchPattern#matchContainer()
		*/
		 int matchContainer() {
			return COMPILATION_UNIT | CLASS | METHOD | FIELD;
		}
	

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		
	
		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
		std::wstring indexEntryPrefix() override{
			return simpleName;
		};
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
			int accuracy, MatchSet* locator);
	};

	
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif