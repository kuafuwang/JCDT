
#ifndef MultipleInsertCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define MultipleInsertCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class ReplaceCorrectionProposal;
	class MultipleInsertCorrectionProposal :public CorrectionProposal
	{
	public:
		std::vector<ReplaceCorrectionProposal*> proposals;
		~MultipleInsertCorrectionProposal();
		CorretioinType GetType() override {
			return MULTIPLE_INSERT_REPLACE_TYPE;
		}

	};

	
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

