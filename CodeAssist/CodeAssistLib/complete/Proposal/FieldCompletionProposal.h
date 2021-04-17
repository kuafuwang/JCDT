
#ifndef FieldCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#define FieldCompletionProposal_Jikes_CodeAssist_INCLUDED_INCLUDED
#include "JavaCompletionProposal.h"

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	class FieldCompletionProposal : public JavaCompletionProposal
	{
	public:
		FieldCompletionProposal(int access, double  _relevance, 
			JavaModel::ISourceRange* completion_rang,
			const wstring& declaring_type_package_name,
			const wstring& declaring_type_name, 
			const wstring& name,
			const wstring& type_package_name,
			const wstring& type_name, 
			const wstring& completion_name);

		const wstring& GetDisplayString() override;
		const wstring& GetReplacementString() override;
		wstring declaringTypePackageName; 
		 wstring declaringTypeName; 
		 wstring name;
		 wstring typePackageName;  
		 wstring typeName; 
		 wstring completionName; 
		 const wchar_t* GetTokenFroMatch() override {
			 return name.c_str();
		 }
	};


}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

