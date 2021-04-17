
#ifndef CompilationUnit_java_model_INCLUDED_INCLUDED
#define CompilationUnit_java_model_INCLUDED_INCLUDED
#include "ICompilationUnit.h"
#include <boost/smart_ptr/shared_ptr.hpp>


namespace Jikes { // Open namespace Jikes block

	class FileSymbol;
	class JikesError;
	class CompilationUnitDeclaration;
	class LexStream;
	
namespace JavaModel{

	class IType;
	class IImportDeclaration;


	class CompilationUnit : public ICompilationUnit
	{
	public:
		wstring m_path;
		wstring getFullName() override
		{
			return L"CompilationUnit file " + fName;
		}
	
		 const vector<IType*>* getTypes() override;
		 IType* getType(const wstring& name) override;

		const vector<IPackageDeclaration*>* getPackageDeclarations() override;

		IPackageDeclaration* getPackageDeclaration(const wstring& name) override;

		IJavaElement* ShallowCopy(IJavaElement*) override;

		CompilationUnit(IJavaElementType _type,IPackageFragment* parent, const wstring& name_symbol_);
		CompilationUnit(CompilationUnit&);
	
		bool generateInfos(IProgressMonitor* pm) override;
	
	/*	IType* createIndependentType(const wstring& name) override;*/
		wstring getPath() override;

		IImportDeclaration* getImport(const wstring& name) override;
		const vector<IImportDeclaration*>* getImports() override;
	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

