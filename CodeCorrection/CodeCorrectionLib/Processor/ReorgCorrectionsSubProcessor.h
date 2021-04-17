
#ifndef ReorgCorrectionsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#define ReorgCorrectionsSubProcessor_Jikes_Correction_INCLUDED_INCLUDED
#include <vector>



namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class ProblemPosition;
	
	class CorrectionProposal;
	class ReorgCorrectionsSubProcessor
	{
	public:
		 static void getWrongTypeNameProposals(ProblemPosition* problemPos , std::vector<CorrectionProposal*>& proposals);


		 static void get_NON_ABSTRACT_TYPE_INHERITS_ABSTRACT_METHOD_Proposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);


		 static void getWrongPackageDeclNameProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);

		
		 static void removeImportStatementProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);
		void getMethodOverLoadProposals(ProblemPosition* problemPos, std::vector<CorrectionProposal*>& proposals);
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

