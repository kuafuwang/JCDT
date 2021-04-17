#include "PackageCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		PackageCompletionProposal::PackageCompletionProposal(const wstring& replaceMentString, 
			int access, double  _relevance, JavaModel::ISourceRange* completion_rang,
			const wstring& package_name)
			: JavaCompletionProposal(PacakgeProposal, access, _relevance, completion_rang),
		                                                                                                                                                                                           packageName(package_name)
		{
			fReplacementString = (replaceMentString);

		}

		const wstring& PackageCompletionProposal::GetDisplayString()
		{
			if(fDisplayString.empty())
			{
				fDisplayString = packageName;
				if(fReplacementString.find(L".*") != wstring::npos){
					fDisplayString += L".*;";
				}
			}
			return fDisplayString;
		}

		const wstring& PackageCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())	
			 fReplacementString = packageName;

			return fReplacementString;

		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

