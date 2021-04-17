#include "stdafx.h"
#include "PathUtil.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <stdexcept>
#include <windows.h>


namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

PathUtil::PathUtil()
{
}


PathUtil::~PathUtil()
{
}
  boost::filesystem::path  PathUtil::getAbosolutPath(const  boost::filesystem::path& _p)
{
	boost::system::error_code ec;
	return boost::filesystem::system_complete(boost::filesystem::canonical(_p, ec), ec);
}

time_t PathUtil::last_write_time(const std::wstring& abolust_path)
{
	boost::system::error_code ec;
	time_t temp = 0;
	if (boost::filesystem::exists(abolust_path, ec) && boost::filesystem::is_regular_file(abolust_path, ec))
	{
		temp = boost::filesystem::last_write_time(abolust_path, ec);
	}

	return temp;
}

std::wstring PathUtil::GetAppDir()
{
	
	wchar_t ptcAppPath[MAX_PATH];
	memset(ptcAppPath, 0x00, sizeof(wchar_t)*MAX_PATH);
	::GetModuleFileNameW(NULL, ptcAppPath, MAX_PATH);
	std::wstring _strAppPath(ptcAppPath);

	std::wstring _strAppDir = _strAppPath.substr(0,_strAppPath.rfind(L'\\') + 1);
	return _strAppDir;
}

boost::filesystem::path
PathUtil::relativePath( boost::filesystem::path  p, boost::filesystem::path  relative_to) 
{

	using boost::filesystem::path;
	p = boost::filesystem::absolute(p);
	relative_to = boost::filesystem::absolute(relative_to);

	if (p == relative_to)
		return "./";
	/*!! this breaks stuff if path is a filename rather than a directory,
	which it most likely is... but then relative_to shouldn't be a filename so... */

	boost::filesystem::path from_path, from_base, output;

	boost::filesystem::path::iterator path_it = p.begin(), path_end = p.end();
	boost::filesystem::path::iterator base_it = relative_to.begin(), base_end = relative_to.end();

	// check for emptiness
	if ((path_it == path_end) || (base_it == base_end))
		throw std::runtime_error("path or relative_to was empty; couldn't generate relative path");

#ifdef WIN32
	// drive letters are different; don't generate a relative path
	if (*path_it != *base_it)
		return p;

	// now advance past drive letters; relative paths should only go up
	// to the root of the drive and not past it
	++path_it, ++base_it;
#endif

	// Cache system-dependent dot, double-dot and slash strings

	const std::string _dot = std::string(1, '.');
	const std::string _dots = "..";
	const std::string _sep = std::string(1, '/');


	// iterate over path and relative_to
	while (true) {

		// compare all elements so far of path and relative_to to find greatest common root;
		// when elements of path and relative_to differ, or run out:
		if ((path_it == path_end) || (base_it == base_end) || (*path_it != *base_it)) {

			// write to output, ../ times the number of remaining elements in relative_to;
			// this is how far we've had to come down the tree from relative_to to get to the common root
			for (; base_it != base_end; ++base_it) {
				if (*base_it == _dot)
					continue;
				else if (*base_it == _sep)
					continue;

				output /= "../";
			}

			// write to output, the remaining elements in path;
			// this is the path relative from the common root
			boost::filesystem::path::iterator path_it_start = path_it;
			for (; path_it != path_end; ++path_it) {

				if (path_it != path_it_start)
					output /= "/";

				if (*path_it == _dot)
					continue;
				if (*path_it == _sep)
					continue;

				output /= *path_it;
			}

			break;
		}

		// add directory level to both paths and continue iteration
		from_path /= path(*path_it);
		from_base /= path(*base_it);

		++path_it, ++base_it;
	}

	return output;
}

	}// Close namespace PathModel block


} // Close namespace Jikes block
