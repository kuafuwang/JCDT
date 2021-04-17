
#ifndef IImportDeclaration_java_model_INCLUDED_INCLUDED
#define IImportDeclaration_java_model_INCLUDED_INCLUDED

#include "JavaElement.h"
#include "ISourceReference.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IImportDeclaration : public JavaElement,public ISourceReference
	{
	public:
		IImportDeclaration(IJavaElement* parent, const wstring& name);

		IImportDeclaration(IImportDeclaration& o);
		ISourceRange* getSourceRange() override;
		void  setSourceRange(ISourceRange*) override;
		void figureRange() override;
		~IImportDeclaration();
		virtual bool isOnDemand();
		IJavaElement* DeepCopy(IJavaElement*) override;

		ISourceRange* getNameRange() override;
		void setNameRange(ISourceRange*) override;
	
	protected:
		ISourceRange* source_rang;
		ISourceRange* name_rang;
	};



	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

