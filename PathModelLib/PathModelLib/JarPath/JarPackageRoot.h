
#ifndef JarPackageRoot_jikes_PathModel_INCLUDED_INCLUDED
#define JarPackageRoot_jikes_PathModel_INCLUDED_INCLUDED
#include "IOpenable.h"
#include <string>
#include <map>
#include <vector>
#include <windows.h>
namespace Jikes { // Open namespace Jikes block
	class Zip;

namespace PathModel{
	class JarPathManager;
	class ClassFile;
	class JarPackage;

	class JarPackageRoot  :public IOpenable
	{
	public:
		void buildStructure(IProgressMonitor* monitor) override;;
	
	public:
		void beginToOccupy();
		void endToOccupy();
		JarPackageRoot(const JarPathManager*,const std::wstring&);
		~JarPackageRoot();
		void computeChildren();
		Zip* Zipfile();
		Zip* zipfile;
	
		std::map<std::wstring, JarPackage* >& getChildren()
		{
			LazyLoad();
			return children;
		}
		JarPackage* FindOrInsertPackageFragment(const std::wstring&);

	private:
		const JarPathManager* mgr;
		std::wstring resoucePath;
		CRITICAL_SECTION* usage_reference;// 用来更新源的锁
		std::map<std::wstring, JarPackage* > children;
	};

}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

