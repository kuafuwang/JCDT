#include "TypeCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
	

		TypeCompletionProposal::TypeCompletionProposal( ProposalType i_type,
			int access, double  _relevance, JavaModel::ISourceRange* completion_rang,
			const wstring& package_name, const wstring& type_name) : JavaCompletionProposal(i_type,access,_relevance,completion_rang) 
			,  packageName(package_name),typeName(type_name){

			
		}

		const wstring& TypeCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				fDisplayString = typeName + L"     in " + packageName;
			}
			
			return  fDisplayString;
		}

		const wstring& TypeCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
				fReplacementString = typeName;
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

