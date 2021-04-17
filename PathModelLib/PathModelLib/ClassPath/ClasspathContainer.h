#ifndef DefaultContainer_jikes_PathModel_INCLUDED_INCLUDED
#define DefaultContainer_jikes_PathModel_INCLUDED_INCLUDED

#include <string>
#include <vector>

#include "IClasspathContainer.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "ClasspathEntry.h"

using std::vector;
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		enum Version {
			JCRE_NONE, JCRE_V211, JCRE_V212, JCRE_V221, JCRE_V222, JCRE_V301, JCRE_V304, JCRE_V305, JCRE_USER_DEFINE
		};
		extern   const wchar_t* szJCDK_2_1_1;
		extern   const wchar_t* szJCDK_2_1_2;
		extern   const wchar_t* szJCDK_2_2_1;
		extern    const wchar_t* szJCDK_2_2_2;
		extern    const wchar_t* szJCDK_3_0_4;
		extern    const wchar_t* szJCDK_3_0_5;

		extern    const wchar_t* szGP_211;
		extern    const wchar_t* szSM_2012;

		extern const wchar_t* szJDK_1_1;
		extern const wchar_t* szJDK_1_2;
		extern const wchar_t* szJDK_1_3;
		extern const wchar_t* szJDK_1_4;
		extern const wchar_t* szJDK_1_5;
		extern const wchar_t* szJDK_1_6;
		extern const wchar_t* szJDK_1_7;
		extern const wchar_t* szJDK_1_8;

		extern const wchar_t* VersionToString(Version v);
		extern Version StringToVersion(const wstring& str);
		extern  Version detectSDKVersion(const boost::filesystem::path _path);

		extern bool detectIsBootApi(boost::filesystem::path _path);
		
		 class DefaultContainer :public IClasspathContainer
		{


		protected:
			std::vector< IClasspathEntry* > entrys;
			std::vector< boost::filesystem::path > exports;
			wstring m_source_attach_path;
			bool m_recommend;
		public:
			std::wstring getSourceAttachmentRootPath() override {
				return m_source_attach_path;
			};
			DefaultContainer(const std::wstring& _name, const std::wstring& _path) ;
			DefaultContainer(): m_recommend(false)
			{
				
			}
			bool IsRecommend() { return m_recommend; }
			void SetRecommend(bool v) { m_recommend = v; }
			void initEntries();
			void initexports();
			virtual  std::vector< IClasspathEntry* > getClasspathEntries() {
				return entrys;
			}
			virtual std::vector<boost::filesystem::path  >getExports() {
				return exports;
			}
			virtual unsigned __int64 getKind() {
				return K_FOLDER;
			}

			virtual ~DefaultContainer();

		};

		class DefaultJCREContainer :public DefaultContainer
		{
		public:
			std::wstring getSourceAttachmentRootPath() override;
		private:
		
			Version version;
			wstring m_strSource;

			wstring m_strTarget;
			wstring m_source_attach_path;
			
		public:
			
			Version GetVersion()
			{
				return  version;
			}
			virtual ~DefaultJCREContainer() {};
			virtual unsigned __int64 getKind() {
				return K_DEFAULT_SYSTEM_JCRE;
			}
			wstring getSuggestTarget() const;
			wstring getSuggestSource() const;

			DefaultJCREContainer(const boost::filesystem::path& _path,
				const std::wstring& _name, Version _v);
			void EnumEntries( );
		};

	

		inline wstring DefaultJCREContainer::getSuggestTarget() const
		{
			return m_strTarget;
		}

		inline wstring DefaultJCREContainer::getSuggestSource() const
		{
			return m_strSource;
		}




		class JCREContainer :public IClasspathContainer
		{

		private:
		
			Version version;
			std::vector<IClasspathEntry*> entrys;
			vector< boost::filesystem::path > exports;
			wstring m_source_attach_path;
		public:
			

			Version GetVersion()
			{
				return  version;
			}
		
		
			virtual  std::vector< IClasspathEntry* > getClasspathEntries() {
				return entrys;
			}
			virtual vector< boost::filesystem::path > getExports()
			{
				return exports;
			}
			void  InitExports();

			virtual ~JCREContainer();

			virtual unsigned __int64 getKind() {
				return K_DEFAULT_SYSTEM_JCRE;
			}
			JCREContainer(const boost::filesystem::path& _path,
				const std::wstring& _name, const wstring&   source_attach_path, Version);
			void  EnumEntries();
			JCREContainer() {}

			std::wstring getSourceAttachmentRootPath() override {
				return m_source_attach_path;
			};
		};
		class GPContainer :public DefaultContainer
		{
			wstring m_source_attach_path;
		public:
			

			virtual ~GPContainer() {};
			virtual unsigned __int64 getKind() {
				return K_DEFAULT_GP;
			}

			GPContainer(const std::wstring& _name, const std::wstring& _path);
		};

		class SMContainer :public DefaultContainer
		{

			wstring m_source_attach_path;
		public:
	
			virtual ~SMContainer() {};
			virtual unsigned __int64 getKind() {
				return K_CHINA_SHANGMI;
			}

			SMContainer(const std::wstring& _name, const std::wstring& _path);
		};

	}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED
