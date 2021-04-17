#include "SynchronizedCompletionProposalcpp.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& SynchronizedCompletionProposalcpp::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"synchronized";
				}
				else
				{
					fDisplayString = L"synchronized([cursor]){}";
				}
			}
			return  fDisplayString;
		}
		const wstring& SynchronizedCompletionProposalcpp::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				fReplacementString = L"synchronized ";
				cursorLenth = fReplacementString.size();
				if (otheTag == NORMAL)
				{

				}
				else
				{
					fReplacementString = L"synchronized(){\n\n";
					auto len = 0;
					if(completionRang)
					{
						len = completionRang->m_nStartIndex;
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

