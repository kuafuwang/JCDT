
#ifndef MethodCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define MethodCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class MethodCompletionProposal : public JavaCompletionProposal
	{

	public:
		MethodCompletionProposal(int access, double  _relevance, JavaModel::ISourceRange* completion_rang, const wstring& declaring_type_package_name, const wstring& declaring_type_name, const wstring& selector, const std::vector<wstring>& parameter_package_names, const std::vector<wstring>& parameter_type_names, const std::vector<wstring>& basic_strings, const wstring& return_type_package_name, const wstring& return_type_name, const wstring& completion_name);

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring GetInfoFotTip() override;
		wstring declaringTypePackageName;
		wstring declaringTypeName;
		wstring selector;
		std::vector<wstring> parameterPackageNames;
		std::vector<wstring> parameterTypeNames;
		std::vector<wstring> parameterNames;
		wstring returnTypePackageName;
		wstring returnTypeName;
		wstring completionName;
		const wchar_t* GetTokenFroMatch() override {
			return selector.c_str();
		}
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

