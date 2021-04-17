
#include "JavaCompletionProposal.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		JavaCompletionProposal::JavaCompletionProposal(ProposalType i_type, 
			int access, double  _relevance, JavaModel::ISourceRange* completion_rang)
			: IJavaCompletionProposal(i_type, access, _relevance),
			  completionRang(completion_rang), cursorLenth(0), otheTag(0)
		{

		}

		JavaCompletionProposal::~JavaCompletionProposal()
		{
		
			delete completionRang;
		}

	
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

