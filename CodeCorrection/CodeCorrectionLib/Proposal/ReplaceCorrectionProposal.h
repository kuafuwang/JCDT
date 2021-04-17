
#ifndef ReplaceCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define ReplaceCorrectionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class ReplaceCorrectionProposal :public CorrectionProposal
	{
	public:
		int left_line_no;
		int left_column_no;
		int right_line_no;
		int right_column_no;
		std::wstring fReplacementString;
		ReplaceCorrectionProposal(
			int _left_line_no,
			int _left_column_no,
			int _right_line_no,
			int _right_column_no
		) :
			left_line_no(_left_line_no), left_column_no(_left_column_no),
			right_line_no(_right_line_no), right_column_no(_right_column_no)
		{

		}
		ReplaceCorrectionProposal* Clone() const
		{
			auto _ret =  new ReplaceCorrectionProposal(left_line_no, left_column_no, right_line_no, right_column_no);
			_ret->fReplacementString = fReplacementString;
			_ret->fDisplayString = fDisplayString;
			return _ret;
		}
		CorretioinType GetType() override {
			return  REPLACE_TYPE;
		}
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

