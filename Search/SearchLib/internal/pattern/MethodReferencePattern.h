#ifndef  MethodReferencePattern_JIKES_SERACH_INCLUDE
#define MethodReferencePattern_JIKES_SERACH_INCLUDE
#include "MethodPattern.h"

namespace Jikes { // Open namespace Jikes block
	class AstMethodInvocation;
	class MethodSymbol;

	namespace JavaModel {
		class IType;
	}
	namespace PathModel {
		class MemberInFileMoses;
	}
	
namespace Search{
	class MethodReferencePattern :public MethodPattern {
	public:
		PathModel::MemberInFileMoses* declaringType;
		std::vector < std::wstring  >* allSuperDeclaringTypeNames;
	
		MethodReferencePattern(
			std::wstring _selector,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _declaringQualification,
			std::wstring _declaringSimpleName,
			std::wstring _returnQualification,
			std::wstring _returnSimpleName,
			std::vector<std::wstring>* _parameterQualifications,
			std::vector<std::wstring>* _parameterSimpleNames,
			PathModel::MemberInFileMoses* _declaringType);

		wstring getPatternName() {
			return L"MethodReferencePattern: "; //$NON-NLS-1$
		}
		int matchContainer() {
			return METHOD | FIELD;
		}

		void initializePolymorphicSearch(MatchSet* locator, IProgressMonitor* progressMonitor);

		/**
		* Returns whether the code gen will use an invoke virtual for
		* this message send or not.
		*/
		bool isVirtualInvoke(MethodSymbol* method, AstMethodInvocation* messageSend);

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding, AstMethodInvocation* messageSend);
		/**
		* @see SearchPattern#matchReportReference
		*/
		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
		                          int accuracy, MatchSet* locator);

	
		 
	
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif
