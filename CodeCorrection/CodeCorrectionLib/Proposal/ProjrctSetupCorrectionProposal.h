
#ifndef ProjrctSetupCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define ProjrctSetupCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class ProjrctSetupCorrectionProposal :public CorrectionProposal
	{
	public:
		std::wstring project;
		ProjrctSetupCorrectionProposal(
			std::wstring& _project
		) :project(_project)
		{

		}

		CorretioinType GetType() override {
			return  PROJECTSETUP_TYPE;
		}
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

