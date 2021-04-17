
#ifndef JarPathManager_jikes_PathModel_INCLUDED_INCLUDED
#define JarPathManager_jikes_PathModel_INCLUDED_INCLUDED
#include <string>
#include <map>
#include <boost/thread.hpp>
#include "JarPackageRoot.h"

namespace Jikes { // Open namespace Jikes block


namespace PathModel{
	class JarPackageRoot;
	class JarPathManager
	{
	public:
		typedef boost::shared_lock<boost::shared_mutex> readLock;
		typedef boost::unique_lock<boost::shared_mutex> writeLock;
		static boost::shared_ptr<JarPackageRoot> NO_JARPACKAGEROOT;

		~JarPathManager();
		JarPathManager();
		boost::shared_ptr<JarPackageRoot>& getJarPackageRoot(const std::wstring& _path);

		void addJarPackageRootReference(const std::wstring& _path);

		int removeJarPackageRootReference(const std::wstring& _path);
		CRITICAL_SECTION* cs_generate;
	private:

		std::map<std::wstring, boost::shared_ptr<JarPackageRoot> >  childrens;
		std::map<std::wstring, int > childrensReference;
		boost::shared_mutex rw_mutex;
	};
}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

