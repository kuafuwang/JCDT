#ifndef Jikes_Jikes_AssistResult_INCLUDED
#define Jikes_Jikes_AssistResult_INCLUDED
#include <string>
#include <vector>

#include "AssistInfo.h"

namespace Jikes { // Open namespace Jikes block
	class Ast;
	namespace Correction
	{
		class CorrectionProposal;
	}
	namespace JavaModel {

		enum IJavaElementType;
		class ISourceRange;
	}
	class AccessFlags;
namespace CodeAssist{
	class AssistInfo;
	class AssistExpression;
	class IJavaCompletionProposal;
	class ResultCollector;
	class AssistResult{
	public:
		AssistInfo* info = nullptr;
		virtual ~AssistResult();
	};

	class ResultForTips :public AssistResult {
	public:
		wstring tipsInfo ;
		wstring filePath;
		JavaModel::ISourceRange* rng;
		ResultForTips();
		virtual ~ResultForTips();
	};

	struct GoToDeclItem
	{
		GoToDeclItem(wstring& lpsz_item_name,
		             Jikes::JavaModel::IJavaElementType i_type,
		             const Jikes::AccessFlags& _access,
		             const wstring& file_path,
		             JavaModel::ISourceRange* _rng,
		             bool _binary);

		~GoToDeclItem();
		wstring lpszItemName;
		Jikes::JavaModel::IJavaElementType  iType;
		Jikes::AccessFlags* access;
		wstring filePath;
		JavaModel::ISourceRange* rng;
		bool is_binary;
	};
	class ResultForGoTodecl :public AssistResult {
	public:
		std::vector<GoToDeclItem*> items;
		virtual ~ResultForGoTodecl()
		{
			for(auto it : items)
			{
				delete it;
			}
		}


	};

	class ResultForDebug :public AssistResult
	{
	public:

		std::wstring expr_string_;
		AssistExpression* expr = nullptr;
		~ResultForDebug();

		AssistExpression* TransferExpr(){
			auto temp = expr;
			expr = nullptr;
			return temp;
		}
	};
	class ResultForScope :public AssistResult
	{
	public:

		JavaModel::ISourceRange*  range;
		~ResultForScope();

		JavaModel::ISourceRange* TransferRange() {
			auto temp = range;
			range = nullptr;
			return temp;
		}
	};
	class ResultCodeComplete :public AssistResult {
	public:
		int lenEntered;
		ResultCodeComplete();
		ResultCollector* collector;
		virtual ~ResultCodeComplete();
	};
	class ResultForCorrectionError :public AssistResult {
	public:
		wchar_t* pTipsInfo = nullptr;
		std::vector<Correction::CorrectionProposal*> proposals;
		std::vector<MiniError*> errorInfo;
		virtual ~ResultForCorrectionError();
	};
	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

