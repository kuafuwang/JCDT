
#ifndef InsertCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define InsertCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"
#include <string>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	class InsertCorrectionProposal :public CorrectionProposal
	{
	public:
		int left_line_no;
		int left_column_no;
		std::wstring fInsertionString;
		InsertCorrectionProposal(
			int _left_line_no,
			int _left_column_no,
			const std::wstring& _fInsertionString
		) :left_line_no(_left_line_no),
			left_column_no(_left_column_no),
			fInsertionString(_fInsertionString)
		{

		}

		CorretioinType GetType() override {
			return INSERT_TYPE;
		}
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

