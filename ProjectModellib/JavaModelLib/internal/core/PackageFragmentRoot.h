
#ifndef PackageFragmentRoot_java_model_INCLUDED_INCLUDED
#define PackageFragmentRoot_java_model_INCLUDED_INCLUDED
#include "IPackageFragmentRoot.h"
#include "Openable.h"
#include <map>
#include <boost/filesystem/path.hpp>
#include "IJavaElementType.h"
using std::map;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class PackageFragment;

	class PackageFragmentRoot
		: public IPackageFragmentRoot
	{
	public:

		bool equals(IJavaElement* o) override;
		IJavaElement* DeepCopy(IJavaElement*) override;
		wstring getPath() override;
		
		wstring getFullName() override
		{
			return L"IPackageFragmentRoot " + fName;
		}
		std::vector<IJavaElement*>* getChildrenOfType(IJavaElementType _type) override;


		int getKind() override;

		void addChild(const wstring& compoundName, IPackageFragment* child) override;

		bool hasChildren() override;

		std::vector<IJavaElement*>* getChildren() override;

		void removeInfo() override;

		IPackageFragment* getPackageFragment(const wstring& packageName) override;


		void ProcessRemoveElement(const wstring&) override;
		void ProcessRemoveElement(map<wstring, NameLookUpNotify*>&) override;

		void IncrementUpdateProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*>>&) override;

		void SynchronousProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*>>&,
			IPackageFragmentRoot* clone) override;

		IJavaElement* ShallowCopy(IJavaElement*) override;

		
		void addChild(IJavaElement* child) override;
		IPackageFragment* getPackageFragment(wstring& packageName) ;
		wstring getSourceAttachmentPath() override;
		wstring getSourceAttachmentRootPath() override;
		bool isArchive() override;
		bool isExternal() override;
		bool generateInfos(IProgressMonitor* pm) override;
		

		virtual void computeChildren();
		 void computeFolderChildren(const wstring& folder,const wstring& prefix);

		PackageFragmentRoot(
			const wstring& _path,
			int _root_type,
			IJavaProject* p, 
			const wstring& name,
			const wstring& attach_path,
			IJavaElementType _type = IJavaElementType::PACKAGE_FRAGMENT_ROOT);

		PackageFragmentRoot(PackageFragmentRoot&);
		  IPackageFragment* FindOrInsertPackageFragment(const wstring& compound_name)override;
		bool CopyChildFromOriginalWhenIsClone();

		virtual ~PackageFragmentRoot();

		const std::map<wstring, IPackageFragment*>& getInternalChildren();
		std::map<wstring, IPackageFragment*> children;
		boost::filesystem::path resoucePath;
		int root_kind ;

		wstring sourceAttachmentRootPath;

	protected:
		void HelpToDeepCopy(PackageFragmentRoot& o);
	public:
		PathModel::MosesPath* getMosesPath() override;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

