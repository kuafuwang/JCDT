
#ifndef MultipleReplaceCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define MultipleReplaceCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class ReplaceCorrectionProposal;
	class MultipleReplaceCorrectionProposal :public CorrectionProposal
	{
	public:
		std::vector<ReplaceCorrectionProposal*> proposals;
		~MultipleReplaceCorrectionProposal();
		CorretioinType GetType() override {
			return MULTIPLE_REPLACE_TYPE;
		}

	};

	
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

