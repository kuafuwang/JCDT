#include "FieldCompletionProposal.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

		 FieldCompletionProposal::FieldCompletionProposal(int access, double  _relevance,
			 JavaModel::ISourceRange* completion_rang, const wstring& declaring_type_package_name, 
			 const wstring& declaring_type_name, const wstring& name, const wstring& type_package_name,
			 const wstring& type_name, const wstring& completion_name) 
			 : JavaCompletionProposal(FieldProposal, access, _relevance, completion_rang),
			declaringTypePackageName(declaring_type_package_name),
			declaringTypeName(declaring_type_name),
			name(name),
			typePackageName(type_package_name),
			typeName(type_name),
			completionName(completion_name)
		{

		}

		const wstring& FieldCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				fDisplayString = name + L"    in " + declaringTypeName;
			}
				
			return  fDisplayString;
		}

		const wstring& FieldCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
				fReplacementString = name;
			return  fReplacementString;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

