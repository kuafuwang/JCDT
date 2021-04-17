#include "MethodCompletionProposal.h"
#include <CodeAssistLib/impl/CodeAssistUtil.h>

namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		MethodCompletionProposal::MethodCompletionProposal(int access, double  _relevance, 
			JavaModel::ISourceRange* completion_rang,
			const wstring& declaring_type_package_name, 
			const wstring& declaring_type_name,
			const wstring& selector,
			const vector<wstring>& parameter_package_names,
			const vector<wstring>& parameter_type_names, 
			const vector<wstring>& basic_strings,
			const wstring& return_type_package_name, 
			const wstring& return_type_name, const wstring& completion_name)
			: JavaCompletionProposal(MethodProposal, access, _relevance, completion_rang),
			declaringTypePackageName(declaring_type_package_name),
			declaringTypeName(declaring_type_name),
			selector(selector),
			parameterPackageNames(parameter_package_names),
			parameterTypeNames(parameter_type_names),
			parameterNames(basic_strings),
			returnTypePackageName(return_type_package_name),
			returnTypeName(return_type_name),
			completionName(completion_name)
		{

		}

		const wstring& MethodCompletionProposal::GetDisplayString()
		{
			if (fDisplayString.empty())
			{
				
				{
					wstring _ret(L"(");
					int  size_paramenterNames = parameterNames.size();
					if (size_paramenterNames)
					{
						size_paramenterNames = size_paramenterNames - 1;
					}
					else
					{
						size_paramenterNames = -1;
					}
					auto size_formal_parameters = parameterTypeNames.size();
					for (int i = 0, size = size_formal_parameters; i < size; ++i)
					{

						_ret.append(parameterTypeNames[i]);

						if (i <= size_paramenterNames) {
							_ret.append(L" ").append(parameterNames[i]);
						}

						if (i != size - 1) {
							_ret.push_back(',');
						}
					}
					_ret.append(L" )");
					fDisplayString = selector  + _ret +   L"  " + returnTypeName + L"     in " + declaringTypeName;

				}
			}
	
			return  fDisplayString;
		}

		const wstring& MethodCompletionProposal::GetReplacementString()
		{
			if (fReplacementString.empty())
			{
				
				int i = 0, size = parameterTypeNames.size();
				wstring _ret(L"(");
				if(size)
				{
					cursorLenth = selector.size() + 1;
					_ret.append(L")");
					if(returnTypeName.empty() || returnTypeName == L"void")
					{
						_ret.append(L";");
					}
					
					fReplacementString = selector + _ret;
				}
				else
				{
					_ret.append(L")");
					if (returnTypeName.empty() ||  returnTypeName == L"void")
					{
						_ret.append(L";");
					}
					fReplacementString = selector + _ret;
					cursorLenth = fReplacementString.size();

				}
			
				
			}
				
			return  fReplacementString;
		}

		wstring MethodCompletionProposal::GetInfoFotTip()
		{
			auto size_formal_parameters = parameterTypeNames.size();
			if (!size_formal_parameters)
				return wstring();

			wstring _ret(L"(");
			int  size_paramenterNames = parameterNames.size();
			if (size_paramenterNames)
			{
				size_paramenterNames = size_paramenterNames - 1;
			}
			else
			{
				size_paramenterNames = -1;
			}
		
			for (int i = 0, size = size_formal_parameters; i < size; ++i)
			{
				_ret.append(parameterTypeNames[i]);

				if (i <= size_paramenterNames) {
					_ret.append(L" ").append(parameterNames[i]);
				}

				if (i != size - 1) {
					_ret.push_back(',');
				}
			}
			_ret.append(L" )");
			return _ret;
		}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

