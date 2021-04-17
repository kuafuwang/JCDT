#include "stdafx.h"
#include "MultipleInsertCorrectionProposal.h"
#include "ReplaceCorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		MultipleInsertCorrectionProposal::~MultipleInsertCorrectionProposal()
		{
			for(auto it : proposals)
			{
				delete it;
			}
			proposals.clear();
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

