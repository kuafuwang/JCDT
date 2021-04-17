
#ifndef IndependentSourceType_java_model_INCLUDED_INCLUDED
#define IndependentSourceType_java_model_INCLUDED_INCLUDED

#include <JavaModelLib/internal/core/SourceType.h>


namespace Jikes {// Open namespace Jikes block
	class AstClassBody;
	class AstMethodDeclarator;
	class FieldDeclaration;
	class TypeDeclaration;
	class TypeSymbol;
	class LexStream;
namespace JavaModel{
	
	class IndependentSourceType:public SourceType
	{
	public:

		IJavaElement* DeepCopy(IJavaElement*) override;

		IJavaElement* ShallowCopy(IJavaElement*) override;

		IndependentSourceType(IJavaElement* parent,const wstring& name);

		IndependentSourceType(IndependentSourceType& o);
		MemberElementInfo* getElementInfo() override;;


	
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

