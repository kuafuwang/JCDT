#ifndef ContainersCtrl_jikes_PathModel_INCLUDED_INCLUDED
#define ContainersCtrl_jikes_PathModel_INCLUDED_INCLUDED

#include <map>

#include <string>
#include <boost/filesystem/path.hpp>
#include <set>
using std::wstring;

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		class IClasspathContainer;
		class ContainersCtrl
		{
		public:
			IClasspathContainer* getContainer(int kind, const wstring& name) const ;
			IClasspathContainer* getContainerByPath(int kind, const boost::filesystem::path& _path) const;

			ContainersCtrl();
			~ContainersCtrl();
			ContainersCtrl(const ContainersCtrl&) = delete;
			void getAllJCREContainer(std::vector<IClasspathContainer*>&) const;
			void getAllNONJCREContainer(std::vector<IClasspathContainer*>&) const;
			void getAllJCREContainerNames(std::vector<wstring>&) const;
			void AddContainer(IClasspathContainer* pContainer);

			const std::map<int, std::map<std::wstring, IClasspathContainer*> >& getAllContainers();
		private:
			std::map<int, std::map<std::wstring, IClasspathContainer*> > containers;
		};

		inline const std::map<int, std::map<std::wstring, IClasspathContainer*> >& ContainersCtrl::getAllContainers()
		{
			return containers;
		}


	}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED

