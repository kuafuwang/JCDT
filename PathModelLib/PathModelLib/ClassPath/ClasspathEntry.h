#ifndef ClasspathEntry_jikes_PathModel_INCLUDED_INCLUDED
#define ClasspathEntry_jikes_PathModel_INCLUDED_INCLUDED

#include <string>
#include "IClasspathEntry.h"
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;
using std::wstring;

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

class ClasspathEntry :public IClasspathEntry
{
public:
	std::wstring getSourceAttachmentRootPath();
protected:
	boost::filesystem::path abolust_path;
	wstring name;
	wstring sourceAttachmentPath;
public:
	ClasspathEntry(const fs::path& _abolust_path, const wstring& nm, const wstring& attach_path);

	ClasspathEntry();

	virtual ~ClasspathEntry();

	void setAbolutePath(fs::path& _abolust_path);

	void setAttachSourcePath(fs::path& _abolust_path)
	{
		sourceAttachmentPath = _abolust_path.wstring();
	}

	const fs::path& getAbolutePath();

	void setName(const wstring& nm);

	const wstring& getName();
};

class LibraryClasspathEntry :public ClasspathEntry
{
public:
	LibraryClasspathEntry(const fs::path& _abolust_path, const wstring& nm, const wstring& attach_path, const fs::path& _export_path);

	boost::filesystem::path   GetExportPath();
	void SetExportPath(const boost::filesystem::path&);

	virtual int getEntryKind();
	static wstring  GetMD5Name(const fs::path&);
	static wstring  GetFileMD5Stamp(const fs::path&);
	static wstring  GetNameInPackageView(const fs::path&);
	void   SetMd5ReadMeFile(const wstring& _f)
	{
		m_MD5_readme_file = _f;
	}
	const wstring& GetMd5ReadMeFile()
	{
		return m_MD5_readme_file;
	}
	time_t last_write_time();
	void SetTime(time_t _t)
	{
		m_time = _t;
	}
	time_t GetTime() const
	{
		return m_time;
	}
	bool is_lib_exists();
	bool is_MD5_readme_file_exist();
private:
	boost::filesystem::path m_export_path;
	wstring m_MD5_readme_file;
	time_t m_time;
};

class SourceClasspathEntry :public ClasspathEntry
{
public:
	SourceClasspathEntry(const fs::path& _abolust_path, const wstring& nm, const wstring& attach_path);

	virtual int getEntryKind();
};

class ProjectClasspathEntry :public IClasspathEntry
{

private:
	std::wstring projectName;
public:
	ProjectClasspathEntry(const std::wstring& _projectName);

	ProjectClasspathEntry();

	virtual ~ProjectClasspathEntry();
	void setProjectName(const std::wstring& projectName_);
	

	const std::wstring& getProjectName();

	virtual int getEntryKind();
};



class ContainerClasspathEntry :public IClasspathEntry
{

private:
	std::wstring containerName;
	int  containerKind;

public:
	ContainerClasspathEntry(const std::wstring& _containerName, int kind);

	void setContainerkind(int kind);

	int getContainerKind() const;

	ContainerClasspathEntry();

	virtual ~ContainerClasspathEntry();
	void setContainerName(const std::wstring& _containerName);
	const std::wstring& getContainerName();

	virtual int getEntryKind();
};



}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED

