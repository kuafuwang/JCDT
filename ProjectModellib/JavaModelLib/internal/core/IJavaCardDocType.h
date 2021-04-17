
#ifndef IJavaCardDocType_java_model_INCLUDED_INCLUDED
#define IJavaCardDocType_java_model_INCLUDED_INCLUDED
#include <string>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IJavaCardDocType
	{
	public: 
		virtual ~IJavaCardDocType()
		{
			
		}
		virtual std::wstring getJavaCardDocUrlFragment() = 0;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

