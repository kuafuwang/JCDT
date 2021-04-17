
#ifndef JavaProject_java_model_INCLUDED_INCLUDED
#define JavaProject_java_model_INCLUDED_INCLUDED
#include "IJavaProject.h"
#include "Openable.h"
#include <vector>
#include <boost/filesystem/path.hpp>
#include <set>
#include <map>
#include <windows.h>
namespace Jikes
{
	namespace PathModel
	{
		class IClasspathEntry;
		class ContainersCtrl;
		class ResouceOptStack;

	}
}
using std::map;

namespace Jikes { // Open namespace Jikes block


   namespace PathModel
   {
	   class JarPathManager;
   }

namespace JavaModel{
	class IJavaModel;

	class IPackageFragment;
	
	class JavaModel;
	class IPackageFragmentRoot;
	class BaseJavaFile;


	class BaseProject
		: public IJavaProject
	  
	{
	public:
		
        enum ModifyLevel
        {
	        PROJECT,
			PKG_ROOT,
			PKG
        };
		bool equals(IJavaElement* o) override;
		wstring getPath() override;

		wstring getFullName() override
		{
			return L"Project " + fName;
		}
		IJavaProject* getJavaProject() override{
			return this;
		};
		std::vector<IJavaElement*>* getChildren() override;
		bool hasChildren() override;
		void removeInfo() override;
		void calculateSourceEntry();
		void beginToOccupy() override;
		void endToOccupy() override;
		JavaModel* getJavaModel() override;
		IPackageFragment* findPackageFragmentByName(const std::wstring& simpleRootName, const std::wstring& pkgName) override;
		IPackageFragmentRoot* findPackageFragmentRootByName(const std::wstring& simpleName) override;

		virtual ~BaseProject();
		
		void addChild(IJavaElement* child) override;
	
		IPackageFragment* getPackageFrament(const wstring&, const wstring&);
		

		BaseProject(IJavaModel* p, const wstring& projectPath, const wstring& name);
		BaseProject(BaseProject&);
	
		const map<wstring, IPackageFragmentRoot* >& getInternalChildren();
		const map<wstring, IPackageFragmentRoot* >& getInternalSourcChildren();
		

		map<wstring, IPackageFragmentRoot*> all_children;
		map<wstring, IPackageFragmentRoot*> source_children;
	
		void HelpToDeepCopy(BaseProject&o);
		
	protected:
		wstring unix_style_project_path;
		CRITICAL_SECTION* usage_reference;// 用来更新源的锁
		PathModel::ResouceOptStack* opt_stack;
		CRITICAL_SECTION* opt_cs;// 
		
	};
	


	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

