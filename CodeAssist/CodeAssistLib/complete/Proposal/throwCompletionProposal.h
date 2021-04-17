
#ifndef ThrowCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define ThrowCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class ThrowCompletionProposal : public JavaCompletionProposal
	{
	public:
		ThrowCompletionProposal(  int access, double  _relevance, JavaModel::ISourceRange* completion_rang)
			: JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString();
		const wchar_t* GetTokenFroMatch() override {
			return StringConstant::US_throw;
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

