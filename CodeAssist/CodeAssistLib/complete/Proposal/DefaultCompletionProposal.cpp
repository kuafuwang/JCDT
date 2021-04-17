#include "DefaultCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& DefaultCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"default:";
				}
				else if(WITH_BREAK == otheTag)
				{
					fDisplayString = L"default: break;";
				}
				else
				{
					fDisplayString = L"default : return [cursor];";
				}
			}
			return  fDisplayString;
		}

		const wstring& DefaultCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				
				if (otheTag == NORMAL)
				{
					
					fReplacementString = L"default:";
					cursorLenth = fReplacementString.size()+1;
				}
				else if (WITH_BREAK == otheTag)
				{
					
					fReplacementString = L"default: break;";
				
					cursorLenth = fReplacementString.size() + 1;
				}
				else
				{
					fReplacementString = L"default : return ;";
					cursorLenth = fReplacementString.size() - 1;
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

