
#ifndef ProblemPosition_Jikes_Correction_INCLUDED_INCLUDED
#define ProblemPosition_Jikes_Correction_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block

	namespace JavaModel
	{
		class LayerPosInJavaModel;
		class JavaModel;

	}
	namespace CodeAssist
	{
		class AssistInfo;
	}
namespace Correction{
	
	class ProblemPosition
	{
	public:
		ProblemPosition(JavaModel::LayerPosInJavaModel* _layer_pos,
			JavaModel::JavaModel* _pJm,
			CodeAssist::AssistInfo* _cur_info,
			int _problemId,
			int _left_line_no,
			int _left_column_no,
			int _right_line_no,
			int _right_column_no
			):layer_pos(_layer_pos),pJm(_pJm),cur_info(_cur_info),problemId(_problemId),
			left_line_no(_left_line_no),left_column_no(_left_column_no),
			right_line_no(_right_line_no),right_column_no(_right_column_no)
		{
			
		}
		JavaModel::LayerPosInJavaModel* layer_pos ;
		JavaModel::JavaModel* pJm;
		CodeAssist::AssistInfo* cur_info;
		int problemId;
		int left_line_no;
		int left_column_no;
		int right_line_no;
		int right_column_no;
	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

