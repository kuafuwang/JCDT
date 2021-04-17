#include "stdafx.h"
#include "ClasspathEntry.h"
#include <boost/algorithm/string.hpp>

#include <JCDT_Lib/internal/util/CharOperation.h>
#include <boost/filesystem/operations.hpp>
#include "PathModelLib/Util/PathUtil.h"
using  boost::filesystem::path;


namespace Jikes { // Open namespace Jikes block


	namespace PathModel {



		 const std::wstring& ProjectClasspathEntry::getProjectName()
		{
			return projectName;
		}

		int ProjectClasspathEntry::getEntryKind()
		{
			return IClasspathEntry::CPE_PROJECT;
		}

		ContainerClasspathEntry::
		ContainerClasspathEntry(const std::wstring& _containerName, int kind): containerName(_containerName),
		                                                                       containerKind(kind)
		{
		}

		void ContainerClasspathEntry::setContainerkind(int kind)
		{
			containerKind = kind;
		}

		int ContainerClasspathEntry::getContainerKind() const
		{
			return containerKind;
		}

		ContainerClasspathEntry::ContainerClasspathEntry()
		{
		}

		ContainerClasspathEntry::~ContainerClasspathEntry()
		{
		}

		std::wstring ClasspathEntry::getSourceAttachmentRootPath()
		{
			return sourceAttachmentPath;
		}

		ClasspathEntry::ClasspathEntry(const fs::path& _abolust_path, const wstring& nm,
		                               const wstring& attach_path): name(nm)
		{
			abolust_path = boost::replace_all_copy(_abolust_path.wstring(), L"\\", L"/");
			sourceAttachmentPath = boost::replace_all_copy(attach_path, L"\\", L"/");
		}

		ClasspathEntry::ClasspathEntry()
		{
		}

		ClasspathEntry::~ClasspathEntry()
		{
		}

		void ClasspathEntry::setAbolutePath(fs::path& _abolust_path)
		{
			abolust_path = _abolust_path;
		}

		const fs::path& ClasspathEntry::getAbolutePath()
		{
			return abolust_path;
		}

		void ClasspathEntry::setName(const wstring& nm)
		{
			name = nm;
		}

		const wstring& ClasspathEntry::getName()
		{
			return name;
		}

		LibraryClasspathEntry::LibraryClasspathEntry(const fs::path& _abolust_path, const wstring& nm,
		                    const wstring& attach_path, const fs::path& _export_path): ClasspathEntry(
			_abolust_path, nm, attach_path), m_export_path(_export_path)
		{
			m_time = last_write_time();
		}

		boost::filesystem::path LibraryClasspathEntry::GetExportPath()
		{
			return m_export_path;
		}

		void LibraryClasspathEntry::SetExportPath(const boost::filesystem::path& _p)
		{
			m_export_path = _p;
		}

		int LibraryClasspathEntry::getEntryKind()
		{
			return IClasspathEntry::CPE_LIBRARY;
		}
	//	static wstring  GetMD5Name(const wstring& lib__path);
		//static wstring  GetFileMD5Stamp(const wstring& lib__path);
		 wstring LibraryClasspathEntry::GetMD5Name(const path& lib_path)
		{
			
			 std::vector<unsigned char> md5_value;
			 auto _path_string = lib_path.string();
			
			 char hex[35];
			 memset(hex, 0, sizeof(hex));
			 for (int i = 0; i < md5_value.size(); ++i)
			 {
				 sprintf(hex + i * 2, "%02x", md5_value[i]);
			 }
			 hex[32] = '\0';
			 return   CharOperation::ConvertUtf8ToUnicode(hex);
		}
		 wstring LibraryClasspathEntry::GetFileMD5Stamp(const path& lib_path)
		 {
			 std::vector<unsigned char> md5_value;

			 char hex[35];
			 memset(hex, 0, sizeof(hex));
			 for (int i = 0; i < md5_value.size(); ++i)
			 {
				 sprintf(hex + i * 2, "%02x", md5_value[i]);
			 }
			 hex[32] = '\0';
			 return  CharOperation::ConvertUtf8ToUnicode(hex);
		 }

		wstring LibraryClasspathEntry::GetNameInPackageView(const fs::path& _path)
		{
			auto fileName = _path.filename().wstring();
			fileName += L":";
			fileName += _path.wstring();
			return fileName;
		}

		 time_t LibraryClasspathEntry::last_write_time()
		{

			return PathUtil::last_write_time(abolust_path.wstring());
		}

		bool LibraryClasspathEntry::is_lib_exists()
		{
			if (boost::filesystem::exists(abolust_path) && boost::filesystem::is_regular_file(abolust_path))
			{
				return true;
			}
			return false;
		}
		bool LibraryClasspathEntry::is_MD5_readme_file_exist()
		{
			if (boost::filesystem::exists(m_MD5_readme_file) && boost::filesystem::is_regular_file(m_MD5_readme_file))
			{
				return true;
			}
			return false;
		}
		SourceClasspathEntry::SourceClasspathEntry(const fs::path& _abolust_path, const wstring& nm,
		                                           const wstring& attach_path): ClasspathEntry(
			_abolust_path, nm, attach_path)
		{
		}

		int SourceClasspathEntry::getEntryKind()
		{
			return IClasspathEntry::CPE_SOURCE;
		}

		ProjectClasspathEntry::ProjectClasspathEntry(const std::wstring& _projectName): projectName(_projectName)
		{
		}

		ProjectClasspathEntry::ProjectClasspathEntry()
		{
		}

		ProjectClasspathEntry::~ProjectClasspathEntry()
		{
		}

		void ProjectClasspathEntry::setProjectName(const std::wstring& projectName_)
		{
			projectName = projectName_;
		}



		 void ContainerClasspathEntry::setContainerName(const std::wstring& _containerName)
		{
			containerName = _containerName;
		}
		 const std::wstring& ContainerClasspathEntry::getContainerName()
		{
			return containerName;
		}

		int ContainerClasspathEntry::getEntryKind()
		{
			return IClasspathEntry::CPE_CONTAINER;
		}
	}// Close namespace PathModel block


} // Close namespace Jikes block
