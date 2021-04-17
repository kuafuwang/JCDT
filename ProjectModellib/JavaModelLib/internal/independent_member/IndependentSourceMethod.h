
#ifndef IndependentSourceMethod_java_model_INCLUDED_INCLUDED
#define IndependentSourceMethod_java_model_INCLUDED_INCLUDED


#include <JavaModelLib/internal/core/BaseMethod.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class IndependentSourceMethod:
		public BaseMethod 
		
	{
	public:
		
		IJavaElement* ShallowCopy(IJavaElement* parent) override;
		 IndependentSourceMethod(IJavaElement* parent,
			const wstring& name_symbol_,
			 const vector<wstring>& parameterTypes);
		 IndependentSourceMethod(IndependentSourceMethod&);
	

		MemberElementInfo* getElementInfo() override;;
	};


	




}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

