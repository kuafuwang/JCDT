
#ifndef ClassFile_java_model_INCLUDED_INCLUDED
#define ClassFile_java_model_INCLUDED_INCLUDED

#include "IClassFile.h"
#include <PathModelLib/JarPath/ClassFile.h>


namespace Jikes { // Open namespace Jikes block
	class Zip;
	
namespace JavaModel{

	class JarPackageFragmentRoot;

	class ClassFile 
		 : 
		  public IClassFile
	{
	public:
		Jikes::ClassFile* getClassFileInJar()
		{
			return _class_in_jar->getData();
		}
		JarPackageFragmentRoot* getPackagePackageFragmentRoot();
	
		wstring getFullName() override;
		IJavaElement* ShallowCopy(IJavaElement*) override;

		bool isOpen() override;
		
		IType* getType() override;

	
		ClassFile(IJavaElementType _type,
			IPackageFragment* parent,
			const wstring& name_symbol_);
		
		ClassFile(ClassFile&);

		bool generateInfos(IProgressMonitor* pm) override;

	
		std::vector<IJavaElement*>* getChildren() override;

		void addChild(IJavaElement* child) override;

	
		PathModel::ClassFile* _class_in_jar;
		
	};
	
	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

