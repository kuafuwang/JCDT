#include "SwitchCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& SwitchCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"break";
				}
				else
				{
					fDisplayString = L"switch( [cursor] ){}";
				}
			}
			return  fDisplayString;
		}

		const wstring& SwitchCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				fReplacementString = L"switch";
				cursorLenth = fReplacementString.size();
				if (otheTag == NORMAL)
				{
					
				}
				else
				{
					cursorLenth = cursorLenth + 1;
					fReplacementString = L"switch(){\n\n";
					int len = 0;
					if(completionRang)
					{
						len = completionRang->m_nStartIndex - 1;
					}
					 
					for (auto i = 0; i < len; ++i)
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

