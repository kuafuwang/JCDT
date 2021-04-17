#ifndef  ConstructorDeclarationPattern_JIKES_SERACH_INCLUDE
#define ConstructorDeclarationPattern_JIKES_SERACH_INCLUDE
#include "MethodDeclarationPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class ConstructorDeclarationPattern:public MethodDeclarationPattern
	{
	public:
		ConstructorDeclarationPattern(std::wstring declaringSimpleName,
		                              int _matchMode,
		                              bool _isCaseSensitive,
		                              std::wstring declaringQualification,
		                              std::vector<std::wstring>* parameterQualifications, 
			std::vector<std::wstring>* parameterSimpleNames);
		/**
		* @see SearchPattern#matchesBinary(Object, Object)
		*/
		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, std::pair<const ConstantPool*, CPClassInfo*>& class_data);

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);

	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif