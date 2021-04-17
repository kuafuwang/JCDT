#include "ClassCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		const wstring& ClassCompletionProposal::GetDisplayString()
		{

			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"class";
				}
				else if(otheTag == WITH_BRACKETS)
				{
					fDisplayString = L"class [cursor] extends x{}";
				}
				else
				{
					fDisplayString = L"class [cursor] {}";

				}
			}
			return  fDisplayString;
		}
		const wstring& ClassCompletionProposal::GetReplacementString()
		{

			if (fReplacementString.empty())
			{
				fReplacementString = L"class ";
				cursorLenth = fReplacementString.size();
				if (otheTag == NORMAL)
				{
					
				}
				else if (otheTag == WITH_BRACKETS)
				{
					
					fReplacementString = L"class  extends x{\n\n";

					int len = 0;
					if(completionRang)
					{
						len = completionRang->m_nStartIndex;
					}

					for(auto i = 0 ; i < len ; ++i)
					{
						fReplacementString.push_back(' ');
					}
					fReplacementString.push_back('}');
				}
				else
				{
					fReplacementString = L"class {\n\n";
			
					int len = 0;
					if (completionRang)
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

