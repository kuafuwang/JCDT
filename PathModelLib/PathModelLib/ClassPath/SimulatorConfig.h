#pragma once
#include <string>
#include <vector>
using namespace std;

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		struct SimulatorItem
		{
			wstring name;
			wstring filename;
			wstring desc;
			bool t1_supported;
			bool t0_supported;
			bool ta_supported;
			bool show_in_cmd;
			//SimulatorItem() {}
		explicit SimulatorItem(const wstring& name_, const wstring& filename_, const wstring& desc_,
				bool t0_supported_, bool t1_supported_, bool ta_supported_, bool show_in_cmd_) :
				name(name_), filename(filename_), desc(desc_), t1_supported(t1_supported_),
				t0_supported(t0_supported_), ta_supported(ta_supported_), show_in_cmd(show_in_cmd_)
			{

			}
		};
		class SimulatorConfig
		{
		public:
			SimulatorConfig();
			~SimulatorConfig();
			void Load(const wstring& filename);
		//	bool Save();
		/*	bool AddItem(const SimulatorItem& item);
			void RemoveItem(const wstring item_name);*/

			vector< SimulatorItem >& GetSimulators()
			{
				return _simulators;
			}

			bool IsSimulator(wstring name);
			SimulatorItem* GetSimulatorItem(const wstring& name);
			wstring  dir_path;
		private:
			
			wstring _config_file_name;
			vector< SimulatorItem >  _simulators;
			vector<string> readers;

		};

	}
}