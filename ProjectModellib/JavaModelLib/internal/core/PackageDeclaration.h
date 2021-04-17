
#ifndef PackageDeclaration_java_model_INCLUDED_INCLUDED
#define PackageDeclaration_java_model_INCLUDED_INCLUDED

#include "IPackageDeclaration.h"
#include "IJavaElementType.h"
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class PackageDeclaration
		:public IPackageDeclaration 
	{
	public:
	
		PackageDeclaration( const wstring& name, IJavaElement* p)
			: IPackageDeclaration(p, name)
			
		{

		}
		PackageDeclaration(PackageDeclaration&o):IPackageDeclaration(o)
			{}
	
		IPackageDeclaration* ShallowCopy(IJavaElement* parent) override
		{
			auto clone = new PackageDeclaration(*this);
			clone->fParent = parent;
			return clone;
		};
		

	

	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

