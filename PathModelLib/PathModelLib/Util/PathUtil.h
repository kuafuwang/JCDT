#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#ifndef PathUtil_jikes_PathModel_INCLUDED_INCLUDED
#define PathUtil_jikes_PathModel_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

class PathUtil
{
public:
	PathUtil();
	~PathUtil();
	static std::wstring GetAppDir();
	static  boost::filesystem::path
		relativePath( boost::filesystem::path  p,  boost::filesystem::path  relative_to);

	static boost::filesystem::path   getAbosolutPath(const  boost::filesystem::path& _p);
	static time_t last_write_time(const std::wstring&);
	
	
};



	}// Close namespace PathModel block


} // Close namespace Jikes block


#endif // _INCLUDED