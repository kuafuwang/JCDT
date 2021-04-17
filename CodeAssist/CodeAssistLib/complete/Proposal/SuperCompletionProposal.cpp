#include "SuperCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& SuperCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"super";
				}
				else
				{
					fDisplayString = L"super( [cursor] );";
				}
			}
			return  fDisplayString;
		}
		const wstring& SuperCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
					fReplacementString = L"super";
					cursorLenth = fReplacementString.size();
				
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

