
#ifndef BuildMsgInfo_java_model_INCLUDED_INCLUDED
#define BuildMsgInfo_java_model_INCLUDED_INCLUDED
#include <string>
#include <map>
#include <set>

#include <vector>

using std::wstring;
using std::map;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class DefaultProblem;
	enum FileTypeInJavaProject
	{
		_A_EDIT_FILE,
		_BELONG_TO_CLASS_FILE,
		NO_MY_GUY
	};

	class  LayerPosInProject
	{
	public:
		LayerPosInProject(const wstring& _root,
		                  const wstring& _pkg,
		                  const wstring& _name, FileTypeInJavaProject _t
		);
		wstring root;
		wstring pkg;
		wstring name;
		FileTypeInJavaProject _type;
		
	};
	class  LayerPosInJavaModel
	{
	public:
		LayerPosInJavaModel(const wstring& _proj, const wstring& _root,
		                    const wstring& _pkg, const wstring& _name,
		                    FileTypeInJavaProject _t
		);

		LayerPosInJavaModel* Clone() const;
		wstring proj;
		LayerPosInProject projectPos;
	};
	class BuildProblem
	{
	public:
		~BuildProblem();
		std::vector<DefaultProblem*> data;
	};

	

	class   BuildMsgInfo
	{
	public:
		BuildMsgInfo(const wstring& _proj);

		~BuildMsgInfo();
		wstring proj;
		map< wstring, LayerPosInProject* > infos;
	};
	class   ComilerMsgInfo
	{
	public:
		ComilerMsgInfo(const wstring& _proj);

		~ComilerMsgInfo();
		wstring proj;
		std::set< wstring > infos;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

