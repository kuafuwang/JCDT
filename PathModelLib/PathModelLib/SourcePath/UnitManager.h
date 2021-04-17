
#ifndef UnitManager_jikes_PathModel_INCLUDED_INCLUDED
#define UnitManager_jikes_PathModel_INCLUDED_INCLUDED
#include <boost/unordered/unordered_map.hpp>
#include <windows.h>
#include <string>
#include <set>
using std::wstring;
namespace Jikes { // Open namespace Jikes block


namespace PathModel{
	class CompilationUnitTypInfo;

	class UnitManager
	{
	public:
		UnitManager();
		~UnitManager();
		void beginToOccupy();
		void endToOccupy();
		boost::unordered_map<std::wstring, boost::unordered_map<wstring, CompilationUnitTypInfo* >> units;
		
		CRITICAL_SECTION* usage_reference;// 用来更新源的锁
	
		bool removeProject(const wstring& _path);
		void clear();
		
	};
	inline void UnitManager::beginToOccupy()
	{
		EnterCriticalSection(usage_reference);
	}

	inline  void UnitManager::endToOccupy()
	{
		LeaveCriticalSection(usage_reference);
	}
}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

