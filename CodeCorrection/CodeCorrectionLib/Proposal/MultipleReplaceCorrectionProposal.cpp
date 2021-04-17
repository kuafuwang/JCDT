#include "stdafx.h"
#include "MultipleReplaceCorrectionProposal.h"
#include "ReplaceCorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


	namespace Correction {

		MultipleReplaceCorrectionProposal::~MultipleReplaceCorrectionProposal()
		{
			for(auto it : proposals)
			{
				delete it;
			}
			proposals.clear();
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

