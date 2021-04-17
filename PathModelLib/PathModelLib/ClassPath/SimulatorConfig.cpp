#include "stdafx.h"
#include "SimulatorConfig.h"

#include <Utillib/XML/Markup.h>
#include <Utillib/common/Util.h>
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		SimulatorConfig::SimulatorConfig()
		{
		}


		SimulatorConfig::~SimulatorConfig()
		{
		}
		void SimulatorConfig::Load(const wstring& filename)
		{
			_config_file_name = filename;

			//. º”‘ÿ∂œµ„
			CMarkup xml;
			string strTemp;
			_simulators.clear();
			if (!xml.Load(_config_file_name.c_str()))
			{
				return;
			}
			if (!xml.FindElem(_T("SimulatorInfo")))
			{
				return;
			}

			xml.IntoElem();

			int iVerson = 0;
			if (!xml.FindElem(_T("Version"))) {
				return;
			}


			if (!xml.FindElem(L"Simulators")) {
				return;
			}

			xml.IntoElem();
			while (xml.FindElem(_T("item"))) {
				wstring name = xml.GetAttrib(_T("name")).GetString();
				wstring filePath = xml.GetAttrib(_T("filename")).GetString();
				wstring desc = xml.GetAttrib(_T("desc")).GetString();

				bool t0 = false;
				if (xml.GetAttrib(_T("T0")) == L"Y")
				{
					t0 = true;
				}

				bool t1 = false;
				if (xml.GetAttrib(_T("T1")) == L"Y")
				{
					t1 = true;
				}

				bool ta = false;
				if (xml.GetAttrib(_T("TA")) == L"Y")
				{
					ta = true;
				}
				bool show_in_cmd = false;
				if (xml.GetAttrib(_T("show")) == L"Y")
				{
					show_in_cmd = true;
				}
				if(!Util::IsExist(filePath))
				{
					auto  temp  =Util::FormatPath(dir_path, Util::add_slash_to_last_segment_when_format_path);
					temp += filePath;
					if (Util::IsExist(temp))
					{
						filePath = temp;
					}
				}



				_simulators.push_back(SimulatorItem(name, filePath, desc, t0, t1, ta, show_in_cmd));
			}
			xml.OutOfElem();



			xml.OutOfElem();


		}
		/*bool SimulatorConfig::Save()
		{

			CMarkup xml;
			xml.AddElem(_T("SimulatorInfo"));

			xml.IntoElem();

			xml.AddElem(_T("Version"), 1);


			xml.AddElem(_T("Simulators"));
			xml.IntoElem();
			for (vector< SimulatorItem >::iterator it = _simulators.begin(); it != _simulators.end(); ++it) {

				xml.AddElem(L"item");
				xml.AddAttrib(_T("name"), it->name.c_str());
				xml.AddAttrib(_T("filename"), it->filename.c_str());
				xml.AddAttrib(_T("desc"), it->desc.c_str());
				if (it->t0_supported)
				{
					xml.AddAttrib(_T("T0"), L"Y");
				}
				else
				{
					xml.AddAttrib(_T("T0"), L"N");
				}
				if (it->t1_supported)
				{
					xml.AddAttrib(_T("T1"), L"Y");
				}
				else
				{
					xml.AddAttrib(_T("T1"), L"N");
				}
				if (it->ta_supported)
				{
					xml.AddAttrib(_T("TA"), L"Y");
				}
				else
				{
					xml.AddAttrib(_T("TA"), L"N");
				}
			}
			xml.OutOfElem();

			xml.OutOfElem();

			if (!xml.Save(_config_file_name.c_str()))
			{

				return false;
			}
			return true;

		}*/

		/*bool SimulatorConfig::AddItem(const SimulatorItem& item)
		{
			for (size_t i = 0; i < _simulators.size(); ++i)
			{
				if (item.name == _simulators[i].name)
				{
					return false;
				}
			}
			_simulators.push_back(item);
			Save();
			return true;
		}
		void SimulatorConfig::RemoveItem(const wstring item_name)
		{

			for (vector< SimulatorItem >::iterator it = _simulators.begin(); it != _simulators.end();)
			{
				if (item_name == it->name)
				{
					it = _simulators.erase(it);
				}
				else
				{
					++it;
				}
			}

			Save();

		}
*/

		bool SimulatorConfig::IsSimulator(wstring name)
		{
			for (size_t i = 0; i < _simulators.size(); ++i)
			{
				if (_simulators[i].name == name)
				{
					return true;
				}
			}

			return false;
		}
		SimulatorItem* SimulatorConfig::GetSimulatorItem(const wstring& name)
		{
			for (size_t i = 0; i < _simulators.size(); ++i)
			{
				if (_simulators[i].name == name)
				{
					return &_simulators[i];
				}
			}

			return nullptr;
		}
	}
}