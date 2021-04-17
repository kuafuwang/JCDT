#include "SuperCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		const wstring& SuperCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				if (otheTag == NORMAL)
				{
					fDisplayString = L"super";
				}
				else
				{
					fDisplayString = L"super( [cursor] );";
				}
			}
			return  fDisplayString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

