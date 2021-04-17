
#ifndef LocalVariableCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define LocalVariableCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class LocalVariableCompletionProposal : public JavaCompletionProposal
	{
	public:

		LocalVariableCompletionProposal(int access, double  _relevance,
		                                JavaModel::ISourceRange* completion_rang,
		                                const wstring& name,
		                                const wstring& type_package_name,
		                                const wstring& type_name);


		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring name;
		wstring typePackageName;
		wstring typeName;
		const wchar_t* GetTokenFroMatch() override {
			return name.c_str();
		}
	};

	
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

