
#ifndef CorrectPackageDeclarationProposal_Jikes_Correction_INCLUDED_INCLUDED
#define CorrectPackageDeclarationProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class CorrectPackageDeclarationProposal:public CorrectionProposal
	{
	public:
		CorretioinType GetType() override {
			return  PACAKGE_DECL;
		}
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

