#include "ContinueCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& ContinueCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"continue;";
				}
				else
				{
					fDisplayString = L"continue [cursor];";
				}
			}
			return  fDisplayString;
		}
		const wstring& ContinueCompletionProposal::GetReplacementString()
		{
		
			if (fReplacementString.empty())
			{
			
				if (otheTag == NORMAL)
				{
					fReplacementString = L"continue;";
					cursorLenth = fReplacementString.size();
				}
				else
				{
					fReplacementString = L"continue ;";
					cursorLenth = fReplacementString.size() - 1;
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

