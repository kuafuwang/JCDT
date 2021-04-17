#include  "LocalVariableCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 LocalVariableCompletionProposal::LocalVariableCompletionProposal( int access,
			 double  _relevance, JavaModel::ISourceRange* completion_rang, const wstring& name, const wstring& type_package_name,
			 const wstring& type_name) : JavaCompletionProposal(LocalVariableProposal, access, _relevance, completion_rang),
			name(name),
			typePackageName(type_package_name),
			typeName(type_name)
		{
		}

		 const wstring& LocalVariableCompletionProposal::GetDisplayString()
		 {
			 if (fDisplayString.empty())
				 fDisplayString = name;
			 return  fDisplayString;
		 }

		 const wstring& LocalVariableCompletionProposal::GetReplacementString()
		 {
			 if (fReplacementString.empty())
				 fReplacementString = name;
			 return  fReplacementString;
		 }
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

