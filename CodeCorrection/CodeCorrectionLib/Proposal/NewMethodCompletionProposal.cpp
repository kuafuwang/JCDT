#include "stdafx.h"
#include "NewMethodCompletionProposal.h"
#include "ReplaceCorrectionProposal.h"
#include <JCDT_Lib/internal/env/StringConstant.h>
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
using  namespace  std;
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		wstring NewMethodCompletionProposal::getModifersString(wchar_t seperator)
		{
			std::vector< std::wstring> modifiers;
			access->GetModifers(modifiers);
			if (modifiers.empty())
				access->GetModifers(modifiers);

			wstring ret;
			for (size_t i = 0, size = modifiers.size(); i < size; ++i) {
				if (i == 0) {
					if(L"abstract" != modifiers[i])
						ret += modifiers[i];
				}
				else {

					if (L"abstract" != modifiers[i]){
						ret += seperator;
						ret += modifiers[i];
					}
				
				}

			}
			return ret;
		}
		std::wstring NewMethodCompletionProposal::getOutPut()
		{
			wstring info;
			
			info.push_back('\n');

			for(auto i = 0 ; i < left_column_no - 1; ++i){
				info.push_back(' ');
			}

			info += getModifersString() + L" ";

			if (!constructor)
				info += returnTypeName + L"  ";
			info += selector + L"(";
			size_t i = 0, size = formal_parameterTypes.size();

			for(;i < size; ++i){
				info += formal_parameterTypes[i] + L"  " + paramenterNames[i] + L",";
			}
			if (size)
				info[info.size() - 1] = ')';
			else
				info.push_back(')');

			if (isInterface)
				info.push_back(';');
			else
			{

				wstring temp;
				for(auto k = 0 ; k < left_column_no - 1; ++k)
					temp.push_back(' ');
				

				info += L"{\n";
				info.reserve(info.size() + left_column_no);
				info += temp;
				if(constructor || returnTypeName == StringConstant::US_void)
				{
					
				}
				else
				{
					if (returnTypeName == StringConstant::US_boolean)
					{
						info += L"return true;";
					}
					else if(returnTypeName == StringConstant::US_byte 
						|| returnTypeName == StringConstant::US_short
						|| returnTypeName == StringConstant::US_int
						|| returnTypeName == StringConstant::US_float
						|| returnTypeName == StringConstant::US_char
						|| returnTypeName == StringConstant::US_double

						){
						info += L"return 0;";
					}
					else{
						info += L"return null;";
					}
					

				}
				info.push_back('\n');
				info += temp;
				info.append(L"}\n");
			}
			return info;
		}

		NewMethodCompletionProposal::NewMethodCompletionProposal(std::wstring& _fileName, std::wstring& _projectName, std::wstring& _packageName)
			: fileName(_fileName), projectName(_projectName), packageName(_packageName), isInterface(false), constructor(false), left_line_no(0), left_column_no(0), right_line_no(0), right_column_no(0)
		{
			access = new AccessFlags();
		}

		NewMethodCompletionProposal::~NewMethodCompletionProposal()
		{
			for (auto it : proposals)
				delete it;
			delete access;
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

