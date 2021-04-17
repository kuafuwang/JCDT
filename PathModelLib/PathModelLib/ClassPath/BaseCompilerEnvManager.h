#ifndef BaseCompilerEnvManager_jikes_PathModel_INCLUDED_INCLUDED
#define BaseCompilerEnvManager_jikes_PathModel_INCLUDED_INCLUDED

#include <vector>
#include <PathModelLib/ClassPath/IClasspathContainer.h>
#include <PathModelLib/ClassPath/ContainersCtrl.h>

namespace boost{
	namespace filesystem{
	class path;
}
}

using std::vector;
using std::wstring;



namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		class JarPathManager;
		class UnitManager;
		class IConverterContainer;
		class IClasspathContainer;

		extern const wchar_t* PACK_DIR_NAME;
		class SimulatorConfig;
		struct SimulatorItem;
	class PackDetail
	{
	public:
		~PackDetail();
		std::wstring m_startPagePath;
		SimulatorConfig* m_simulator_config = nullptr;
		std::wstring m_HelpDocPath;
		ContainersCtrl m_containerCtrl;
	};
class BaseCompilerEnvManager
{

public:


	enum{
		UPDATE_LOCAL_JAVA_PATH = 125,
		UPDATE_LOCAL_JC_PATH
	};

	~BaseCompilerEnvManager(void);
	
	BaseCompilerEnvManager(void);

	void Load(const wstring& strFileName);
	void SaveCofig(const wstring& strFileName);
	IConverterContainer* getConverter(int kind, const wstring& name);
	
	IClasspathContainer* getContainer(const std::wstring& packName, int kind ,const wstring& name);
	IClasspathContainer* getContainerByPath(int kind, const boost::filesystem::path&  _path);
	IClasspathContainer* FindOrInsetJCREContainer(const std::wstring& packName,const boost::filesystem::path&  _path,
		const wstring& attach_path);
	bool AddContainer(const std::wstring& packName, IClasspathContainer* pContainer);
	
	 bool RecordRemoveJarOrClassDire(const boost::filesystem::path& abolust_path);
	 bool RecordAddJarOrClassDire(const boost::filesystem::path& abolust_path);
	 void Clear();
	 wstring GetJavacExePath();
	 wstring GetJavaExePath();
	 wstring GetJavapExePath();
	 wstring GetProguardDirPath();
	 wstring GetECJPath();
	 wstring GetJvmDllPath();
	 wstring GetJDT_ToolDllPath();
	 void ProcessPackLibs();
	 void ProcessPackSimulators();
	 void GetAllSimulatorNames(const std::wstring&packName, std::vector<std::wstring>& names);
	 bool GetSimulatorPath(const wstring& simulatorName, wstring& path);
	 bool GetHelpDocPath(const wstring& packName, wstring& path);
	 std::wstring GetStartPagePath(const std::wstring& packName = L"");
	 SimulatorItem* GetSimulatorItem(const wstring& simulatorName);
	
	 void GetAllPackNames(std::vector<std::wstring>& names);
	const ContainersCtrl& getContainerCtrl(const std::wstring& pack);
private:
	ContainersCtrl* InternalGetContainerCtrl(const std::wstring& pack);
	ContainersCtrl m_EMPTY_CTRL;
	bool LoadCofig(const wstring& strFileName);
	std::map<wstring, PackDetail*> m_ChipPackDatas;
	
public:
	vector<IConverterContainer*> m_converter_containers;
	wstring m_AppDir;
	wstring m_strSource;
	wstring m_strTarget;

	bool m_SupportInt;
	JarPathManager* jar_manager_;
	
};



 }// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED

