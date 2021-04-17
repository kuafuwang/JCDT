#include "stdafx.h"
#include "MosesPath.h"
#include <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		MosesPath::~MosesPath()
		{
		}

		MosesPath::MosesPath(MosesPathKind _kind, bool _binary) : kind(_kind),binary(_binary)
		{
		}

		
		PkgRootMoses::PkgRootMoses( const std::wstring& _proj, std::wstring& _root, bool _binary)
			: MosesPath(PKG_ROOT, binary),
		      proj(_proj),root(_root)
		{

		}

		MosesPath* PkgRootMoses::Clone()
		{
			return  new PkgRootMoses( proj, root, binary);
		}

		wstring PkgRootMoses::getPathInFileSystem()
		{
			return  root;
		}



		PkgMoses::PkgMoses(const std::wstring& _proj, const std::wstring& _root, std::wstring& _pkg, bool _binary)
			: MosesPath(PKG, _binary),
			proj(_proj),
			root(_root),
			pkg(_pkg)
		{

		}

		MosesPath* PkgMoses::Clone()
		{
			return  new PkgMoses(proj, root, pkg, binary);
		}

		wstring PkgMoses::getPathInFileSystem()
		{
			if (binary)
				return  root;
			wstring _path = root;
			_path +=  L"/" + boost::replace_all_copy(pkg,L".",L"/");
		
			return _path;
		}

		FileMoses::FileMoses(const std::wstring& _proj, const std::wstring& _root, 
			const std::wstring& _pkg, 
			std::wstring& _file, 
			bool _binary)
			: MosesPath(FILE,_binary),
			proj(_proj),
			root(_root),
			pkg(_pkg), file(_file)
		{
		}

		MosesPath* FileMoses::Clone()
		{
			return  new FileMoses(proj, root, pkg, file,binary);
		}

		FileMoses::~FileMoses()
		{
		}

		wstring FileMoses::getPathInFileSystem()
		{
			if (binary)
				return  root;
			wstring _path = root;
			_path += L"/" + boost::replace_all_copy(pkg, L".", L"/");
			_path += L"/" + file + L".java";
			return _path;
		}


		MemberInFileMoses::MemberInFileMoses(
			const std::wstring& _proj, const std::wstring& _root,
			const std::wstring& _pkg, const std::wstring& _file,
			const vector<wstring> _enclosing_types,
			const std::wstring& _name, MosesPath::MosesPathKind _k
			, bool _binary)
			: MosesPath(_k, _binary),
			proj(_proj),
			root(_root),
			pkg(_pkg),
			file(_file),
			enclosing_types(_enclosing_types),
			name(_name)
		{
		}

	
		MosesPath* MemberInFileMoses::Clone()
		{
			return  new MemberInFileMoses(proj, root, pkg,file , enclosing_types, name, kind, binary);
		}

		wstring MemberInFileMoses::getPathInFileSystem()
		{
			if (binary)
				return  root;
			wstring _path = root;
			_path += L"/" + boost::replace_all_copy(pkg, L".", L"/");
			_path += L"/" + file + L".java";
			return  _path;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

