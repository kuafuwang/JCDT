
#ifndef RenameCUCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define RenameCUCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class RenameCUCompletionProposal :public CorrectionProposal
	{
	public:
		RenameCUCompletionProposal(std::wstring& _fileName,
		std::wstring& _projectName,
		std::wstring& _packageName):fileName(_fileName), projectName(_projectName),packageName(_packageName)
		{
			
		}

		CorretioinType GetType() override {
			return  RENAME_CU;
		}
		std::wstring fileName;
		std::wstring oldFileName;
		std::wstring projectName;
		std::wstring packageName;
		std::wstring path;

	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

