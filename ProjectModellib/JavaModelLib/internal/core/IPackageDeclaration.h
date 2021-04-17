
#ifndef IPackageDeclaration_java_model_INCLUDED_INCLUDED
#define IPackageDeclaration_java_model_INCLUDED_INCLUDED
#include "JavaElement.h"
#include "ISourceReference.h"
#include <JavaModelLib/internal/core/IJavaCardDocType.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IPackageDeclaration: public JavaElement, public ISourceReference
	{
	public:

		IPackageDeclaration(IJavaElement* parent , const wstring& name)
			: JavaElement(parent, IJavaElementType::PACKAGE_DECLARATION, name), source_rang(nullptr), name_rang(nullptr)
		{
		}

		 IPackageDeclaration(JavaElement& o)
			: JavaElement(o), source_rang(nullptr), name_rang(nullptr)
		{
		}
		 ISourceRange* getSourceRange() override;
		 void  setSourceRange(ISourceRange*) override;
		 void figureRange() override;
		~IPackageDeclaration();
		IJavaElement* DeepCopy(IJavaElement* parent);
		ISourceRange* getNameRange() override;
		 void setNameRange(ISourceRange*) override;
		
	protected:
		ISourceRange* source_rang;
		ISourceRange* name_rang;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

