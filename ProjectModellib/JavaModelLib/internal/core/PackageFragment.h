
#ifndef PackageFragment_java_model_INCLUDED_INCLUDED
#define PackageFragment_java_model_INCLUDED_INCLUDED
#include "IPackageFragment.h"
#include "Openable.h"
#include <map>
#include "IJavaElementType.h"

using std::map;

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class JType;
	class PackageFragmentRoot;
	class BaseJavaFile;

	class PackageFragment :
		  public IPackageFragment, public IJavaCardDocType
	{
	public:
		bool IsZip() {
			return fLEType == JAR_PACKAGE_FRAGMENT ;
		}
		IJavaElement* DeepCopy(IJavaElement*) override;
		wstring getPath() override;
		wstring getFullName() override
		{
			return L"Package " + fName;
		}
		std::vector<IJavaElement*>* getChildrenOfType(IJavaElementType _type) override;
		ICompilationUnit* FindOrInsertCompilationUnit(const wstring& name) override;
		IClassFile* FindOrInsertClassFile(const wstring& name) override;
		bool hasChildren() override;
		void CopyChildren(IPackageFragment* parent) override;
		void removeInfo() override;
		
		void ProcessRemoveElement(map<wstring, NameLookUpNotify*>&) override;
		void SynchronousProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*> >&, IPackageFragment*) override;
		void IncrementUpdateProcess(map<ResourceOptKind, map<wstring, NameLookUpNotify*>>&) override;

		IJavaElement* ShallowCopy(IJavaElement*) override;


		wstring getCompoundName() override;
		void addChild(IJavaElement* child) override;
		bool generateInfos(IProgressMonitor* pm) override;
		
		std::vector<IJavaElement*>* getChildren() override;
		bool CopyChildFromOriginalWhenIsClone() ;


		PackageFragment(const wstring& name_, 
			IJavaElement* owner_, 
			IJavaElementType _type = IJavaElementType::PACKAGE_FRAGMENT);
	
		PackageFragment(PackageFragment&);
		virtual ~PackageFragment();

		virtual void computeChildren();


		virtual		IClassFile* getClassFile(const wstring& name) override;

		virtual		std::vector<IClassFile*> getClassFiles() override;

		virtual		ICompilationUnit*
			getCompilationUnit(const wstring& name) override;

		virtual		std::vector<ICompilationUnit*> 
			getCompilationUnits()  override;

		virtual		int getKind() override;

		virtual		bool hasSubpackages() override;

		virtual		bool isDefaultPackage() override;
		const map<wstring, ICompilationUnit*>* getInternalCompilationUnits();
		const map<wstring, IClassFile*>* getInternalClassFiles();
		map<wstring, IClassFile*>* class_files;
		map<wstring, ICompilationUnit*>* compilations;

		map<wstring, IClassFile*>* innerClassesFiles;
	protected:
		void HelpToDeepCopy(PackageFragment& o);

	private:
		enum
		{
			DEPRECATED = 0x01
		};
		void computeClassChildren();
	public:
		PathModel::MosesPath* getMosesPath() override;
		std::wstring getJavaCardDocUrlFragment() override;
	};

	inline const map<wstring, ICompilationUnit*>* PackageFragment::getInternalCompilationUnits()
	{
		LazyLoad();
		return compilations;
	};

	inline const map<wstring, IClassFile*>* PackageFragment::getInternalClassFiles()
	{
		LazyLoad();
		return class_files;
	};


}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

