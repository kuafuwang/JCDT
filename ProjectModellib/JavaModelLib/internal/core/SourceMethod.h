
#ifndef SourceMethod_java_model_INCLUDED_INCLUDED
#define SourceMethod_java_model_INCLUDED_INCLUDED

#include "BaseMethod.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class SourceMethod:
		public BaseMethod
		
	{
	public:
		
		IJavaElement* ShallowCopy(IJavaElement* parent) override;
		 SourceMethod(IJavaElement* parent,
			const wstring& name_symbol_,const  vector<wstring>& parameterNames);
		 SourceMethod(SourceMethod&);
	
	};


	




}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

