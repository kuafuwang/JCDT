
#ifndef ICompilationUnit_java_model_INCLUDED_INCLUDED
#define ICompilationUnit_java_model_INCLUDED_INCLUDED

#include  "BaseJavaFile.h"



namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IPackageDeclaration;
	class IPackageFragment;
	class IType;
	class IImportDeclaration;

	class ICompilationUnit :
		public BaseJavaFile
		 
	{
	public:
		
		ICompilationUnit(IJavaElementType _type, IPackageFragment* p, const wstring& name)
			: BaseJavaFile(_type, p, name)
		{
		}

		ICompilationUnit(BaseJavaFile& base_java_file)
			: BaseJavaFile(base_java_file)
		{
		}
		virtual const vector<IType*>* getTypes() = 0;
		virtual IType* getType(const wstring& name) = 0;

		virtual const vector<IPackageDeclaration*>* getPackageDeclarations() = 0; 
		virtual const vector<IImportDeclaration*>* getImports() = 0;
		virtual IPackageDeclaration* getPackageDeclaration(const wstring& name) =0;
	/*	virtual IType* createIndependentType(const wstring& name) = 0;*/
		virtual IImportDeclaration* getImport(const wstring& name) = 0;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

