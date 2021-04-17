
#ifndef NameLookUpNotify_javamodel_INCLUDED_INCLUDED
#define NameLookUpNotify_javamodel_INCLUDED_INCLUDED
#include <map>
#include <set>
#include <string>
#include <vector>
using std::wstring;
namespace Jikes { // Open namespace Jikes block
	namespace PathModel {

enum ResourceOptKind
	{
		ELEMENT_ADD,
		ELEMENT_CHANGE,
		ELEMENT_REMOVE
	};
	enum CompilerAct {
		REBUILD,
		INCREMENT,
		NOTHING
	};



	enum NotifyLevel{
		PROJECT_LV,
		PKG_ROOT_LV,
		PKG_LV,
		FILE_LV
	};
	

	
	class NameLookUpNotify
	{
	public:
		virtual ~NameLookUpNotify()
		{
		}
	   explicit NameLookUpNotify(ResourceOptKind _kind, const wstring& _name, NotifyLevel _level);

		NameLookUpNotify(NameLookUpNotify&);

		NotifyLevel level;
		ResourceOptKind kind;
		std::wstring  name;

		virtual NameLookUpNotify* Clone() = 0;
		
	};

	struct ProjectNotify :public NameLookUpNotify
	{
		ProjectNotify(ResourceOptKind _kind, const wstring& _name)
			: NameLookUpNotify(_kind, _name, PROJECT_LV)
		{
		}
		NameLookUpNotify* Clone() override;
		explicit ProjectNotify(NameLookUpNotify& name_look_up_notify)
			: NameLookUpNotify(name_look_up_notify)
		{
		}
	};

	struct PackageFragmentRootMsg :public NameLookUpNotify
	{
		PackageFragmentRootMsg(ResourceOptKind _kind, const wstring& _name, 
			const wstring& _root_path, const wstring& _attach_path );
		PackageFragmentRootMsg(PackageFragmentRootMsg&);
		NameLookUpNotify* Clone() override;
		wstring root_path ;
		wstring attach_path ;
		bool source_only = false;
		~PackageFragmentRootMsg();
	};
	struct PackageFragmentMsg :public NameLookUpNotify
	{
		PackageFragmentMsg(ResourceOptKind _kind,  const wstring& _name, const wstring& _root_name);
		NameLookUpNotify* Clone() override;
		PackageFragmentMsg(PackageFragmentMsg&);
		std::wstring root_name;	
		
	};
	struct FileMsg :public NameLookUpNotify{
		FileMsg(ResourceOptKind _kind, const wstring& _name,
			const std::wstring& _root_name, const std::wstring& _pkg_name);

		FileMsg(FileMsg&);

		~FileMsg();
		NameLookUpNotify* Clone() override;
		std::wstring root_name;
		std::wstring pkg_name;
		wstring file_path ;

	};

	}// Close namespace JavaModel block

} // Close namespace Jikes block


#endif // _INCLUDED

