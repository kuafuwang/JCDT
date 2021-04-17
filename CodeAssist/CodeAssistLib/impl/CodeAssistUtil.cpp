#include "stdafx.h"

#include "CodeAssistUtil.h"
//#include <JCDT_Lib/internal/env/StringConstant.h>
using std::wstring;
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {


		std::vector<wstring>    CodeAssistUtil::m_lszpBasicTypes =
		{
		  L"byte",
		  L"short",
		  L"char",
		  L"long",
		  L"float",
		  L"double",
		  L"boolean",
		  L"int"
		};

		std::vector<wstring>    CodeAssistUtil::m_lszpPostFixOps =
		{
		  L"++",
		  L"--"
		};



		std::vector<wstring>   CodeAssistUtil::m_lszpInfixOps =
		{
		  L"||",
		  L"&&",
		  L"|",
		  L"^",
		  L"&",
		  L"==",
		  L"!=",
		  L"<",
		  L">",
		  L"<=",
		  L">=",
		  L"<<",
		  L">>",
		  L">>>",
		  L"+",
		  L"-",
		  L"*",
		  L"/",
		  L"%"
		};



		std::vector<wstring>   CodeAssistUtil::m_lszpPrefixOps =
		{
		  L"++",
		  L"--",
		  L"!",
		  L"~",
		  L"+",
		  L"-",
		};

		std::vector<wstring>    CodeAssistUtil::m_lszpAssignmentOps =
		{
		  L"=",
		  L"+=",
		  L"-=",
		  L"*=",
		  L"/=",
		  L"&=",
		  L"|=",
		  L"^=",
		  L"%=",
		  L"<<=",
		  L">>=",
		  L">>>="
		};

		std::vector<wstring> m_lszpOperators =
		{
		  L"=",
		  L"+",
		  L"-",
		  L"*",
		  L"/",
		  L"&",
		  L"|",
		  L"^",
		  L"%",
		  L"<<",
		  L">>",
		  L">>>",
		  L"!",
		  L">",
		  L"<",
		  L"~",
		  L"?",
		  L":",
		};


		std::vector<wstring>   CodeAssistUtil::m_lszpModifiers =
		{
		  L"public",
		  L"protected",
		  L"private",
		  L"static",
		  L"abstract",
		  L"final",
		  L"native",
		  L"synchronized",
		  L"transient",
		  L"volatile",
		  L"strictfp"
		};
		std::vector<wstring>   CodeAssistUtil::m_lszpJavaKeyword =
		{
			L"abstract",
			L"default",
			L"if",
			L"private",
			L"this",
			L"boolean",
			L"do",
			L"implements",
			L"protected",
			L"throw",
			L"break",
			L"double",
			L"import",
			L"public",
			L"throws",
			L"byte",
			L"else",
			L"instanceof",
			L"return",
			L"transient",
			L"case",
			L"extends",
			L"long",
			L"short",
			L"try",
			L"catch",
			L"final",
			L"interface",
			L"static",
			L"void",
			L"char",
			L"finally",
			L"long",
			L"strictfp",
			L"volatile",
			L"class",
			L"float",
			L"native",
			L"super",
			L"while",
			L"const",
			L"for",
			L"new",
			L"switch",
			L"continue",
			L"goto",
			L"package",
			L"synchronized",
			L"int",
			L"null"
		};

		
		std::vector<wstring>    CodeAssistUtil::m_lszpFIRSTexpr =
		{
			L"(",
			L"this",
			L"super",
			L"new",
			L"void",
		};

		

		bool CodeAssistUtil::IsInArray(const wstring& strKeyWord, const wchar_t** strArray)
		{
			for (int I = 0; strArray[I] != nullptr; I++)
				if (strKeyWord == (strArray[I]))
					return true;

			return false;
		}

		bool CodeAssistUtil::IsInArray(const std::wstring& strKeyWord, vector<wstring>& strArray)
		{
			auto data = strArray.data();
			auto size = strArray.size();
			for (int I = 0; I < size; I++)
				if (strKeyWord == (data[I]))
					return true;

			return false;
		}

		bool CodeAssistUtil::IsTokenThatMatchCodeAssist(const wstring& strKeyWord)
		{
			if (strKeyWord == L"this")
			{
				return true;
			}
			else if (strKeyWord == L"super")
			{
				return true;
			}
			if (CodeAssistUtil::IsInArray(strKeyWord, CodeAssistUtil::m_lszpJavaKeyword))
			{
				return  false;
			}
			if (CodeAssistUtil::IsInArray(strKeyWord, CodeAssistUtil::m_lszpBasicTypes))
			{
				return  false;
			}
			

			if (strKeyWord == L"null"
				|| strKeyWord == L"true"
				|| strKeyWord == L"false")
				return false;

			return	true;
		}
				bool CodeAssistUtil::IsReservedJavaToken(const wstring& strKeyWord)
		{
			if (CodeAssistUtil::IsInArray(strKeyWord, CodeAssistUtil::m_lszpJavaKeyword))
			{
				return  true;
			}
			if (CodeAssistUtil::IsInArray(strKeyWord, CodeAssistUtil::m_lszpBasicTypes))
			{
				return  true;
			}
			if (CodeAssistUtil::IsInArray(strKeyWord, CodeAssistUtil::m_lszpFIRSTexpr))
			{
				return  true;
			}

			if (strKeyWord == L"null"
				|| strKeyWord == L"true"
				|| strKeyWord == L"false")
				return true;

			return	false;
		}
	}
}