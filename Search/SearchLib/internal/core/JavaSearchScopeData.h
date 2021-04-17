#ifndef  JavaSearchScopeData_JIKES_SERACH_INCLUDE
#define JavaSearchScopeData_JIKES_SERACH_INCLUDE

#include <string>
#include <vector>

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class PathInfo
	{
	public:
		std::wstring name;
		std::wstring path;
		bool with_sub_dir;
		PathInfo(const std::wstring& _name,
			const std::wstring& _path,
			bool _with_sub_dir);
	};

	class  GroupInfo
	{
	public:
		GroupInfo(const std::wstring& _name);

		~GroupInfo();
		std::wstring groupName;
		std::vector<  Search::PathInfo* > items;
		void* groupData;
	};

}// Close namespace Search block
	

} // Close namespace Jikes block

#endif