
#ifndef IndependentSourceField_java_model_INCLUDED_INCLUDED
#define IndependentSourceField_java_model_INCLUDED_INCLUDED
#include <JavaModelLib/internal/core/IField.h>


namespace Jikes { // Open namespace Jikes block
	

namespace JavaModel{
	
	class IndependentSourceField:
		 public IField 
	{
	public:
		IField* ShallowCopy(IJavaElement* parent) override;

		IndependentSourceField(IJavaElement* parent, const wstring& name, IJavaElementType _type);
		IndependentSourceField(IndependentSourceField&o);

		MemberElementInfo* getElementInfo() override;;
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

