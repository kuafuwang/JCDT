
#ifndef NewMethodCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#define NewMethodCompletionProposal_Jikes_Correction_INCLUDED_INCLUDED
#include "CorrectionProposal.h"
#include <vector>
#include <string>



namespace Jikes { // Open namespace Jikes block

	class AccessFlags;

namespace Correction{
	class ReplaceCorrectionProposal;

	class NewMethodCompletionProposal :public CorrectionProposal
	{
	public:
		std::vector<ReplaceCorrectionProposal*> proposals;
		CorretioinType GetType() override {
			return  NEW_METHOD_TYPE;
		}

		std::wstring getModifersString(wchar_t seperator = ' ');
		std::wstring getOutPut();
		NewMethodCompletionProposal(std::wstring& _fileName,
		                            std::wstring& _projectName,
		                            std::wstring& _packageName);

		~NewMethodCompletionProposal();
		AccessFlags* access;
		std::wstring fileName;
		std::wstring oldFileName;
		std::wstring projectName;
		std::wstring packageName;
		std::wstring path;
		std::wstring selector;
	
		bool isInterface;
		std::vector<std::wstring> formal_parameterTypes;
		std::vector<std::wstring>  paramenterNames;
		bool constructor;
		std::wstring returnTypePackage;
		std::wstring returnTypeName;
		int left_line_no;
		int left_column_no;
		int right_line_no;
		int right_column_no;
	};

}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

