#ifndef  MethodDeclarationPattern_JIKES_SERACH_INCLUDE
#define MethodDeclarationPattern_JIKES_SERACH_INCLUDE
#include "MethodPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class MethodDeclarationPattern:public MethodPattern
	{
	public:
		MethodDeclarationPattern(
			std::wstring _selector,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _declaringQualification,
			std::wstring _declaringSimpleName,
			std::wstring _returnQualification,
			std::wstring _returnSimpleName,
			std::vector<std::wstring> *_parameterQualifications,
			std::vector<std::wstring> *_parameterSimpleNames);

		wstring getPatternName() {
			return L"MethodDeclarationPattern: "; //$NON-NLS-1$
		}
		int matchContainer() {
			return CLASS;
		}
		/**
		* @see SearchPattern#matchesBinary(Object, Object)
		*/
		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
		bool matchesBinaryPackge(const std::wstring& compoundPackageName)override;
		bool matchesBinaryClass(const std::wstring& mainTypeName) override;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif