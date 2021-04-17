#include "stdafx.h"
#include "UnitManager.h"
#include "CompilationUnitTypInfo.h"

namespace Jikes { // Open namespace Jikes block

	namespace PathModel {
		UnitManager::UnitManager()
		{
			 usage_reference = new CRITICAL_SECTION();
			 InitializeCriticalSectionAndSpinCount(usage_reference, 2000);
		}

		UnitManager::~UnitManager()
		{
			for (auto it : units)
			{
				auto& files = it.second;
				for (auto fileIt : files)
				{
					delete fileIt.second;
				}

			}
			if (usage_reference) {
				DeleteCriticalSection(usage_reference);
				delete usage_reference;
			}
		}
		

		bool UnitManager::removeProject(const wstring& projectName)
		{
			auto find__it = units.find(projectName);
			if(find__it != units.end())
			{
				auto& files = find__it->second;
				for(auto it : files)
				{
					delete it.second;
				}

				units.erase(find__it);
				return true;
			}
			return false;
		}

		void UnitManager::clear()
		{
			for(auto it : units)
			{
				auto& files = it.second;
				for (auto fileIt : files)
				{
					delete fileIt.second;
				}

			}
			units.clear();
		}

	} // Close namespace JavaModel block

} // Close namespace Jikes block

