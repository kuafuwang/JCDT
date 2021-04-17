
#ifndef JarPackageFragmentRoot_java_model_INCLUDED_INCLUDED
#define JarPackageFragmentRoot_java_model_INCLUDED_INCLUDED
#include "PackageFragmentRoot.h"


namespace Jikes { // Open namespace Jikes block

	class Zip;
	namespace PathModel
	{
		
		class JarPackageRoot;
	}
namespace JavaModel{
	
	class JarPackageFragmentRoot:public PackageFragmentRoot
	{
	public:

	
		
		virtual ~JarPackageFragmentRoot();

		IJavaElement* ShallowCopy(IJavaElement*) override;

		JarPackageFragmentRoot(const wstring& _path,IJavaProject* p, const wstring& name,const wstring& attach_path);
		JarPackageFragmentRoot(JarPackageFragmentRoot&o);
		void computeChildren() override;
		IPackageFragment* FindOrInsertPackageFragment(const wstring& compound_name) override;

		boost::shared_ptr<PathModel::JarPackageRoot> _jar_pkgs;
		
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

