#include "stdafx.h"

#include "ReplaceCorrectionProposal.h"
#include "MultipleNewMethodCorrectionProposal.h"
#include "NewMethodCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		MultipleNewMethodCorrectionProposal::~MultipleNewMethodCorrectionProposal()
		{
			for(auto it : proposals)
			{
				delete it;
			}
			proposals.clear();
			for(auto it : method_proposals)
			{
				delete it;
			}
			method_proposals.clear();
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

