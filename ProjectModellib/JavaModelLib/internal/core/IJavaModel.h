
#ifndef IJavaModel_java_model_INCLUDED_INCLUDED
#define IJavaModel_java_model_INCLUDED_INCLUDED

#include  "Openable.h"


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class IJavaModel
		:  public Openable	  
	{
	public:
		wstring getPath() override {
			return{};
		}
		IJavaModel(const wstring& name)
			: Openable(nullptr, IJavaElementType::JAVA_MODEL, name)
		{

		}


		
	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

