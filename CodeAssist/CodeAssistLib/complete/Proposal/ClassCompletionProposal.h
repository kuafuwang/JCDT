
#ifndef ClassCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define ClassCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class ClassCompletionProposal : public JavaCompletionProposal
	{

	public:
		ClassCompletionProposal( int access, double  _relevance, JavaModel::ISourceRange* completion_rang)
			: JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)

		{

		}

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString();
		const wchar_t* GetTokenFroMatch() override {
			return StringConstant::US_class;
		}
		enum
		{
			NORMAL,
			WITH_BRACKETS,
			WITH_EXTENDS,
		};

	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

