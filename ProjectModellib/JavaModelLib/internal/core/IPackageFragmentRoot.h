
#ifndef IPackageFragmentRoot_java_model_INCLUDED_INCLUDED
#define IPackageFragmentRoot_java_model_INCLUDED_INCLUDED
#include "IJavaElement.h"
#include "Openable.h"
#include "JCDT_Lib/internal/core/IProgressMonitor.hpp"
#include <PathModelLib/DeltaPath/ResouceOptStack.h>
using std::map;
using namespace Jikes::PathModel;
namespace Jikes
{
	namespace PathModel
	{
		class IClasspathEntry;
	}
}
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class IPackageFragment;
	
	class IJavaProject;

	class IPackageFragmentRoot:public  Openable
	{
		

	public:
		IPackageFragmentRoot(IJavaElement* parent, IJavaElementType _type, const wstring& name)
			: Openable(parent, _type, name)
		{
		}

		 IPackageFragmentRoot(Openable& o)
			: Openable(o)
		{
		}
		static const int K_SOURCE = 1;
		static const int K_BINARY = 2;
		

		wstring DEFAULT_PACKAGEROOT_PATH = L""; //$NON-NLS-1$
	
		void attachSource(wstring& archivePath, wstring& rootPath, IProgressMonitor* monitor) ;

		IPackageFragment* createPackageFragment(
			wstring& name,
			bool force,
			IProgressMonitor* monitor);
		
	   virtual	int getKind() = 0;

		virtual IPackageFragment* getPackageFragment(const wstring& packageName) = 0;

	
		virtual wstring getSourceAttachmentPath() =0;

		virtual wstring getSourceAttachmentRootPath() = 0;

		virtual bool isArchive() = 0;

		 virtual bool isExternal() = 0;



		virtual  IPackageFragment* FindOrInsertPackageFragment(const wstring& compound_name) = 0;
		virtual void ProcessRemoveElement(map<wstring, NameLookUpNotify*>&) = 0;
		virtual void ProcessRemoveElement(const wstring&) = 0;

		virtual void SynchronousProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >&,
			IPackageFragmentRoot* clone) = 0;
		virtual void IncrementUpdateProcess(map<ResourceOptKind,
			map<wstring, NameLookUpNotify*> >&) = 0;

		virtual void addChild(const wstring& compoundName, IPackageFragment* child) = 0;
	
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

