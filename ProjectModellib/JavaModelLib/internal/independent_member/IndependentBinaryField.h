
#ifndef IndependentBinaryField_java_model_INCLUDED_INCLUDED
#define IndependentBinaryField_java_model_INCLUDED_INCLUDED
#include <JavaModelLib/internal/core/IField.h>



namespace Jikes { // Open namespace Jikes block
	class LiteralValue;

namespace JavaModel{
	class IType;

	class IndependentBinaryField
		:  public IField 
	{
	public:
		 bool isBinary() override
		{
			return true;
		}
		IField* ShallowCopy(IJavaElement*) override;
		
		 IndependentBinaryField(IType* parent, const wstring& name_symbol_, IJavaElementType _type);
		 IndependentBinaryField(IndependentBinaryField&);


		virtual ~IndependentBinaryField() { }

		MemberElementInfo* getElementInfo() override;;
	};

	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

