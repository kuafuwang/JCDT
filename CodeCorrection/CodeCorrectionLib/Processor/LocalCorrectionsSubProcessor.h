
#ifndef LocalCorrectionsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#define LocalCorrectionsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#include <vector>
namespace Jikes { // Open namespace Jikes block
	class ProblemPosition;


namespace Correction{
	class CorrectionProposal;
	class ProblemPosition;
	class LocalCorrectionsSubProcessor
	{
	public:
		 static void addCastProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		 static void addUncaughtExceptionProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		 static void addMethodWithConstrNameProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		 static void addVoidMethodReturnsProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		 static void addMissingReturnTypeProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

