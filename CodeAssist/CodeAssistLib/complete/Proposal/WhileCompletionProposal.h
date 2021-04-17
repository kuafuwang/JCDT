
#ifndef WhileCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define WhileCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class WhileCompletionProposal : public JavaCompletionProposal
	{
	public:
		WhileCompletionProposal( int access, double  _relevance, JavaModel::ISourceRange* completion_rang)
			: JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}
		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString();


		const wchar_t* GetTokenFroMatch() override {
			return StringConstant::US_while;
		}

		enum
		{
			NORMAL,
			DO_WHILE,
			WITH_BRACKETS
		};
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

