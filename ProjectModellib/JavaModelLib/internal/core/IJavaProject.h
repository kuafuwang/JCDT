
#ifndef IJavaProject_java_model_INCLUDED_INCLUDED
#define IJavaProject_java_model_INCLUDED_INCLUDED

#include "Openable.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IPackageFragment;
	class IPackageFragmentRoot;

	class IJavaProject:   public Openable
	{
		

	public:
		IJavaProject(IJavaElement* parent, const wstring& name)
			: Openable(parent, IJavaElementType::JAVA_PROJECT, name)
		{
		}

		IJavaProject(Openable& o)
			: Openable(o)
		{
		}
		virtual void  beginToOccupy() = 0;
		virtual void  endToOccupy() = 0;
	  virtual IPackageFragmentRoot* findPackageFragmentRootByName(
		  const std::wstring& simpleName) = 0;

	  virtual IPackageFragment* findPackageFragmentByName(
		  const std::wstring& simpleRootName,
		  const std::wstring& pkgName ) = 0;

	  

	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

