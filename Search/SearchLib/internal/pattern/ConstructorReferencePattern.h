#ifndef  ConstructorReferencePattern_JIKES_SERACH_INCLUDE
#define ConstructorReferencePattern_JIKES_SERACH_INCLUDE
#include "MethodReferencePattern.h"

namespace Jikes {
	class AstSuperCall;
	class AstThisCall;
	// Open namespace Jikes block
	class AstClassCreationExpression;
	class ExplicitConstructorCall;

namespace Search{
	class ConstructorReferencePattern:public MethodReferencePattern
	{
	public:
		ConstructorReferencePattern(
			std::wstring _declaringSimpleName,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _declaringQualification,
			std::vector< std::wstring >* _parameterQualifications,
			std::vector<std::wstring >* _parameterSimpleNames,
			PathModel::MemberInFileMoses* _declaringType) :MethodReferencePattern(_declaringSimpleName,_matchMode, _isCaseSensitive,
				_declaringQualification, _declaringSimpleName,L"",L"",_parameterQualifications,_parameterSimpleNames,_declaringType)
		{
		}


		/**
		* Returns whether this constructor pattern  matches the given allocation expression.
		* Look at resolved information only if specified.
		*/
		int matchLevel(AstClassCreationExpression* allocation, bool resolve);

		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* Returns whether this constructor pattern  matches the given explicit constructor call.
		* Look at resolved information only if specified.
		*/
		
		int matchLevel(AstSuperCall* call, bool resolve);
		int matchLevel(AstThisCall* call, bool resolve);
		/**
		* @see SearchPattern#matchLevel(Binding binding).
		*/
		int matchLevel(Symbol* binding);

		void matchReportReference(Ast* reference, JavaModel::IJavaElement* element,
			int accuracy, MatchSet* locator) override;

		bool IsMayInTheFile(BaseLexStream*) override;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif