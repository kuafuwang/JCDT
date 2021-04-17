#include "AnomymousCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		AnomymousCompletionProposal::AnomymousCompletionProposal(int access, double  _relevance,
			JavaModel::ISourceRange* completion_rang, const wstring& super_type_package_name, 
			const wstring& super_type_name, const std::vector<wstring>& parameter_package_names, const std::vector<wstring>& parameter_type_names, 
			const std::vector<wstring>& basic_strings, const wstring& completion_name)
			: JavaCompletionProposal(AnonymousTypeProposal, access, _relevance, completion_rang),
		    superTypePackageName(super_type_package_name),
		    superTypeName(super_type_name),
		    parameterPackageNames(parameter_package_names),
		    parameterTypeNames(parameter_type_names),
			parameterNames(basic_strings),
		    completionName(completion_name)
		{
		}

		const wstring& AnomymousCompletionProposal::GetDisplayString()
		{
		
			return  fDisplayString;
		}

		const wstring& AnomymousCompletionProposal::GetReplacementString()
		{
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

