#include "WhileCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 const wstring& WhileCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"while";
				}
				else if(otheTag == WITH_BRACKETS)
				{
					fDisplayString = L"while( [cursor] ){}";
				}
				else
				{
					fDisplayString = L"while( [cursor] );";
				}
			}
			return  fDisplayString;
		}

		 const wstring& WhileCompletionProposal::GetReplacementString()
		 {
			 if (fReplacementString.empty())
			 {
				 fReplacementString = L"while ";
				 cursorLenth = fReplacementString.size();
				 if (otheTag == NORMAL)
				 {

				 }
				 else if (otheTag == WITH_BRACKETS)
				 {
					 fReplacementString = L"while(){\n\n";
					 int len = 0;
					 if (completionRang)
					 {
						 len = completionRang->m_nStartIndex;

					 }
					 for (auto i = 0; i < len - 1; ++i)
					 {
						 fReplacementString.push_back(' ');
					 }
					 fReplacementString.push_back('}');
				 }
				 else
				 {
					 fReplacementString = L"while();";
				 }
			 }
			 return  fReplacementString;
		 }
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

