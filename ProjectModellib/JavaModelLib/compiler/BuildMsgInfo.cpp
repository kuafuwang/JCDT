#include "BuildMsgInfo.h"

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

		 LayerPosInProject::LayerPosInProject(const wstring& _root, const wstring& _pkg, const wstring& _name, 
			 FileTypeInJavaProject _t) : root(_root), pkg(_pkg), name(_name), _type(_t)
		{

		}

		LayerPosInJavaModel::LayerPosInJavaModel(const wstring& _proj, const wstring& _root, const wstring& _pkg, const wstring& _name, FileTypeInJavaProject _t): proj(_proj), projectPos(_root, _pkg, _name, _t)
		{
		}

		LayerPosInJavaModel* LayerPosInJavaModel::Clone() const
		{
			return new LayerPosInJavaModel(proj, projectPos.root, projectPos.pkg, projectPos.name, projectPos._type);
		}

		BuildProblem::~BuildProblem()
		{
			auto _size = data.size();
			for (auto i = 0; i < _size; ++i)
				delete data[i];
		}

		BuildMsgInfo::BuildMsgInfo(const wstring& _proj): proj(_proj)
		{
		}

		BuildMsgInfo::~BuildMsgInfo()
		{
			auto it = infos.begin();
			auto end = infos.end();
			for (; it != end; ++it)
			{
				delete it->second;
			}
		}

		ComilerMsgInfo::ComilerMsgInfo(const wstring& _proj): proj(_proj)
		{
		}

		ComilerMsgInfo::~ComilerMsgInfo()
		{
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

