
#ifndef SuperCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define SuperCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class SuperCompletionProposal : public JavaCompletionProposal
	{


	public:

		SuperCompletionProposal(int access, int _relevance, JavaModel::ISourceRange* completion_rang)
			: JavaCompletionProposal(KeywordProposal, access, _relevance, completion_rang)
		{
		}
		const wstring& GetDisplayString() override;
		enum

		{
			NORMAL,
			WITH_BRACKETS
		};
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

