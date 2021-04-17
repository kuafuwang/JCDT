#include "stdafx.h"
#include "ClasspathContainer.h"
#include <Utillib/zlib/ZipFile.h>
#include <Utillib/string/stringex.h>
#include <boost/algorithm/string.hpp>
using  boost::filesystem::path;


namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		const wchar_t* VersionToString(Version v)
		{
			switch (v)
			{
			case JCRE_V211:
				return L"JCRE_V211";
			case JCRE_V212:
				return L"JCRE_V212";
			case JCRE_V221:
				return L"JCRE_V221";
			case JCRE_V222:
				return L"JCRE_V222";
			case JCRE_V301:
				return L"JCRE_V301";
			case JCRE_V304:
				return L"JCRE_V304";
	

			case JCRE_V305:
				return L"JCRE_V305";

			case JCRE_USER_DEFINE:
				return L"JCRE_USER_DEFINE";
			default:
				break;;
			}
			return L"JCRE_USER_DEFINE";
			
		}
		Version StringToVersion(const wstring& str)
		{
			if(L"JCRE_V211" == str)
			{
				return JCRE_V211;
			}

			if (L"JCRE_V212" == str)
			{
				return JCRE_V212;
			}

			if (L"JCRE_V221" == str)
			{
				return JCRE_V221;
			}

			if (L"JCRE_V222" == str)
			{
				return JCRE_V222;
			}

			if (L"JCRE_V301" == str)
			{
				return JCRE_V301;

			}

			if (L"JCRE_V304" == str)
			{
				return JCRE_V304;
			}

			if (L"JCRE_V305" == str)
			{
				return JCRE_V305;
			}

			if (L"JCRE_USER_DEFINE" == str)
			{
				return JCRE_USER_DEFINE;
			}

			
			return JCRE_NONE;
			

		}
		//const wchar_t* DefaultJCREContainer::DEFAULT_NMAE = L"JavaCosDK";
		const wchar_t* szJCDK_2_1_1 = L"java_card_kit-2_1_1";
		const wchar_t* szJCDK_2_1_2 = L"java_card_kit-2_1_2";
		const wchar_t* szJCDK_2_2_1 = L"java_card_kit-2_2_1";
		const wchar_t* szJCDK_2_2_2 = L"java_card_kit-2_2_2";
		const wchar_t* szJCDK_3_0_4 = L"java_card_kit-3_0_4";
		const wchar_t* szJCDK_3_0_5 = L"java_card_kit-3_0_5";
		const wchar_t* szGP_211 = L"Global Platform 2.1.1";
		const wchar_t* szSM_2012 = L"China cryptographic algorithm specific 2012";;

		const wchar_t* szJDK_1_1 = L"1.1";
		const wchar_t* szJDK_1_2 = L"1.2";
		const wchar_t* szJDK_1_3 = L"1.3";
		const wchar_t* szJDK_1_4 = L"1.4";
		const wchar_t* szJDK_1_5 = L"1.5";
		const wchar_t* szJDK_1_6 = L"1.6";
		const wchar_t* szJDK_1_7 = L"1.7";
		const wchar_t* szJDK_1_8 = L"1.8";
		const wchar_t* szJDK_1_9 = L"1.9";
		const wchar_t* szJDK_1_10 = L"1.10";
		const wchar_t* szJDK_1_11 = L"1.11";
		const wchar_t* szJDK_1_12 = L"1.12";
		bool detectIsBootApi(path _path)
		{
			boost::system::error_code ec;
			if (!boost::filesystem::is_regular_file(_path, ec))
			{
				return false;
			}
			if (!boost::filesystem::exists(_path, ec))
			{
				return false;
			}
			std::wstringex temp = _path.wstring();
			temp.tolower();
			if(!temp.end_with(L".jar"))
			{
				return false;
			}
			ZipFile _zip(_path.string().c_str(), ZipFile::read);
			if (!_zip)
			{
				return false;
			}
			std::vector<std::string> nameList;
			_zip.namelist(nameList);
			for (auto it : nameList)
			{
				if (it == "java/lang/Object.class")
				{
					return  true;
				}
			}
			return false;
		}
		Version detectSDKVersion(path root)
		{
			boost::system::error_code ec;
			if(boost::filesystem::is_regular_file(root, ec))
			{
				return JCRE_NONE;
			}
			bool has_exist = boost::filesystem::is_directory(root, ec);//1.检查 api.jar是否存在
			if (!has_exist)
				return JCRE_NONE;

			path libDir(root);
			libDir.append((L"lib"));

			has_exist = is_directory(libDir, ec);//1.检查 文件夹是否存在
			if (!has_exist)
				return JCRE_NONE;
			Version version;
			if (exists(path(libDir).append(L"tools.jar"), ec))
			{
				path api_classic_path = path(libDir).append((L"api_classic.jar"));
				if (!exists(api_classic_path, ec))
				{
					return JCRE_NONE;
				}
				ZipFile _zip(api_classic_path.string().c_str(), ZipFile::read);
				if (!_zip)
				{
					return JCRE_NONE;
				}
				std::vector<std::string> nameList;
				_zip.namelist(nameList);
				for (auto it : nameList)
				{
					if (it == "javacard/framework/SensitiveArrays.class")
					{
						return  Version::JCRE_V305;
					}
				}
				for (auto it : nameList)
				{
					if (it == "javacardx/framework/string/StringUtil.class")
					{
						return  Version::JCRE_V304;
					}
				}
				return Version::JCRE_V301;
			}

			if (exists(path(libDir).append((L"api21.jar")), ec))
			{
				return  Version::JCRE_V221;
			}
			if (exists(path(libDir).append((L"converter.jar")), ec))
			{
				// assume 2.2.1 first
				version = Version::JCRE_V221;
				path _api_path = path(libDir).append((L"api.jar"));
				if (exists(_api_path, ec))
				{
					ZipFile _zip(_api_path.string().c_str(), ZipFile::read);
					if (!_zip)
					{
						return JCRE_NONE;
					}
					std::vector<std::string> nameList;
					_zip.namelist(nameList);
					for (auto it : nameList)
					{
						if (it == "javacardx/apdu/ExtendedLength.class")
						{
							return  Version::JCRE_V222;
						}
					}
				}

				return version;
			}

			return JCRE_NONE;

		}
		
		void  DefaultContainer::initEntries() {
			using namespace boost::filesystem;

			directory_iterator end;
			vector<wstring> classpaths;

			if (exists(path(m_path).append(L"lib"))) {
				for (directory_iterator pos(path(m_path).append(L"lib")); pos != end; ++pos) {
					if (is_directory(*pos) == false) {

						path file(*pos);
						if (file.extension().wstring() == L".jar")
							entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
								getSourceAttachmentRootPath(),L""));
					}
				}
				exist = true;
			}
			else
			{
				exist = false;
			}
		}
		void  DefaultContainer::initexports()
		{
			using namespace boost::filesystem;
			if (exports.size() == 0) {
				if (exists(path(m_path).append(L"export_files"))) {
					directory_iterator end;
					for (directory_iterator pos(path(m_path).append(L"export_files")); pos != end; ++pos) {
						if (is_directory(*pos)) {
							exports.push_back(path(*pos).wstring());
						}
					}
					exist = true;
				}
				else
				{
					exist = false;
				}
			}
		}
		DefaultContainer::~DefaultContainer()
		{
			size_t len = entrys.size();
			for (size_t i = 0; i < len; ++i)
			{
				delete entrys[i];

			}
			entrys.clear();
		}

		std::wstring DefaultJCREContainer::getSourceAttachmentRootPath()
		{
			return m_source_attach_path;
		};

		DefaultJCREContainer::DefaultJCREContainer(const boost::filesystem::path& _path,
			const std::wstring& _name,Version _v)
		{

			m_path = _path;
			name = _name;
			
			version = _v;
			switch (version)
			{
			case JCRE_V211:
			case JCRE_V212:
			case JCRE_V221:
			case JCRE_V222:
		
				m_strTarget = szJDK_1_5;
				m_strSource = szJDK_1_5;
				break;
			case JCRE_V301:
			case JCRE_V304:
				m_strTarget = szJDK_1_6;
				m_strSource = szJDK_1_6;
				break;

			case JCRE_V305:
				m_strTarget = szJDK_1_8;
				m_strSource = szJDK_1_8;
				break;

			default:
				version = _v;
				m_strTarget = szJDK_1_5;
				m_strSource = szJDK_1_5;
				break;
			}
		
			m_source_attach_path = boost::filesystem::path(m_path).append(L"attach_source").wstring();
			boost::replace_all(m_source_attach_path, L"\\", L"/");
		  
			EnumEntries();
			
			
			initexports();
		}

		void  DefaultJCREContainer::EnumEntries() 
		{
			DefaultContainer::initEntries();
			/*using namespace boost::filesystem;



			if (!exists(path(m_path).append(L"lib")))
			{
				exist = false;
				return;
			}
			directory_iterator end;
			vector<wstring> classpaths;
			
			for (directory_iterator pos(path(m_path).append(L"lib")); pos != end; ++pos) 
			{
				if (is_directory(*pos))
				{
					continue;
				}
				
				path file(*pos);
				if (file.extension().wstring() != L".jar")
				{
					continue;;
				}
				auto filename = file.filename().wstring();
				boost::to_lower(filename);
				if(L"api.jar" == filename || L"api21.jar" == filename || L"api_classic.jar" == filename)
				{
					entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
						getSourceAttachmentRootPath(),L""));
				}
				else if(L"api_connected.jar" == filename || L"api_classic_annotations.jar" == filename)
				{
					if(version == JCRE_USER_DEFINE)
					{
						entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
							getSourceAttachmentRootPath(), L""));
					}
				}
				
				
			}
			exist = true;*/
			
		}
		void  JCREContainer::EnumEntries()
		{
			using namespace boost::filesystem;
			/*using namespace boost::filesystem;



			if (!exists(path(m_path).append(L"lib")))
			{
				exist = false;
				return;
			}
			directory_iterator end;
			vector<wstring> classpaths;

			for (directory_iterator pos(path(m_path).append(L"lib")); pos != end; ++pos)
			{
				if (is_directory(*pos))
				{
					continue;
				}

				path file(*pos);
				if (file.extension().wstring() != L".jar")
				{
					continue;;
				}
				auto filename = file.filename().wstring();
				boost::to_lower(filename);
				if (L"api.jar" == filename || L"api21.jar" == filename || L"api_classic.jar" == filename)
				{
					entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
						getSourceAttachmentRootPath(),L""));
				}
				else if (L"api_connected.jar" == filename || L"api_classic_annotations.jar" == filename)
				{
					if (version == JCRE_USER_DEFINE)
					{
						entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
							getSourceAttachmentRootPath(), L""));
					}
				}
				

			}
			exist = true;*/
			directory_iterator end;
			vector<wstring> classpaths;

			if (exists(path(m_path).append(L"lib"))) {
				for (directory_iterator pos(path(m_path).append(L"lib")); pos != end; ++pos) {
					if (is_directory(*pos) == false) {

						path file(*pos);
						if (file.extension().wstring() == L".jar")
							entrys.push_back(new LibraryClasspathEntry(file, file.wstring(),
								getSourceAttachmentRootPath(), L""));
					}
				}
				exist = true;
			}
			else
			{
				exist = false;
			}

		}


		void  JCREContainer::InitExports()
		{
			if (exports.size() == 0) {
				boost::filesystem::path _path = boost::filesystem::path(m_path).append(L"api_export_files");
				if (boost::filesystem::exists(_path)) {
					//1.检查 api.jar是否存在)
					exports.push_back(_path);
					exist = true;
				}
				else
				{
					exist = false;
				}
			}
		}
		JCREContainer::JCREContainer(const boost::filesystem::path& _path, const std::wstring& _name,
			const wstring&   source_attach_path, Version _v) :IClasspathContainer(_path, _name), m_source_attach_path(source_attach_path), version(_v)
		{
			boost::replace_all(m_source_attach_path, L"\\", L"/");
			EnumEntries();
			InitExports();
		}
		JCREContainer::~JCREContainer()
		{

			size_t len = entrys.size();
			for (size_t i = 0; i < len; ++i)
			{
				delete entrys[i];

			}

		};

		GPContainer::GPContainer(const std::wstring& _name, const std::wstring& _path) :DefaultContainer(_name, _path) {


		}

		SMContainer::SMContainer(const std::wstring& _name, const std::wstring& _path) : DefaultContainer(_name, _path){


		}
		DefaultContainer::DefaultContainer(const std::wstring& _name, const std::wstring& _path):m_recommend(false)
		{




			m_path = _path;
			name = _name;

			m_source_attach_path = boost::filesystem::path(m_path).append(L"attach_source").wstring();
			boost::replace_all(m_source_attach_path, L"\\", L"/");
			initEntries();
			initexports();
		}

	}// Close namespace PathModel block


} // Close namespace Jikes block
