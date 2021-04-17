#ifndef ICompletionProposalExtension_Jikes_CodeAssist_INCLUDED_INCLUDED
#define ICompletionProposalExtension_Jikes_CodeAssist_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		class IDocumentView;
class   ICompletionProposalExtension 
{
public:
	virtual ~ICompletionProposalExtension()
	{
		
	}

	virtual void DisplayApply(IDocumentView* document) = 0;



	virtual void SelectApply(IDocumentView* document) = 0;
	

};

	}// Close namespace Jikes block


} // Close namespace CodeAssist block


#endif // _INCLUDED


