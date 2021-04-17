
#ifndef JavaCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define JavaCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "IJavaCompletionProposal.h"
#include "ICompletionProposalExtension.h"
#include <JCDT_Lib/internal/env/StringConstant.h>
namespace Jikes { // Open namespace Jikes block
	namespace JavaModel {
		class ISourceRange;
	}

namespace CodeAssist{
	
	class JavaCompletionProposal: public IJavaCompletionProposal 
	{
	public:

		JavaCompletionProposal(ProposalType i_type, int access, double  _relevance, JavaModel::ISourceRange* completion_rang);
		~JavaCompletionProposal();
	
		JavaModel::ISourceRange* completionRang;
		int cursorLenth;
		int otheTag;
	};


}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

