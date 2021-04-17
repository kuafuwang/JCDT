
#ifndef ElseCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define ElseCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class ElseCompletionProposal : public JavaCompletionProposal
	{

	public:
		ElseCompletionProposal(int access, double  _relevance, JavaModel::ISourceRange* completion_rang)
			: JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}
		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString();
		const wchar_t* GetTokenFroMatch() override {
			return StringConstant::US_else;
		}
		enum
		{
			NORMAL,
			WITH_BRACKETS,
			WITH_IF
		};
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

