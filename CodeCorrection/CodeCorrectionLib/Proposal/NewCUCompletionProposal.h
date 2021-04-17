
#ifndef NewCUCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define NewCUCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class NewCUCompletionProposal :public CorrectionProposal
	{
	public:
		NewCUCompletionProposal(std::wstring& _fileName,
		std::wstring& _projrctName,
		std::wstring& _packageName):fileName(_fileName),projrctName(_projrctName),packageName(_packageName)
		{
			
		}

		CorretioinType GetType() override {
			return  NEWCU_TYPE;
		}
		std::wstring fileName;
		std::wstring projrctName;
		std::wstring packageName;

	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

