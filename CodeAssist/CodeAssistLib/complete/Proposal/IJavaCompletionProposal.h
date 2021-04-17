
#ifndef AutoCompleteInfo_Jikes_CodeAssist_INCLUDED_INCLUDED
#define AutoCompleteInfo_Jikes_CodeAssist_INCLUDED_INCLUDED
#include <locale>
#include <string>

using std::wstring;

namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	
	class IJavaCompletionProposal{
	public:
		enum ProposalType
		{
			AnonymousTypeProposal,
			ClassProposal,
			FieldProposal,
			InterfaceProposal,
			LocalVariableProposal,
			MethodProposal,
			MethodDeclarationProposal,
			ModifierProposal,
			PacakgeProposal,
			TypeProposal,
			VariableNameProposal,
			KeywordProposal
		};

		IJavaCompletionProposal(ProposalType i_type,
		                        int access, double  _relevance);

		virtual ~IJavaCompletionProposal()
		{
			
		}
		double getRelevance()
		{
			return relevance;
		}
		virtual const wstring&  GetDisplayString()
		{
			return  fDisplayString;
		};
		virtual const wstring& GetReplacementString()
		{
		//	return fReplacementString;
			return  fDisplayString;
		};
		virtual wstring GetInfoFotTip()
		{
			return L"";
		}
		
		virtual  const wchar_t* GetTokenFroMatch()  {
			return fDisplayString.c_str();
		}
		bool needToCheckMatch;
		wstring fDisplayString;
		wstring fReplacementString;
		ProposalType  iType;
		int access;
		double relevance;

	};

	inline IJavaCompletionProposal::IJavaCompletionProposal(ProposalType i_type, int access, double  _relevance):
		 needToCheckMatch(true),
		iType(i_type),
		access(access),
		relevance(_relevance) 
	{

	}
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

