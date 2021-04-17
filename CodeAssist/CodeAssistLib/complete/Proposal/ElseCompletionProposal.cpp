#include "ElseCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& ElseCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"else";
				}
				else if(otheTag == WITH_BRACKETS)
				{
					fDisplayString = L"else { [cursor] }";
				}
				else
				{
					fDisplayString = L"else if([cursor]){}";
				}
			}
			return  fDisplayString;
		}

		const wstring& ElseCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				if (otheTag == NORMAL)
				{
					fReplacementString = L"else";
					cursorLenth = fReplacementString.size();
				}
				else if (otheTag == WITH_BRACKETS)
				{
					
					int len = 0;
					if(completionRang)
					{
						len  = completionRang->m_nStartIndex;
					}
					fReplacementString = L"else{\n";
					cursorLenth = fReplacementString.size() - 1;
					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString += L"    \n";
					cursorLenth = fReplacementString.size() - 1;
					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString += L"}";
					
				}
				else
				{
					
					int len = 0;
					if (completionRang)
					{
						len = completionRang->m_nStartIndex;
					}
					fReplacementString = L"else if(){\n\n";
					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					
					fReplacementString += L"}";

					cursorLenth = 8;

				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

