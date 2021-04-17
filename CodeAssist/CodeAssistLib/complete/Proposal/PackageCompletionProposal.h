
#ifndef PackageCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define PackageCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class PackageCompletionProposal : public JavaCompletionProposal
	{
	public:
		PackageCompletionProposal(
			const wstring& replaceMentString, 
			int access, 
			double  _relevance,
			JavaModel::ISourceRange* completion_rang, 
			const wstring& package_name);


		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring packageName;
		enum
		{
			
		};
		const wchar_t* GetTokenFroMatch() override {
			return fReplacementString.c_str();
		}
	};

}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

