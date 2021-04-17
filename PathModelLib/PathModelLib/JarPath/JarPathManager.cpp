#include "stdafx.h"
#include "JarPathManager.h"
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		boost::shared_ptr<JarPackageRoot> JarPathManager::NO_JARPACKAGEROOT;

		JarPathManager::~JarPathManager()
		{
			
				DeleteCriticalSection(cs_generate);
				delete cs_generate;
			
		}

		JarPathManager::JarPathManager()
		{
			cs_generate = new CRITICAL_SECTION();
			InitializeCriticalSectionAndSpinCount(cs_generate, 2000);

		}

		boost::shared_ptr<JarPackageRoot>& JarPathManager::getJarPackageRoot(const std::wstring& _path)
		{
			readLock (rw_mutex);
			auto find_it = childrens.find(_path);
			if (find_it != childrens.end())
			{
				return find_it->second;
			}
			return NO_JARPACKAGEROOT;
		}

		void JarPathManager::addJarPackageRootReference(const std::wstring& _path)
		{
			writeLock (rw_mutex);
			auto find_it = childrensReference.find(_path);
			if (find_it != childrensReference.end())
			{
				find_it->second += 1;
			}
			else
			{
				childrensReference.insert(std::make_pair(_path, 1));
				childrens.insert(std::make_pair(_path, boost::make_shared<JarPackageRoot>(this, _path)));
			}
		}

		int JarPathManager::removeJarPackageRootReference(const std::wstring& _path)
		{
			writeLock (rw_mutex);
			auto find_it = childrensReference.find(_path);
			if (find_it != childrensReference.end())
			{
				return 0;
			}

			if (find_it->second > 1)
			{
				find_it->second -= 1;
			}
			else
			{
				childrens.erase(_path);
			}
			return 0;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

