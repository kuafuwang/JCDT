
#ifndef MultipleNewMethodCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define MultipleNewMethodCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class ReplaceCorrectionProposal;
	class NewMethodCompletionProposal;
	class MultipleNewMethodCorrectionProposal :public CorrectionProposal
	{
	public:
		std::vector<ReplaceCorrectionProposal*> proposals;
		std::vector<NewMethodCompletionProposal*> method_proposals;
		~MultipleNewMethodCorrectionProposal();
		CorretioinType GetType() override {
			return MULTIPLE_NEW_METHOD_TYPE;
		}

	};

	
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

