
#ifndef MosesPath_jikes_PathModel_INCLUDED_INCLUDED
#define MosesPath_jikes_PathModel_INCLUDED_INCLUDED
#include <string>
#include <vector>
using namespace  std;
namespace Jikes { // Open namespace Jikes block


namespace PathModel{
	
	class MosesPath
	{
	public:
		enum MosesPathKind
		{
			PKG_ROOT,
			PKG,
			FILE,
			TYPE,
			METHOD,
			FIELD
		};
		virtual ~MosesPath();;

		MosesPath(MosesPathKind _kind, bool _binary);
	
		virtual MosesPath* Clone() = 0;
		virtual wstring getPathInFileSystem() = 0;
		MosesPathKind kind;
		bool binary;

	};
	class PkgRootMoses:public MosesPath
	{
	public:
		PkgRootMoses( const std::wstring& _proj, std::wstring& _root, bool _binary);

		MosesPath* Clone() override;
		wstring getPathInFileSystem() override;
	
		std::wstring proj;
		std::wstring root;
	};

	class PkgMoses:public MosesPath
	{
	public:
		PkgMoses(const std::wstring& _proj, const std::wstring& _root, std::wstring& _pkg, bool _binary);

		MosesPath* Clone() override;
		wstring getPathInFileSystem() override;
		std::wstring proj;
		std::wstring root;
		std::wstring pkg;
	};

	class FileMoses:public MosesPath
	{
	public:
		FileMoses(const std::wstring& _proj, const std::wstring& _root, 
			const std::wstring& _pkg, std::wstring& _file, bool _binary);

		MosesPath* Clone() override;

		~FileMoses() override;
		wstring getPathInFileSystem() override;
		std::wstring proj;
		std::wstring root;
		std::wstring pkg;
		std::wstring file;
	};
	
	class MemberInFileMoses :public MosesPath
	{
	public:
		MemberInFileMoses(const std::wstring& _proj,
			const std::wstring& _root,
			const std::wstring& _pkg,
			const std::wstring& _file,
			const vector<wstring> _enclosing_types,
			const std::wstring& _name, MosesPath::MosesPathKind , bool _binary);
		
		MosesPath* Clone() override;
		wstring getPathInFileSystem() override;
		std::wstring proj;
		std::wstring root;
		std::wstring pkg;
		std::wstring file;
		vector<wstring> enclosing_types;
		std::wstring name;
	};
	
}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

