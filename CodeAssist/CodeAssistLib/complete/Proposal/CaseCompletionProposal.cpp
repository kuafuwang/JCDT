#include "CaseCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 CaseCompletionProposal::CaseCompletionProposal(int access, double  _relevance, 
			 JavaModel::ISourceRange* completion_rang) 
			 : JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}

		 const wstring& CaseCompletionProposal::GetDisplayString()
		 {

			 if (fDisplayString.empty())
			 {
				 fDisplayString = L"case ";
				 cursorLenth = fDisplayString.size();
				 if (otheTag != NORMAL)
				 {
					 fDisplayString = L"case [cursor] :break;";
				 }
			 }
			 return  fDisplayString;
		 }
		 const wstring& CaseCompletionProposal::GetReplacementString()
		 {

			 if (fReplacementString.empty())
			 {
				 fReplacementString = L"case ";
				 cursorLenth = fReplacementString.size();
				 if (otheTag != NORMAL)
				
				 {
					 fReplacementString = L"case ;break;";
				 }
			 }
			 return  fReplacementString;
		 }
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

