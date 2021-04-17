
#ifndef UnresolvedElementsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#define UnresolvedElementsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#include <vector>

namespace Jikes {
	class JikesError;
}
namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class ProblemPosition;
	class CorrectionProposal;
	class UnresolvedElementsSubProcessor
	{
	public:

		 //static SimpleName getVariableNode(ASTNode selectedNode);
		 static void getVariableProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		 static void getTypeProposals(ProblemPosition* problemPos,
			 int kind, std::vector<CorrectionProposal*>& proposals);
		 static void getMethodProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);
	
	//	 static void getConstructorProposals(ITypeBinding typeBinding, List arguments, ICompilationUnit cu, ArrayList proposals);
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

