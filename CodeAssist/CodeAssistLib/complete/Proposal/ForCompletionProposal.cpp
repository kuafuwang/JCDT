#include "ForCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& ForCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"for";
				}
				else
				{
					fDisplayString = L"for( [cursor] ;;){}";
				}
			}
			return  fDisplayString;
		}
		const wstring& ForCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				if (otheTag == NORMAL)
				{
					fReplacementString = L"for";
				}
				else
				{
				
					int len = 0;
					if (completionRang)
					{
						len = completionRang->m_nStartIndex;
					}
					fReplacementString = L"for( ;;){\n\n";

					for (auto i = 0; i < len - 1; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString.push_back('}');
					cursorLenth = 5;
				}
			}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

