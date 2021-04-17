#include "TryCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& TryCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"try";
				}
				else
				{
					fDisplayString = L"try{} catch([cursor]){}";
				}
			}
			return  fDisplayString;

		}
		const wstring& TryCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				if (otheTag == NORMAL)
				{
					fReplacementString = L"try ";
					cursorLenth = fReplacementString.size();
				}
				else
				{
					auto len = 0;
				    if(completionRang)
				    {
						len = completionRang->m_nStartIndex;
				    }
					
					fReplacementString = L"try{\n\n";

					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString += L"}catch(){\n\n";
					cursorLenth = fReplacementString.size() -4;
					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString.push_back('}');
				}
			}
			return  fReplacementString;

		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

