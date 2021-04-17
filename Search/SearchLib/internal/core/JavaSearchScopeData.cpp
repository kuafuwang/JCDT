#include "stdafx.h"
#include "JavaSearchScopeData.h"

using  namespace  std;
namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	PathInfo::PathInfo(const wstring& _name, const wstring& _path, bool _with_sub_dir)
		: name(_name), path(_path), with_sub_dir(_with_sub_dir)
	{
	}

	GroupInfo::GroupInfo(const wstring& _name) : groupName(_name), groupData(nullptr)
	{
	}

	GroupInfo::~GroupInfo()
	{
		auto size = items.size();
		auto data = items.data();
		for (auto i = 0; i < size; ++i)
		{
			delete data[i];
		}
	}

}


} // Close namespace Jikes block


