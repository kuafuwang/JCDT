
#ifndef IPackageFragment_java_model_INCLUDED_INCLUDED
#define IPackageFragment_java_model_INCLUDED_INCLUDED

#include "Openable.h"
#include <vector>

#include <PathModelLib/DeltaPath/ResouceOptStack.h>
namespace Jikes{
	class NameSymbol;
}
using namespace Jikes::PathModel;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class BaseJavaFile;

	class IClassFile;
	class ICompilationUnit;
	class IPackageFragmentRoot;

	class IPackageFragment
		:public Openable
		
	{
		

	public:
		IPackageFragment(IJavaElement* parent, IJavaElementType _type, const wstring& name)
			: Openable(parent, _type, name)
		{
		}

		 IPackageFragment(Openable& o)
			: Openable(o)
		{
		}
		enum FileType
		{
			SourceType,
			BinaryType
		};
		 static const wstring DEFAULT_PACKAGE_NAME ; //$NON-NLS-1$
		
	virtual IClassFile*	FindOrInsertClassFile(const wstring&  name) = 0;
	virtual ICompilationUnit*	FindOrInsertCompilationUnit(const wstring&  name) = 0;
	virtual		IClassFile* getClassFile(const wstring&  name) = 0;
		
	virtual		std::vector<IClassFile*> getClassFiles()  = 0;
		
	virtual		ICompilationUnit* getCompilationUnit(const wstring& name) = 0;
		
	virtual		std::vector<ICompilationUnit*> getCompilationUnits() = 0 ;
	
	
	virtual     void CopyChildren(IPackageFragment* parent) = 0;
		
	virtual		int getKind()  = 0;
		
	virtual		bool hasSubpackages() = 0 ;
	
	virtual		bool isDefaultPackage() = 0;


	virtual void ProcessRemoveElement(map<wstring, NameLookUpNotify*>&) = 0;
	virtual void SynchronousProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >&,IPackageFragment*) = 0;
	virtual void IncrementUpdateProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >&) = 0;

	virtual wstring getCompoundName() = 0;
	};


}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

