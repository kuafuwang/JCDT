#include "stdafx.h"
#include "ContainersCtrl.h"
#include <assert.h>
#include <boost/filesystem/path.hpp>
#include "ClasspathContainer.h"
#include <map>
#include <PathModelLib/Util/PathUtil.h>

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {


		void ContainersCtrl::getAllJCREContainerNames(std::vector<wstring>& names)const
		{
			for (auto it : containers)
			{
				auto kind = it.first;
				if (kind == IClasspathContainer::K_DEFAULT_SYSTEM_JCRE || kind == IClasspathContainer::K_SYSTEM_JCRE)
				{
					for (auto mapIt : it.second)
					{
						names.push_back(mapIt.first);
					}
				}
			}
		}
		void ContainersCtrl::getAllJCREContainer(std::vector<IClasspathContainer*>& collect)const
		{
			for (auto it : containers)
			{
				auto kind = it.first;
				if (kind == IClasspathContainer::K_DEFAULT_SYSTEM_JCRE || kind == IClasspathContainer::K_SYSTEM_JCRE)
				{
					for (auto mapIt : it.second)
					{
						collect.push_back(mapIt.second);
					}
				}
			}
		}
		void ContainersCtrl::getAllNONJCREContainer(std::vector<IClasspathContainer*>& collect)const
		{
			for (auto it : containers)
			{
				auto kind = it.first;
				if (kind == IClasspathContainer::K_DEFAULT_SYSTEM_JCRE || kind == IClasspathContainer::K_SYSTEM_JCRE)
				{
					continue;
				}
				for (auto mapIt : it.second)
				{
					collect.push_back(mapIt.second);
				}
			}
		}


		ContainersCtrl::ContainersCtrl()
		{

		}
		ContainersCtrl::~ContainersCtrl()
		{
			for (auto kind_it : containers) {
				for (auto it : kind_it.second) {
					delete it.second;
				}
			}
		}

		IClasspathContainer* ContainersCtrl::getContainer(int kind, const wstring& name)const
		{
			auto find_ret = containers.find(kind);
			if (find_ret != containers.end())
			{
				std::map<std::wstring, IClasspathContainer*>::const_iterator it = find_ret->second.find(name);
				for (; it != find_ret->second.end(); ++it)
				{
					IClasspathContainer* p = it->second;
					if (p->getName() == name && p->getKind() == kind)
					{
						return p;
					}
				}

			}
			return nullptr;
		}

		IClasspathContainer* ContainersCtrl::getContainerByPath(int kind, const boost::filesystem::path& _path)const
		{

			auto find_ret = containers.find(kind);

			if (find_ret != containers.end())
			{
				std::map<std::wstring, IClasspathContainer*>::const_iterator it = find_ret->second.begin();
				for (; it != find_ret->second.end(); ++it)
				{
					if (it->second->getPath() == _path && it->second->getKind() == kind)
						return it->second;
				}

			}

			return nullptr;
		}



		void ContainersCtrl::AddContainer(IClasspathContainer* pContainer)
		{
			if (pContainer == nullptr) {
				assert(false);
				return;
			}
			IClasspathContainer* p = getContainer(pContainer->getKind(), pContainer->getName());
			delete p;
			std::map<int, std::map<std::wstring, IClasspathContainer*> >& all_containers = containers;
			std::map<std::wstring, IClasspathContainer*>& temp = all_containers[pContainer->getKind()];
			// temp.insert(make_pair(pContainer->getName(), pContainer));
			temp[pContainer->getName()] = pContainer;
		}


	}// Close namespace PathModel block


} // Close namespace Jikes block
