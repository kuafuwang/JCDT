
#ifndef CaseCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define CaseCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class CaseCompletionProposal : public JavaCompletionProposal
	{
	public:
		CaseCompletionProposal(int access, double  _relevance, JavaModel::ISourceRange* completion_rang);
		const wstring& GetDisplayString();
		const wstring& GetReplacementString();
		const wchar_t* GetTokenFroMatch() override {
			return StringConstant::US_case;
		}
		enum
		{
			NORMAL,
			WITH_BRACKETS
		};

	};


}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

