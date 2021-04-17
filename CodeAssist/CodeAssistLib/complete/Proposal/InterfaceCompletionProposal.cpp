#include "InterfaceCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		const wstring& InterfaceCompletionProposal::GetDisplayString()
		{

			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"interface";
				}
				else if(otheTag == WITH_BRACKETS)
				{
					fDisplayString = L"interface  [cursor] implements x{}";
				}
				else
				{
					fDisplayString = L"interface [cursor] {}";
				}
			}
			return  fDisplayString;
		}
		const wstring& InterfaceCompletionProposal::GetReplacementString()
		{

			if (fReplacementString.empty())
			{
				fReplacementString = L"interface ";
				cursorLenth = fReplacementString.size();
				if (otheTag == NORMAL)
				{

				}
				else if (otheTag == WITH_BRACKETS)
				{

					fReplacementString = L"interface  implements x{\n\n";
					int len = 0;
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
				else
				{
					fReplacementString = L"interface {\n\n";
					int len = 0;
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

