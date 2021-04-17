
#ifndef SimilarElement_Jikes_Correction_INCLUDED_INCLUDED
#define SimilarElement_Jikes_Correction_INCLUDED_INCLUDED
#include <string>

namespace Jikes { // Open namespace Jikes block


namespace Correction{
	
	class SimilarElement
	{
	public:
		  int fKind;
		 std::wstring fName;
		 int fRelevance;
		 int modifiers;
		SimilarElement(int kind, std::wstring& _name, int relevance, int _modifiers);

		bool eaqual(SimilarElement* o);
	};
}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

