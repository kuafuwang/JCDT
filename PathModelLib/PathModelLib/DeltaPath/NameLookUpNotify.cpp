#include "stdafx.h"
#include "NameLookUpNotify.h"

namespace Jikes { // Open namespace Jikes block

	namespace PathModel {
		

		NameLookUpNotify::NameLookUpNotify(
			ResourceOptKind _kind,
			const wstring& _name,
			NotifyLevel _level)
			: level(_level),kind(_kind),name(_name)
		{

		}

		NameLookUpNotify::NameLookUpNotify(NameLookUpNotify& o)
			: level(o.level), kind(o.kind),name(o.name)
		{

		}


		NameLookUpNotify* ProjectNotify::Clone()
		{
			return new ProjectNotify(*this);
		}

		PackageFragmentRootMsg::PackageFragmentRootMsg(ResourceOptKind _kind, const wstring& _name,
			const wstring& _root_path, const wstring& _attach_path)
			: NameLookUpNotify(_kind,_name, PKG_ROOT_LV), root_path(_root_path), attach_path(_attach_path)
		{

		}

		PackageFragmentRootMsg::PackageFragmentRootMsg(PackageFragmentRootMsg&o): NameLookUpNotify(o),
			root_path(o.root_path), attach_path(o.attach_path)
		{

		}

		NameLookUpNotify* PackageFragmentRootMsg::Clone()
		{
			return new PackageFragmentRootMsg(*this);
		}

		PackageFragmentRootMsg::~PackageFragmentRootMsg()
		{
		
		}

		PackageFragmentMsg::PackageFragmentMsg(ResourceOptKind _kind, const wstring& _name, const wstring& _root_name)
			: NameLookUpNotify(_kind, _name, PKG_LV),root_name(_root_name)
		{
		}

		NameLookUpNotify* PackageFragmentMsg::Clone()
		{
			return new PackageFragmentMsg(*this);
		}

		PackageFragmentMsg::PackageFragmentMsg(PackageFragmentMsg&o)
			: NameLookUpNotify(o),root_name(o.root_name)
		{
		}

		

		FileMsg::FileMsg(ResourceOptKind _kind, const wstring& _name,
			const wstring& _root_name,const wstring& _pkg_name):
			NameLookUpNotify(_kind,_name, FILE_LV),root_name(_root_name),pkg_name(_pkg_name)
		{
		}

		FileMsg::FileMsg(FileMsg& o)
			: NameLookUpNotify(o),root_name(o.root_name),pkg_name(o.pkg_name)
		{
		}

		FileMsg::~FileMsg()
		{
		}

		NameLookUpNotify* FileMsg::Clone()
		{
			return new FileMsg(*this);
		}

	}// Close namespace JavaModel block

} // Close namespace Jikes block

