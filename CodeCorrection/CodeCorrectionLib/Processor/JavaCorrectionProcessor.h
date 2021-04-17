
#ifndef JavaCorrectionProcessor_Jikes_Correction_INCLUDED_INCLUDED
#define JavaCorrectionProcessor_Jikes_Correction_INCLUDED_INCLUDED
#include <vector>
using std::vector;

namespace Jikes { // Open namespace Jikes block

	class JikesError;

namespace Correction{
	class ProblemPosition;
	class CorrectionProposal;
	class JavaCorrectionProcessor
	{
		
	public:
		static bool hasCorrections(int problemId);

		static void collectCorrections(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		static bool UnusedImportProblem(int problemId);
	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

