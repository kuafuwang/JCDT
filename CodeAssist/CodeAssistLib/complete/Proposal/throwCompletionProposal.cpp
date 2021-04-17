#include "ThrowCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& ThrowCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"throw ";
					
				}
				else
				{
					fDisplayString = L"throw [cursor];";
				}
			}
			return  fDisplayString;
		}
		const wstring& ThrowCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				fReplacementString = L"throw ";
				cursorLenth = fReplacementString.size();
				if (otheTag == NORMAL)
				{

				}
				else
				{
					fReplacementString = L"throw ;";
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

