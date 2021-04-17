
#ifndef JarPackageFragment_java_model_INCLUDED_INCLUDED
#define JarPackageFragment_java_model_INCLUDED_INCLUDED
#include "PackageFragment.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class JarPackageFragment:public PackageFragment
	{
	public:
	

		IJavaElement* ShallowCopy(IJavaElement*) override;
		JarPackageFragment(const wstring& compound_name, IJavaElement* owner_);

		JarPackageFragment(JarPackageFragment&o):PackageFragment(o){	
		}

		virtual void computeChildren() override;;
		


	};

	

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

