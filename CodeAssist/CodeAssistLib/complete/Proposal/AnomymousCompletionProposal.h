
#ifndef AnomymousCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define AnomymousCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class AnomymousCompletionProposal : public JavaCompletionProposal
	{

	public:
		AnomymousCompletionProposal(int access, double  _relevance, 
			JavaModel::ISourceRange* completion_rang,
			const wstring& super_type_package_name,
			const wstring& super_type_name,
			const std::vector<wstring>& parameter_package_names,
			const std::vector<wstring>& parameter_type_names, 
			const std::vector<wstring>& basic_strings, 
			const wstring& completion_name);

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring superTypePackageName;
		 wstring superTypeName;
		std::vector<wstring> parameterPackageNames;
		std::vector<wstring> parameterTypeNames;
		std::vector<wstring> parameterNames;
		 wstring completionName;
		 const wchar_t* GetTokenFroMatch() override {
			 return completionName.c_str();
		 }
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

