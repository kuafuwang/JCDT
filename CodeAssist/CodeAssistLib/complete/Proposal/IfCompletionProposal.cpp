#include "IfCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& IfCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"if";
				}
				else if(otheTag == WITH_BRACKETS)
				{
					fDisplayString = L"if( [cursor] ){}";
				}
				else
				{
					fDisplayString = L"if( [cursor] ){}else{}";
				}
			}
			return  fDisplayString;
		}

		const wstring& IfCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				if (otheTag == NORMAL)
				{
					fReplacementString = L"if";
					cursorLenth = fReplacementString.size();
				}
				else if (otheTag == WITH_BRACKETS)
				{
					fReplacementString = L"if(){\n\n";
					int len = 0;
					if(completionRang)
					{
						len = completionRang->m_nStartIndex - 1;
					}
					for (auto i = 0; i < len ; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString.push_back('}');
					cursorLenth = 3;
				}
				else
				{
					int len = 0;
					if (completionRang)
					{
						len = completionRang->m_nStartIndex - 1;
					}
					fReplacementString = L"if(){\n\n";
					for (auto i = 0; i < len; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString = L"}\n";
					fReplacementString.append(L"else{\n\n");
					for (auto i = 0; i < len; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString.push_back('}');
					cursorLenth = 3;
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

