#include "BreakCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		BreakCompletionProposal::BreakCompletionProposal(int access, double  _relevance, JavaModel::ISourceRange* completion_rang): JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}

		const wstring& BreakCompletionProposal::GetDisplayString()
		{

			if(fDisplayString.empty())
			{
				if(otheTag == NORMAL)
				{
					fDisplayString = L"break";
				}
				else
				{
					fDisplayString = L"break [cursor];";
				}
			}
			return  fDisplayString;
		}

		const wstring& BreakCompletionProposal::GetReplacementString()
		{
			
				if (fReplacementString.empty())
				{
					fReplacementString = L"break ";
					cursorLenth = fReplacementString.size();
					if (otheTag != NORMAL)
					{
						fReplacementString = L"break ;";
					}
				}
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

