
#ifndef CorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define CorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED

#include <string>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class CorrectionProposal
	{
	public:
		enum  CorretioinType
		{
			PACAKGE_DECL,
			MULTIPLE_REPLACE_TYPE,
			MULTIPLE_INSERT_REPLACE_TYPE,
			MULTIPLE_NEW_METHOD_TYPE,
			INSERT_TYPE,
		
			NEWCU_TYPE,
			RENAME_CU,
			NEW_METHOD_TYPE,
		
			PROJECTSETUP_TYPE,
			REPLACE_TYPE

		};
		enum ICON_TYPE
		{
			IMPORT_ICON,
			CHANGE_ICON,
			INTERFERC_ICON,
			CLASS_ICON,
			REMOVE_ICON,
			NO_ICON
		};
		CorrectionProposal():icon_type(NO_ICON){}
		virtual ~CorrectionProposal();
		std::wstring  fDisplayString;
		virtual CorretioinType GetType() = 0;
		ICON_TYPE  icon_type;
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

