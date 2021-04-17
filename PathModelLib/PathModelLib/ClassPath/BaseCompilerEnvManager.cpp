#include "StdAfx.h"
#include "Utillib/XML/Markup.h"
#include "BaseCompilerEnvManager.h"
#include <assert.h>
#include <boost/filesystem/path.hpp>
#include <PathModelLib/ClassPath/ClasspathContainer.h>
#include <PathModelLib/Util/PathUtil.h>
#include <PathModelLib/JarPath/JarPathManager.h>
#include <PathModelLib/SourcePath/UnitManager.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/algorithm/string.hpp>
#include "IConverterContainer.h"
#include "ConverterContainer.h"
#include "Utillib/common/Util.h"
#include "SimulatorConfig.h"

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		const wchar_t* libConfigName = L"lib.xml";
		const wchar_t* simuConfigName = L"simulator.xml";
		const wchar_t* PACK_DIR_NAME = L"ChipPack";
		const wchar_t* START_PAGE_NAME = L"startpage";

		BaseCompilerEnvManager::BaseCompilerEnvManager(void):
		m_strSource(szJDK_1_5),
		m_strTarget(szJDK_1_5),
		m_SupportInt(true)
		{
			 jar_manager_ = new JarPathManager();
		}


		PackDetail::~PackDetail()
		{
			delete m_simulator_config;
		}

		BaseCompilerEnvManager::~BaseCompilerEnvManager(void)
		{
			if (jar_manager_)
				delete jar_manager_;
			for(auto it :m_ChipPackDatas)
			{
				delete it.second;
			}

		}
		const ContainersCtrl& BaseCompilerEnvManager::getContainerCtrl(const std::wstring& pack)
		{
			 auto findIt = m_ChipPackDatas.find(pack);
			if(m_ChipPackDatas.end() != findIt)
			{
				return (findIt->second->m_containerCtrl);
			}
			return m_EMPTY_CTRL;
		}
		ContainersCtrl* BaseCompilerEnvManager::InternalGetContainerCtrl(const std::wstring& pack)
		{
			auto findIt = m_ChipPackDatas.find(pack);
			if (m_ChipPackDatas.end() != findIt)
			{
				return &(findIt->second->m_containerCtrl);
			}
			return nullptr;
		}
		void BaseCompilerEnvManager::Load(const wstring& strFileName){

			m_AppDir = PathUtil::GetAppDir();
			if (LoadCofig(strFileName) )
			{
				if (
					   szJDK_1_5 != m_strSource
					&& szJDK_1_6 != m_strSource
					&& szJDK_1_7 != m_strSource
					&& szJDK_1_8 != m_strSource
					)
				{
					m_strSource = szJDK_1_5;
				}

				if (
					   szJDK_1_5 != m_strTarget
					&& szJDK_1_6 != m_strTarget
					&& szJDK_1_7 != m_strTarget
					&& szJDK_1_8 != m_strTarget
					)
				{
					m_strTarget =szJDK_1_5;
				}
			}
			else{

		
				m_strTarget =szJDK_1_5;
				m_strSource = szJDK_1_5;
				m_SupportInt = true;
			
			}

	

			if(!getConverter(IConverterContainer::K_ORACALE_CONVERTER, szConverter_jc222))
			{
				path _root(m_AppDir);
				_root.append(L"builder\\converter\\jc222");
				m_converter_containers.emplace_back(new DefaultConverterContainer( _root.wstring(), szConverter_jc222, szConverter_jc222, szDefaultConveter_2_x_x_Paramer));
			}
			if (!getConverter(IConverterContainer::K_ORACALE_CONVERTER, szConverter_jc305))
			{
				path _root(m_AppDir);
				_root.append(L"builder\\converter\\jc305");
				m_converter_containers.emplace_back(new DefaultConverterContainer(_root.wstring(), szConverter_jc305, szConverter_jc305, szDefaultConveter_3_x_x_Paramer));
			}
			if (!getConverter(IConverterContainer::K_FTSAFE_CONVERTER, szConverter_ft222))
			{
				path _root(m_AppDir);
				_root.append(L"builder\\converter\\ft222");
				m_converter_containers.emplace_back(new FTConverterContainer(_root.wstring(), szConverter_ft222, szConverter_ft222, szDefaultConveter_2_x_x_Paramer));
			}
			if (!getConverter(IConverterContainer::K_FTSAFE_CONVERTER, szConverter_ft305))
			{
				path _root(m_AppDir);
				_root.append(L"builder\\converter\\ft305");
				m_converter_containers.emplace_back(new FTConverterContainer(_root.wstring(), szConverter_ft305, szConverter_ft305, szDefaultConveter_3_x_x_Paramer));
			}

			for(auto packIt : m_ChipPackDatas)
			{
				if(!packIt.second)continue;
				auto& pack = packIt.second->m_containerCtrl;
				
				for (auto it : pack.getAllContainers())
				{
					for (auto second_it : it.second)
					{
						auto contain_it = second_it.second;
						if (!contain_it)
							continue;

						auto libs = contain_it->getClasspathEntries();
						for (auto lib : libs)
						{
							if (!lib)
								continue;

							if (lib->getEntryKind() == IClasspathEntry::CPE_LIBRARY)
							{
								jar_manager_->addJarPackageRootReference(
									reinterpret_cast<LibraryClasspathEntry*>(lib)->getAbolutePath().wstring());
							}
						}
					}
				}

			}

		}



		bool BaseCompilerEnvManager::LoadCofig(const wstring& strFileName){

			CMarkup xml;

			if (! xml.Load(strFileName.c_str()) ){
				return  false;
			}

			if (!xml.FindElem(_T("Global_Env_Config"))) {
				return false;
			}

			xml.IntoElem();
			if (xml.FindElem(_T("Javac_Option"))){

				xml.IntoElem();

				
				if (xml.FindElem(_T("_Source"))) {
					m_strSource =  xml.GetData().GetString();
				}

				if (xml.FindElem(_T("_Target"))) {
					m_strTarget =  xml.GetData().GetString();
				}

				xml.OutOfElem();
			}

			if (xml.FindElem(_T("JC_Option"))){
				xml.IntoElem();
				if (xml.FindElem(_T("SupportInt"))) {
					if(_T("1") ==  xml.GetData()){
						m_SupportInt = true;
					}
					else{
						m_SupportInt = false;
					}
				}
				xml.OutOfElem();
			}

			if (xml.FindElem(_T("Converters"))) 
			{

				xml.IntoElem();

				wstring app_dir = PathUtil::GetAppDir();
				boost::replace_all(app_dir, L"\\", L"/");

				if (xml.FindElem(_T("Items"))) {
					xml.IntoElem();
					while (xml.FindElem(_T("item")))
					{

						boost::filesystem::path _path = xml.GetAttrib(_T("path")).GetString();
						boost::system::error_code ec;
						if (boost::filesystem::is_regular_file(_path, ec))
						{
							continue;
						}


						boost::filesystem::path tempPath(app_dir);

						tempPath.append(boost::replace_all_copy(_path.wstring(), L"\\", L"/"));

						if (boost::filesystem::is_directory(tempPath, ec))
						{
							_path = tempPath;
						}



						std::wstring name = xml.GetAttrib(_T("name")).GetString();
						std::wstring kindName = xml.GetAttrib(_T("kind")).GetString();

						auto kind = IConverterContainer::findKind(kindName);
						IConverterContainer* pContainer = nullptr;
						switch (kind)
						{

							case IConverterContainer::K_ORACALE_CONVERTER:
							case IConverterContainer::K_JCOP_CONVERTER:
							case IConverterContainer::K_FTSAFE_CONVERTER:
							{

								std::wstring strV = xml.GetAttrib(_T("version")).GetString();

							
								std::wstring paramter = xml.GetAttrib(_T("parameter")).GetString();



								if (kind == IConverterContainer::K_ORACALE_CONVERTER)
								{
									pContainer = new DefaultConverterContainer(_path, name, strV, paramter);
								}
								else if(kind == IConverterContainer::K_JCOP_CONVERTER)
								{
									pContainer = new JcopConverterContainer(_path, name, strV, paramter);
								}
								else
								{
									pContainer = new FTConverterContainer(_path, name, strV, paramter);
								}
							}
							break;
							default:
								break;
						}
						if (pContainer)
							m_converter_containers.emplace_back(pContainer);
					}
					xml.OutOfElem();
				}

				xml.OutOfElem();
			}
		
			ProcessPackSimulators();
			
			return true;
		}

		void BaseCompilerEnvManager::SaveCofig(const wstring& strFileName){
			CMarkup xml;
			xml.AddElem(_T("Global_Env_Config"));
			xml.IntoElem();
			{
				xml.AddElem(_T("Javac_Option"));
				xml.IntoElem();
			
				xml.AddElem(_T("_Source"),m_strSource.c_str());
				xml.AddElem(_T("_Target"),m_strTarget.c_str());
				xml.OutOfElem();
			}
			{
				xml.AddElem(_T("JC_Option"));
				xml.IntoElem();	
				if(m_SupportInt)
					xml.AddElem(_T("SupportInt"),_T("1"));
				else
					xml.AddElem(_T("SupportInt"),_T("0"));
				xml.OutOfElem();
			}
			{
				xml.AddElem(_T("Converters"));
				xml.IntoElem();
				{
					wstring app_dir = PathUtil::GetAppDir();
					boost::replace_all(app_dir, L"\\", L"/");
					xml.AddElem(_T("Items"));
					xml.IntoElem();
				
					for (auto pContainer : m_converter_containers)
					{
						if (!pContainer)
							continue;

						auto _kind = pContainer->getKind();
						wstring _path = pContainer->getPath().wstring();

						boost::replace_all(_path, L"\\", L"/");

						if (_path.find(app_dir) != wstring::npos)
						{
							_path = PathUtil::relativePath(_path, app_dir).wstring();
						}

						xml.AddElem(_T("item"));
						xml.AddAttrib(_T("name"), pContainer->getName().c_str());
						xml.AddAttrib(_T("path"), _path.c_str());
						auto kindName = IConverterContainer::getKindName(pContainer->getKind());
						xml.AddAttrib(_T("kind"), kindName.c_str());

						switch (_kind)
						{

						case IConverterContainer::K_ORACALE_CONVERTER:
						case	IConverterContainer::K_JCOP_CONVERTER:
						{
							DefaultConverterContainer* pJcre = dynamic_cast<DefaultConverterContainer*>(pContainer);
							if (!pJcre)
								continue;

							xml.AddAttrib(L"version", pJcre->GetVersion().c_str());
							xml.AddAttrib(L"parameter", pJcre->Parameter().c_str());

						}
						break;


						default:
							break;
						}

						
					}
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
			if (!xml.Save(strFileName.c_str())){	
			}

		}

		IConverterContainer* BaseCompilerEnvManager::getConverter(int kind, const wstring& name)
		{
		
			for(auto it : m_converter_containers)
			{
				if(it->getKind() == kind && it->getName() == name)
				{
					return it;
				}
			}
			return nullptr;
		}

	

		IClasspathContainer* BaseCompilerEnvManager::getContainer(const std::wstring& packName, int kind, const wstring& name)
		{
			auto& _containerCtrl =getContainerCtrl(packName);
			return _containerCtrl.getContainer(kind, name);

		}

		IClasspathContainer* BaseCompilerEnvManager::getContainerByPath(int kind, const boost::filesystem::path& _path)
		{
			//auto _containerCtrl = getContainerCtrl(packName);
		//	if (!_containerCtrl) return nullptr;
			 for(auto& it :m_ChipPackDatas)
			 {
				 if (!it.second) continue;

				 auto ret = it.second->m_containerCtrl.getContainerByPath(kind, _path);
				 if (ret) return ret;
			 }
			return nullptr;
		}

		IClasspathContainer* BaseCompilerEnvManager::FindOrInsetJCREContainer(const std::wstring& packName,const boost::filesystem::path&  _path,
			const wstring& attach_path )
		{

			IClasspathContainer* p = getContainerByPath(IClasspathContainer::K_SYSTEM_JCRE, _path);
			if (p == nullptr) 
			{
				Version _v = detectSDKVersion(_path);
				p = new JCREContainer(_path, _path.leaf().wstring(), attach_path, _v);
				AddContainer(packName,p);
				auto libs = p->getClasspathEntries();
				for (auto lib : libs)
				{
					if (lib->getEntryKind() == IClasspathEntry::CPE_LIBRARY)
					{
						jar_manager_->addJarPackageRootReference(
							reinterpret_cast<LibraryClasspathEntry*>(lib)->getAbolutePath().wstring());
					}
				}
			}
			return p;

			
		}

		bool BaseCompilerEnvManager::AddContainer(const std::wstring& packName, IClasspathContainer* pContainer)
		{
			auto findIt = m_ChipPackDatas.find(packName);
			if (m_ChipPackDatas.end() != findIt)
			{
				 (findIt->second->m_containerCtrl).AddContainer(pContainer);
				 return  true;
			}
			
			auto detail = new PackDetail();
			m_ChipPackDatas.insert({ packName, detail });
			auto _containerCtrl = &detail->m_containerCtrl;		
			_containerCtrl->AddContainer(pContainer);
			
			return  true;
		}

		bool BaseCompilerEnvManager::RecordRemoveJarOrClassDire(const boost::filesystem::path& abolust_path)
		{
			return jar_manager_->removeJarPackageRootReference(abolust_path.wstring());
		}

		bool BaseCompilerEnvManager::RecordAddJarOrClassDire(const boost::filesystem::path& abolust_path)
		{
			if(abolust_path.extension().wstring() == L".jar")
			{
				 jar_manager_->addJarPackageRootReference(abolust_path.wstring());
				 return true;
			}
			return false;
		}

		void BaseCompilerEnvManager::Clear()
		{
			if (jar_manager_)
			{
				delete jar_manager_;
				jar_manager_ = nullptr;
			}
				
		}

		wstring BaseCompilerEnvManager::GetJavacExePath()
		{
			
			return 	m_AppDir + L"builder\\JDK\\bin\\javac.exe";
		}

		wstring BaseCompilerEnvManager::GetJavaExePath()
		{
			
			return 	m_AppDir + L"builder\\JDK\\bin\\java.exe";
		}

		wstring BaseCompilerEnvManager::GetJavapExePath()
		{
			
			return 	m_AppDir + L"builder\\JDK\\bin\\javap.exe";
		}

		wstring BaseCompilerEnvManager::GetProguardDirPath()
		{
			return 	m_AppDir + L"builder\\proguard\\";
		}
		wstring BaseCompilerEnvManager::GetECJPath()
		{
			return 	m_AppDir + L"builder\\ECJ\\ecj.jar";
		}

		wstring BaseCompilerEnvManager::GetJvmDllPath()
		{
			return 	m_AppDir + L"builder\\jre7\\bin\\client\\jvm.dll";
		}

		wstring BaseCompilerEnvManager::GetJDT_ToolDllPath()
		{
			return 	m_AppDir + L"builder\\JDT_Tool.jar";
		}

		void BaseCompilerEnvManager::ProcessPackLibs()
		{
			using namespace boost::filesystem;
			auto tempdir = path(Util::FormatPath(m_AppDir,Util::remove_last_slash_when_format_path));
			tempdir = tempdir.parent_path();
			tempdir.append(PACK_DIR_NAME);
			if (!exists(tempdir))
			{
				return;
			}
			directory_iterator end;
			map<wstring, path> libconfigFiles;

			for (directory_iterator pos(tempdir); pos != end; ++pos)
			{
				if (!is_directory(*pos))
				{
					continue;
				}

				path file(*pos);
				auto packName = file.filename().wstring();
				
				if (!exists(path(file).append(L"lib")) || !exists(path(file).append(libConfigName)))
				{
					continue;
				}
				
				libconfigFiles.insert(std::make_pair(packName,file));
			}
		    for(auto it : libconfigFiles)
		    {
				CMarkup xml;
				auto file = it.second;
				if(!xml.Load(path(file).append(libConfigName).c_str()))
				{
					continue;
				}
		    	
				if (!xml.FindElem(_T("Containers")))
					continue;
				

				xml.IntoElem();

				auto libDir = file;
		    	
				if (!xml.FindElem(_T("Items")))
				{
					continue;
				}
				auto packName = it.first;
		
			
				xml.IntoElem();
				while (xml.FindElem(_T("item")))
				{

					boost::filesystem::path _path = xml.GetAttrib(_T("path")).GetString();
					boost::system::error_code ec;
					if (boost::filesystem::is_regular_file(_path, ec))
					{
						continue;
					}


					boost::filesystem::path tempPath = (libDir);

					tempPath.append(boost::replace_all_copy(_path.wstring(), L"\\", L"/"));

					if (boost::filesystem::is_directory(tempPath, ec))
					{
						_path = tempPath;
					}

					bool bRecommend;
					std::wstringex recommend = xml.GetAttrib(_T("recommend")).GetString();
					recommend.toupper();
					if(recommend == L"Y")
					{
						bRecommend = true;
					}
					else
					{
						bRecommend = false;
					}
					std::wstring name = xml.GetAttrib(_T("name")).GetString();
					std::wstring kindName = xml.GetAttrib(_T("kind")).GetString();

					auto kind = IClasspathContainer::findKind(kindName);
					IClasspathContainer* pContainer = nullptr;
					switch (kind)
					{

					case IClasspathContainer::K_DEFAULT_SYSTEM_JCRE:
					case IClasspathContainer::K_SYSTEM_JCRE:
					{

						std::wstring strV = xml.GetAttrib(_T("version")).GetString();

						Version _v = StringToVersion(strV);
						if (_v == JCRE_NONE)
						{
							_v = detectSDKVersion(_path);
						}

						if (kind == IClasspathContainer::K_DEFAULT_SYSTEM_JCRE)
						{
							pContainer = new DefaultJCREContainer(_path, name, _v);
						}
						else
						{
							std::wstring attach_source = xml.GetAttrib(_T("attach_source")).GetString();

							if (!boost::filesystem::exists(attach_source, ec))
							{
								tempPath =(libDir);
								tempPath.append(boost::replace_all_copy(attach_source, L"/", L"\\"));
								if (boost::filesystem::exists(tempPath, ec))
								{
									attach_source = tempPath.wstring();
								}
							}
							pContainer = new JCREContainer(_path, name, attach_source, _v);
							
						}
					}

					break;
					case IClasspathContainer::K_DEFAULT_GP:
						pContainer = new GPContainer(name, _path.wstring());
						break;
					case IClasspathContainer::K_CHINA_SHANGMI:
						pContainer = new SMContainer(name, _path.wstring());
						break;
					case IClasspathContainer::K_FOLDER:
						pContainer = new DefaultContainer(name, _path.wstring());
					default:
						break;
					}
					if (pContainer)
					{
						((DefaultContainer*)pContainer)->SetRecommend(bRecommend);
						AddContainer(packName, pContainer);
					}
						
				}
				xml.OutOfElem();
				xml.OutOfElem();
		    }
			
		}

		void BaseCompilerEnvManager::ProcessPackSimulators()
		{
			using namespace boost::filesystem;
			auto tempdir = path(Util::FormatPath(m_AppDir,Util::remove_last_slash_when_format_path));
			tempdir = tempdir.parent_path();
			tempdir.append(PACK_DIR_NAME);
			if (!exists(tempdir))
			{
				return;
			}
			directory_iterator end;
			map<wstring, path> simuConfigFiles;

			for (directory_iterator pos(tempdir); pos != end; ++pos)
			{
				if (!is_directory(*pos))
				{
					continue;
				}

				path file(*pos);
				auto packName = file.filename().wstring();
				auto  _detail = new PackDetail();
				m_ChipPackDatas[packName] = _detail;
				auto startePage = file;
				startePage.append(START_PAGE_NAME);
				_detail->m_startPagePath = startePage.wstring();

				_detail->m_HelpDocPath = path(file).append(L"doc/JavaCardAPI.chm").wstring();

				
				SimulatorConfig* config = new SimulatorConfig();
			
				config->dir_path = path(file).append(L"simulators").wstring();
				config->Load(path(file).append(simuConfigName).wstring());
				_detail->m_simulator_config = config;
				
			}
			ProcessPackLibs();


		}
		bool BaseCompilerEnvManager::GetSimulatorPath(const wstring& simulatorName,wstring& path)
		{
			std::wstringex temp(simulatorName);
			auto  names = temp.split(L"@");
			if(names.size() !=2)
				return false;
			auto  findIt = m_ChipPackDatas.find(names[1]);
			if (findIt == m_ChipPackDatas.end())
				return false;
			auto  m_strReader = names[0];
			auto pack_detail = findIt->second;
			
			if (!pack_detail || !pack_detail->m_simulator_config)
				return  false;
			
			vector< SimulatorItem >& simulators = pack_detail->m_simulator_config->GetSimulators();
			for (size_t i = 0; i < simulators.size(); ++i)
			{
				if (simulators[i].name != m_strReader)continue;
				
			    if (Util::IsExist(simulators[i].filename))
				{
					path = simulators[i].filename;
					return true;
				}
				return false;
			}
			return false;
		}

		bool BaseCompilerEnvManager::GetHelpDocPath(const wstring& packName, wstring& path)
		{
			auto  findIt = m_ChipPackDatas.find(packName);
			if (findIt == m_ChipPackDatas.end())
				return false;
		
			auto pack_detail = findIt->second;

			if (!pack_detail || !pack_detail->m_HelpDocPath.empty())
				return  false;

			if(exists(pack_detail->m_HelpDocPath))
			{
				path = pack_detail->m_HelpDocPath;
				return true;
			}
			else
			{
				return false;
			}
		}

		std::wstring BaseCompilerEnvManager::GetStartPagePath(const std::wstring& packName )
		{
			if(!packName.empty())
			{
				auto  findIt = m_ChipPackDatas.find(packName);
				if (findIt != m_ChipPackDatas.end())
				{
					if(findIt->second  && exists(findIt->second->m_startPagePath))
					{
						return findIt->second->m_startPagePath;
					}
				}

			}
			for(auto it : m_ChipPackDatas)
			{
				if (!it.second) continue;

				if(exists(it.second->m_startPagePath))
				{
					return it.second->m_startPagePath;
				}
			}
			return L"";
		}


		SimulatorItem* BaseCompilerEnvManager::GetSimulatorItem(const wstring& simulatorName)
		{
			std::wstringex temp(simulatorName);
			auto  names = temp.split(L"@");
			if (names.size() != 2)
				return nullptr;
			auto  findIt = m_ChipPackDatas.find(names[1]);
			if (findIt == m_ChipPackDatas.end() )
				return nullptr;
			auto  m_strReader = names[0];
			auto pack_detail = findIt->second;
			
			if (!pack_detail || !pack_detail->m_simulator_config)
				return  nullptr;
			
			vector< SimulatorItem >& simulators = pack_detail->m_simulator_config->GetSimulators();
			for (size_t i = 0; i < simulators.size(); ++i)
			{
				if (simulators[i].name != m_strReader)continue;

				return &simulators[i];
			}
			return nullptr;
		}

		void BaseCompilerEnvManager::GetAllPackNames(std::vector<std::wstring>& names)
		{
			names.reserve(4);
			for(auto it : m_ChipPackDatas)
			{
				names.emplace_back(it.first);
			}
		}

		void BaseCompilerEnvManager::GetAllSimulatorNames(const std::wstring& packName,std::vector<std::wstring>& names)
		{
		 
			for(auto it : m_ChipPackDatas)
			{
				if (!it.second) {
					assert(false);
					continue;
				}
				if(it.first != packName)
				{
					continue;
				}
				
				auto pack_detail = it.second;
				if (!pack_detail || !pack_detail->m_simulator_config)
				{
					continue;
				}
				vector< SimulatorItem >& simulators = pack_detail->m_simulator_config->GetSimulators();
				if (simulators.empty())
				{
					continue;
				}
				for (auto& simulator : simulators)
				{
					names.push_back(simulator.name  + L"@" +  it.first); // 
				}
			}
		}
	}// Close namespace PathModel block


} // Close namespace Jikes block




