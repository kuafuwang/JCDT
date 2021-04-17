
#ifndef TypeCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define TypeCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class TypeCompletionProposal : public JavaCompletionProposal
	{

	public:
		TypeCompletionProposal(
			ProposalType i_type, int access, double  _relevance, 
			JavaModel::ISourceRange* completion_rang, 
			const wstring& package_name, const wstring& type_name);

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring packageName;
		wstring typeName;
		const wchar_t* GetTokenFroMatch() override {
			return typeName.c_str();
		}

	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

