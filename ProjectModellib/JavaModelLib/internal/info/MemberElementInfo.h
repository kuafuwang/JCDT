
#ifndef MemberElementInfo_java_model_INCLUDED_INCLUDED
#define MemberElementInfo_java_model_INCLUDED_INCLUDED
#include "SourceRefElementInfo.h"
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class MemberElementInfo :public SourceRefElementInfo {
	public:
		MemberElementInfo()
		{
			
		}
		wstring name;
		void HelpToDeepCopy(MemberElementInfo& o);
		vector< wstring> qualifiedTypeNames;
		AccessFlags access;
		std::vector< std::wstring> modifiers;
		const std::vector<std::wstring>* const getModfiers()
		{
			
			if (modifiers.empty())
				access.GetModifers(modifiers);
			return &modifiers;
		}

		wstring getModifersString(wchar_t seperator)
		{
			if (modifiers.empty())
				access.GetModifers(modifiers);

		
			wstring ret;
			for (size_t i = 0, size = modifiers.size(); i < size; ++i){
				if (i == 0) {
					ret += modifiers[i];
				}
				else{
					ret += seperator;
					ret += modifiers[i];
				}

			}
			return ret;
		}
	};

	inline void MemberElementInfo::HelpToDeepCopy(MemberElementInfo& o)
	{
		SourceRefElementInfo::HelpToDeepCopy(o);
		qualifiedTypeNames.assign(o.qualifiedTypeNames.begin(), o.qualifiedTypeNames.end());
		 access.SetFlags(o.access);
		 name = o.name;
		 modifiers.assign(o.modifiers.begin(), o.modifiers.end());;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

