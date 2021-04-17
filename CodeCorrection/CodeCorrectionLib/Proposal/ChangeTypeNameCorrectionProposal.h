
#ifndef ChangeTypeNameCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define ChangeTypeNameCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class ChangeTypeNameCorrectionProposal :public CorrectionProposal
	{
	public:
	
		ChangeTypeNameCorrectionProposal(const std::wstring& _project,
			const std::wstring& _pkg, const std::wstring& _simpleName)
			:project(_project), pacakgeName(_pkg), simpleName(_simpleName)
		{
			
		}
	
		
		
		CorretioinType GetType() override {
			return  REPLACE_TYPE;
		}
		std::wstring project;
		std::wstring pacakgeName;
		std::wstring simpleName;

		std::wstring newName;
	
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

