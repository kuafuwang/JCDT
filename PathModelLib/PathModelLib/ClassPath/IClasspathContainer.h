#ifndef IClasspathContainer_jikes_PathModel_INCLUDED_INCLUDED
#define IClasspathContainer_jikes_PathModel_INCLUDED_INCLUDED

#include <string>

#include <vector>
#include <boost/filesystem/path.hpp>

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

class IClasspathEntry;
class IClasspathContainer {

protected:
	
	boost::filesystem::path m_path;
	
	std::wstring name;
	bool exist;

public :

	/**
	* Kind for a container mapping to an application library
	*/
	static const unsigned __int64 K_FOLDER = 1;

	/**
	* Kind for a container mapping to a JCRE system library, implicitly contributed by the runtime
	*/
	// 
	static const unsigned __int64 K_DEFAULT_SYSTEM_JCRE = 2;

	/**
	* Kind for a container mapping to a system library
	*/
	static const unsigned __int64 K_SYSTEM_JCRE = 4;

	static const unsigned __int64 K_DEFAULT_GP = 8;

	static const unsigned __int64 K_CHINA_SHANGMI = 16;


	IClasspathContainer(const boost::filesystem::path& _path,const std::wstring& _name)
	:m_path(_path), name(_name), exist(true)
	{
		
	}
	IClasspathContainer():exist(true) {

	}

	
	virtual std::vector< IClasspathEntry* >  getClasspathEntries() = 0;

	virtual std::vector< boost::filesystem::path >  getExports() = 0;

	virtual unsigned __int64 getKind() = 0;

	virtual ~IClasspathContainer()
	{
		
	}

	 boost::filesystem::path  getPath();
	 void setPath(const boost::filesystem::path & _path);

	 std::wstring getName();
	 void setName(const std::wstring& _name);
	 static unsigned __int64 findKind(std::wstring& kindName);
	 static std::wstring getKindName(int kind);

	 virtual std::wstring getSourceAttachmentRootPath() = 0;


};

inline unsigned __int64 IClasspathContainer::findKind(std::wstring& kindName)
{
	if (L"K_FOLDER" == kindName)
	{
		return K_FOLDER;
	}
	else if (L"K_DEFAULT_SYSTEM_JCRE" == kindName)
	{
		return K_DEFAULT_SYSTEM_JCRE;
	}
	else if (L"K_SYSTEM_JCRE" == kindName)
	{
		return K_SYSTEM_JCRE;
	}
	else if (L"K_DEFAULT_GP" == kindName)
	{
		return K_DEFAULT_GP;
	}
	else if (L"K_CHINA_SHANGMI" == kindName)
	{
		return K_CHINA_SHANGMI;
	}
	else
	{
		return K_FOLDER;
	}
}
inline std::wstring IClasspathContainer::getKindName(int kind)
{
	if (K_FOLDER == kind)
	{
		return   L"K_FOLDER";
	}
	else if (K_DEFAULT_SYSTEM_JCRE == kind)
	{
		return L"K_DEFAULT_SYSTEM_JCRE";
	}
	else if (K_SYSTEM_JCRE == kind)
	{
		return L"K_SYSTEM_JCRE";
	}
	else if (K_DEFAULT_GP == kind)
	{
		return L"K_DEFAULT_GP";
	}
	else if (K_CHINA_SHANGMI == kind)
	{
		return L"K_CHINA_SHANGMI";
	}
	else
	{
		return   L"K_FOLDER";
	}
}
inline  boost::filesystem::path  IClasspathContainer::getPath()
{
	return m_path;
}

inline void IClasspathContainer::setPath(const boost::filesystem::path & _path)
{
	m_path = _path;
}

inline  std::wstring IClasspathContainer::getName()
{
	return name;
}
inline void IClasspathContainer::setName(const std::wstring& _name)
{
	name = _name;
}

}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED

