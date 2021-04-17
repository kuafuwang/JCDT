#include "DoCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& DoCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"do";
				}
				else
				{
					fDisplayString = L"do{}while([cursor])";
				}
			}
			return  fDisplayString;
		}
		const wstring& DoCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				if (otheTag == NORMAL)
				{
					fReplacementString = L"do ";
					cursorLenth = fReplacementString.size();
				}
				else
				{
					int len = 0;
				    if(completionRang)
				    {
						len = completionRang->m_nStartIndex;
				    }
					fReplacementString = L"do{\n\n";
					
					for (auto i = 0; i < len -1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString += L"}while();";
					cursorLenth = fReplacementString.size() -2;
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

