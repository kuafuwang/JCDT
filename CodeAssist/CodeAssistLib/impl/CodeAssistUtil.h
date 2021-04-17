#include <string>
#include <vector>
#include <string>
using  namespace  std;
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		class CodeAssistUtil
		{
		public:
			static std::vector<wstring>       m_lszpBasicTypes;
			static std::vector<wstring>       m_lszpPostFixOps;
			static std::vector<wstring>    m_lszpInfixOps;
			static std::vector<wstring>       m_lszpPrefixOps;
			static std::vector<wstring>       m_lszpAssignmentOps;
			static std::vector<wstring>        m_lszpModifiers;
			static std::vector<wstring>       m_lszpJavaKeyword;
			static  std::vector<wstring>         m_lszpFIRSTexpr;
			static bool IsTokenThatMatchCodeAssist(const std::wstring& strKeyWord);
			static bool IsInArray(const std::wstring& strKeyWord, const wchar_t** strArray);
			static bool IsInArray(const std::wstring& strKeyWord, vector<wstring>& strArray);
			static bool IsReservedJavaToken(const std::wstring& strKeyWord);

		};

	}
}
